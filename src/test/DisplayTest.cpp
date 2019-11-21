#include "DisplayTest.hpp"

#include <sapi/hal.hpp>

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


	return case_result();
}
