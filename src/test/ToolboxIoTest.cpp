
#include <ToolboxAPI/toolbox.hpp>
#include <sapi/hal.hpp>

#include "ToolboxIoTest.hpp"

ToolboxIoTest::ToolboxIoTest() : Test("IoTest"){

}

bool ToolboxIoTest::execute_class_api_case(){

   IoAttributes attributes;

   attributes
         .set_peripheral_port(Io::pio_port)
         .set_pin(Io::pin0)
         .set_flags(Io::set_io_function | Io::is_output);

   Pin pin0(
            Pin::Port(0),
            Pin::Number(0)
            );

   TEST_THIS_ASSERT(bool, pin0.open() < 0, false);
   TEST_THIS_ASSERT(bool, Io::set_attributes( attributes ) < 0, false);
   TEST_THIS_ASSERT(bool, pin0.set_output() < 0, false);


   return case_result();
}
