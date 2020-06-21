
#include <ToolboxAPI/toolbox.hpp>
#include <sapi/hal.hpp>
#include <sapi/chrono.hpp>

using namespace toolbox;

#include "ToolboxIoTest.hpp"

ToolboxIoTest::ToolboxIoTest() : Test("IoTest"){

}

bool ToolboxIoTest::execute_class_api_case(){

	 TEST_THIS_ASSERT(bool, Io::set_vdd_out(true), false);

   Pin pin0(
            Pin::Port(0),
            Pin::Number(1)
            );

	 Io pin(Io::io_pin1);

	 TEST_THIS_ASSERT(bool, pin.set_output() < 0, false);

   Timer t;
   t.start();
   while(t < Seconds(20) ){
      pin0 = true;
      wait(Seconds(2));
      pin0 = false;
      wait(Seconds(2));
   }

   return case_result();
}
