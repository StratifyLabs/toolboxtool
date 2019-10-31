#include "TouchTest.hpp"

#include <sapi/hal.hpp>
#include <sapi/chrono.hpp>
#include <sapi/var.hpp>

TouchTest::TouchTest() :
	Test(arg::Name("TouchTest")){

}


bool TouchTest::execute_class_api_case(){


	Microseconds duration = Seconds(10);

	I2C i2c(2); //touch is on I2C 2
	Pin alert( //PC13
				  arg::PortNumber(2),
				  arg::PinNumber(13)
				  );

	print_case_message("initialize alert pin");
	alert.initialize(
				Pin::SET_INPUT | Pin::IS_PULLUP
				);

	if( alert.return_value() <  0 ){
		print_case_failed(alert.result(), __LINE__);
		return case_result();
	}

	print_case_message("initialize i2c");
	if( i2c.initialize() < 0 ){
		print_case_failed(i2c.result(), __LINE__);
		return case_result();
	}

	i2c.prepare(0x38);
	i2c.write(arg::Location(ft_device_mode), 0x00);
	i2c.write(arg::Location(ft_id_g_mode), 0x00);
	i2c.write(arg::Location(ft_id_g_thgroup), 22);
	i2c.write(arg::Location(ft_id_g_periodactive), 12);


	Timer t;

	touch_packet_t touch_packet;
	DataReference touch_data(touch_packet);

	t.start();
	while( t < duration ){

		if( alert.get_value() == false ){

			i2c.prepare(0x38);
			i2c.read(
						arg::Location(ft_reg_num_finger),
						arg::DestinationData(touch_data)
						);


			for(u32 i=0; i < touch_packet.count; i++){
				print_case_message(
							"touch[%d]: %d, %d (%d,%d,%d,%d)",
							i,
							x(touch_packet.point[i]),
							y(touch_packet.point[i]),
							is_press(touch_packet.point[i]),
							is_release(touch_packet.point[i]),
							is_contact(touch_packet.point[i]),
							touch_id(touch_packet.point[i])
							);
			}


		} else {
			wait(Milliseconds(5));
		}
	}



	return case_result();
}
