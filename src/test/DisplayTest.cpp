#include "DisplayTest.hpp"

#include <sapi/hal.hpp>

DisplayTest::DisplayTest() : Test(arg::Name("DisplayTest")){

}


bool DisplayTest::execute_class_api_case(){


	DisplayDevice display;

	if( display.initialize(
			 arg::SourceFilePath("/dev/display0")
			 ) < 0
		 ) {
		print_case_failed("failed to initialize the display");
	}


	display.clear();


	return case_result();
}
