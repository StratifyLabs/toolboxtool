
#include <ToolboxAPI/toolbox.hpp>

#include "ToolboxIoTest.hpp"

ToolboxIoTest::ToolboxIoTest() : Test("IoTest"){

}

bool ToolboxIoTest::execute_class_api_case(){

   IoAttributes attributes;

   attributes
         .set_peripheral_port(Io::pio_port)
         .set_pin(Io::pin0)
         .set_flags(Io::set_io_function | Io::is_output);

   int result;
   if( (result = Io::set_attributes( attributes )) <  0 ){
      print_case_failed("Failed with %d", result);
   } else {
      print_case_message("set PIO attributes");
   }

   return case_result();
}
