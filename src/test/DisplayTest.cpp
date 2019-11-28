#include "DisplayTest.hpp"

#include <sapi/hal.hpp>
#include <sapi/draw.hpp>
#include <sapi/sgfx.hpp>
#include <sapi/sys.hpp>
#include <sapi/var.hpp>

DisplayTest::DisplayTest() : Test(arg::Name("DisplayTest")){

}


bool DisplayTest::execute_class_api_case(){
	DisplayDevice display;
	Timer t;

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

	display.fill(0);

#if 0

	display.set_pen(
				display.pen()
				.set_color(3)
				.set_blend()
				);

	display.draw_rectangle(Region(
									  Point(arg::XValue(10), arg::YValue(0)),
									  Area(arg::Width(150), arg::Height(80))
									  )
								  );

	const FontInfo * font_info = Assets::find_font(
				FontInfo::PointSize(34)
				);

	if( font_info && font_info->is_valid() ){
		display.set_pen(
					display.pen()
					.set_color(5)
					.set_blend()
					);
		font_info->font()->draw(
					"Hello",
					display,
					Point(15, 5)
					);

	} else {
		if( font_info != nullptr ){

			print_case_message("valid %d", font_info->is_valid());
			print_case_message("size %d", font_info->point_size());
			print_case_message("font %p", font_info->font());
		}

		print_case_failed("failed to find a font %p", font_info);
	}

#endif

	DrawingAttributes drawing_attributes;

	drawing_attributes.set_bitmap(display)
			.set_point(DrawingPoint(0,0))
			.set_area(DrawingArea(1000,1000));

	t.restart();
	DrawingArea button_area(250, 250);
	sg_color_t color = 11;

	Array<drawing_int_t, 3> x_locations;
	x_locations.at(0) = 25;
	x_locations.at(1) = 375;
	x_locations.at(2) = 725;

	for(auto x: x_locations){
	RoundedRectangle()
			.set_radius(40)
			.set_color(color)
			.draw(drawing_attributes, DrawingPoint(x, 500-250/2),  button_area);
	}
	t.stop();
	print_case_message(
				"drew rounded rectangles in %ld microseconds",
				t.microseconds()
				);

	Region window(
				Point(0,0),
				Area(320,240)
				);

	if( display.set_window(window) < 0 ){
		print_case_failed("failed to set window");
		return case_result();
	}

	t.restart();
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
