#include "SignalGeneratorTest.hpp"

#include <sapi/hal.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <cmath>

SignalGeneratorTest::SignalGeneratorTest() :
	Test("SignalGeneratorTest"){

}

bool SignalGeneratorTest::execute_class_api_case(){
	StreamFFifo signal_generator;
	Tmr signal_generator_tmr(14);
	Timer t;

	signal_generator_tmr.initialize();
	if( signal_generator_tmr.return_value() < 0 ){
		print_case_failed(signal_generator.result(), __LINE__);
		return case_result();
	}
	signal_generator_tmr.enable();
	if( signal_generator_tmr.return_value() < 0 ){
		print_case_failed(signal_generator.result(), __LINE__);
		return case_result();
	}

	t.start();
	do {
		print_case_message(
					"Timer: %ld",
					signal_generator_tmr.get_value()
					);
		wait(Milliseconds(100));
	} while( t < Seconds(1) );

	print_case_message("open dac0");
	signal_generator.open(
				"/dev/dac0",
				fs::OpenFlags::read_write()
				);

	StreamFFifoInfo info = signal_generator.get_info();

	print_case_message(
				"tx buffer frame size %d, frame count %d",
				info.transmit().ffifo().frame_size(),
				info.transmit().ffifo().frame_count()
				);

	signal_generator.ioctl(
				Device::IoRequest(I_DAC_SETATTR)
				);

#if 0
	print_case_message("manual ramp");
	mcu_channel_t channel;
	for(u32 i=0; i < 4096; i++){
		channel.loc = 0;
		channel.value = i;
		signal_generator.ioctl(
					arg::IoRequest(I_DAC_SET),
					arg::IoArgument(&channel)
					);

		wait(Milliseconds(1));
	}
#endif

	if( signal_generator.return_value() < 0 ){
		print_case_failed(signal_generator.result(), __LINE__);
		return case_result();
	}

	Data frame(info.transmit().ffifo().frame_size());

	frame.populate<u32>(
				[](Reference::Position position, Reference::Count count)->u32{
		return position.argument()*4;
	}
	);


	write_signal(
				signal_generator,
				frame,
				Seconds(2)
				);

	frame.populate<u32>(
				[](Reference::Position position, Reference::Count count)->u32{
		u32 max = count.argument()*4-1;
		u32 middle = count.argument()/2;
		return position.argument() < middle ?
					position.argument() * max / middle :
					max - (position.argument() - middle) * max /middle;
	}
	);

	write_signal(
				signal_generator,
				frame,
				Seconds(2)
				);

	frame.populate<u32>(
				[](Reference::Position position, Reference::Count count)->u32{
		u32 max = count.argument()*4-1;
		float value = sinf(position.argument() * 2 * MCU_PI_FLOAT / count.argument());
		return (value + 1.0f) * max/2;
	}
	);

	write_signal(
				signal_generator,
				frame,
				Seconds(2)
				);

	frame.populate<u32>(
				[](Reference::Position position, Reference::Count count)->u32{
		u32 middle = count.argument() / 2;
		u32 maximum = count.argument() * 4-1;
		return position.argument() < middle ?
					maximum :
					0;
	}
	);

	write_signal(
				signal_generator,
				frame,
				Seconds(2)
				);


	wait(Milliseconds(10));

	return case_result();

}


bool SignalGeneratorTest::write_signal(
		StreamFFifo & stream,
		const Data & frame,
		const MicroTime & duration
		){

	Timer t;
	//fill the buffers
	stream.write(frame);

	stream.write(frame);

	if( stream.start() < 0 ){
		return case_result();
	}

	t.start();
	do {
		stream.write(frame);
	} while( t < duration );


	if( stream.stop() < 0 ){
		return false;
	}
	return true;
}
