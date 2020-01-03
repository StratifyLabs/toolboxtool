#include "TouchTest.hpp"

#include <sapi/hal.hpp>
#include <sapi/chrono.hpp>
#include <sapi/var.hpp>


#include <ToolboxAPI/Touch.hpp>
using namespace toolbox;

TouchTest::TouchTest() :
	Test("TouchTest"){

}


bool TouchTest::execute_class_api_case(){


	Microseconds duration = Seconds(10);

	Timer t;

	Touch touch;

	t.start();
	while( t < duration ){
		int touch_count;
		if( (touch_count = touch.read()) > 0 ){

			print_case_message(
						"primary touch: %d,%d (%d,%d,%d) %d,%d",
						touch.at(Touch::position_primary).point().x(),
						touch.at(Touch::position_primary).point().y(),
						touch.at(Touch::position_primary).is_pressed(),
						touch.at(Touch::position_primary).is_released(),
						touch.at(Touch::position_primary).is_active(),
						touch.at(Touch::position_primary).pressure(),
						touch.at(Touch::position_primary).area()
						);

			if( touch_count > 1 ){
				print_case_message(
							"secondary touch: %d,%d (%d,%d,%d) %d,%d",
							touch.at(Touch::position_secondary).point().x(),
							touch.at(Touch::position_secondary).point().y(),
							touch.at(Touch::position_secondary).is_pressed(),
							touch.at(Touch::position_secondary).is_released(),
							touch.at(Touch::position_secondary).is_active(),
							touch.at(Touch::position_secondary).pressure(),
							touch.at(Touch::position_secondary).area()
							);
			}


		}

		wait(Milliseconds(10));

	}

	return case_result();
}
