#include "DisplayTest.hpp"

#include <sapi/hal.hpp>
#include <sapi/ux.hpp>
#include <sapi/sgfx.hpp>
#include <sapi/sys.hpp>
#include <sapi/var.hpp>

DisplayTest::DisplayTest()
	: Test("DisplayTest"){

}


bool DisplayTest::execute_class_api_case(){
	DisplayDevice display;
	Timer t;

	Bitmap scratch;

	print_case_message("initialize display");
	if( display.initialize("/dev/display0") < 0
		 ) {
		print_case_failed("failed to initialize the display");
	}

	print_case_message("clear display");
	display.clear();
	if( display.return_value() < 0 ){
		print_case_failed("Failed to clear display");
	}

	if( display.set_mode(DisplayDevice::mode_palette) < 0 ){
		print_case_failed("failed to set palette mode");
		return case_result();
	}

	display.fill(0);

	DrawingAttributes drawing_attributes;

	scratch.allocate(
				Area(320, 48),
				Bitmap::BitsPerPixel(4)
				);

	drawing_attributes
			.set_bitmap(scratch)
			.set_point(DrawingPoint(0,0))
			.set_area(DrawingArea(1000,1000));

	t.restart();

	sg_color_t color = 11;
	Array<drawing_int_t, 3> x_locations;
	x_locations.at(0) = 25;
	x_locations.at(1) = 375;
	x_locations.at(2) = 725;

	Text().set_string("Waveform")
			.set_color(3)
			.set_align_center()
			.set_align_middle()
			.draw(drawing_attributes, DrawingPoint(0, 0), DrawingArea(1000, 1000));

	scratch << Pen().set_color(3);
	scratch.draw_line(Point(0,0), scratch.center());

	display.set_window(
				Region(
					Point(0,0),
					scratch.area()
					)
				);

	if( display.return_value() < 0 ){
		print_case_failed("Failed to set display window");
	}

	display.write(scratch);

	if( display.return_value() < 0 ){
		print_case_failed("Failed to write display");
	}

	return true;

	for(auto x: x_locations){
	RoundedRectangle()
			.set_radius(40)
			.set_color(color)
			.draw(drawing_attributes, DrawingPoint(x, 500-250/2),  DrawingArea(250, 250));
	}
	t.stop();
	print_case_message(
				"drew rounded rectangles in %ld microseconds",
				t.microseconds()
				);

	Region window(
				Point(0,0),
				Area(64,64)
				);

	if( display.set_window(window) < 0 ){
		print_case_failed("failed to set window");
		return case_result();
	}

	t.restart();
	if( display.write(*display.bmap()) < 0 ){
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
