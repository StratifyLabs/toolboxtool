#include "DisplayTest.hpp"

#include <sapi/hal.hpp>
#include <sapi/sgfx.hpp>
#include <sapi/sys.hpp>
#include <sapi/var.hpp>

DisplayTest::DisplayTest() : Test(arg::Name("DisplayTest")){

}


bool DisplayTest::execute_class_api_case(){



	DisplayDevice display;

	print_case_message("initialize display");
	if( display.initialize(
			 arg::SourceFilePath("/dev/display0")
			 ) < 0
		 ) {
		print_case_failed("failed to initialize the display");
	}

	print_case_message("clear display");
	display.clear();
	if( display.return_value() < 0 ){
		print_case_failed("Failed to clear display");
	}

	if( display.set_mode(DisplayDevice::PALETTE) < 0 ){
		print_case_failed("failed to set raw mode");
		return case_result();
	}


#if 0
	Bitmap sketch;
	sketch.set_bits_per_pixel(4);

	if( sketch.allocate(
				Area(arg::Width(320), arg::Height(240))
				) < 0 ){
		print_case_failed("failed to allocate bitmap memory");
		return case_result();
	}

	print_case_message("bits per pixel is %d", sketch.bits_per_pixel());

	sketch.clear();

	sketch.set_pen(
				Pen()
				.set_color(8)
				.set_thickness(1)
				.set_solid()
				);

	sketch.draw_line(
				Point(arg::XValue(0), arg::YValue(0)),
				Point(arg::XValue(127), arg::YValue(127))
				);

	sketch.set_pen(
				Pen()
				.set_color(4)
				.set_thickness(1)
				.set_solid()
				);

	sketch.draw_rectangle(Region(
				Point(arg::XValue(10), arg::YValue(0)),
				Area(arg::Width(40), arg::Height(40))
				));

	Printer p;

	p << sketch;
#endif

	display.fill(0);

	display.draw_rectangle(Region(
				Point(arg::XValue(10), arg::YValue(0)),
				Area(arg::Width(80), arg::Height(80))
				));
	Region window(Point(arg::XValue(0), arg::YValue(0)),
			 display.area());

	if( display.set_window(window) < 0 ){
		print_case_failed("failed to set window");
		return case_result();
	}

	Timer t;
	t.start();
	if( display.write(
			 arg::SourceBuffer(display.bmap()),
			 arg::Size(sizeof(sg_bmap_t))
			 ) < 0 ){
		print_case_failed(
					"failed to write device (%d,%d)",
					display.return_value(),
					display.error_number());

	}
	t.stop();
	print_case_message(
				"wrote %d bytes in %d us",
				display.return_value(),
				t.microseconds()
				);



	return case_result();
}
