#include "GraphTest.hpp"

#include <sapi/hal.hpp>
#include <sapi/draw.hpp>
#include <sapi/sgfx.hpp>
#include <sapi/sys.hpp>
#include <sapi/var.hpp>

GraphTest::GraphTest()
	: Test("GraphTest"){

}


bool GraphTest::execute_class_api_case(){
	DisplayDevice display;
	Timer t;

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

	if( display.set_mode(DisplayDevice::PALETTE) < 0 ){
		print_case_failed("failed to set raw mode");
		return case_result();
	}

	display.fill(0);

	DrawingAttributes drawing_attributes;

	drawing_attributes.set_bitmap(display)
			.set_point(DrawingPoint(0,0))
			.set_area(DrawingArea(1000,1000));


	LineGraph line_graph;

	line_graph.x_data().push_back(0.0f);
	line_graph.x_data().push_back(1.0f);
	line_graph.x_data().push_back(2.0f);

	line_graph.x_axis()
			.set_minimum(0.0f)
			.set_maximum(2.0f);

	line_graph.y_axes()
			.push_back(
				Axis()
				.set_minimum(0.0f)
				.set_maximum(2.0f)
				);

	{
		YData y_data;
		y_data.set_color(1)
				.push_back(1.0f)
				.push_back(1.0f)
				.push_back(2.0f);

		line_graph.y_data()
				.push_back(
					y_data
					);
	}

	line_graph.draw(
				drawing_attributes, DrawingPoint(100,100), DrawingArea(800,800)
				);

	Region window(
				Point(0,0),
				Area(320,240)
				);

	if( display.set_window(window) < 0 ){
		print_case_failed("failed to set window");
		return case_result();
	}

	if( display.write(*display.bmap()) < 0 ){
		print_case_failed(
					"failed to write device (%d,%d)",
					display.return_value(),
					display.error_number());

	}


	return case_result();
}
