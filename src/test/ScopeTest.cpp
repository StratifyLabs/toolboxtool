#include "ScopeTest.hpp"

#include <sapi/hal.hpp>

ScopeTest::ScopeTest() : Test("ScopeTest"){

}



bool ScopeTest::execute_class_api_case(){

	Pin div_10_enable(
				Pin::Port(4),
				Pin::Number(0)
				);

	Pin vdd_out_enable(
				Pin::Port(4),
				Pin::Number(1)
				);

	Pin vref_enable(
				Pin::Port(3),
				Pin::Number(6)
				);


	vdd_out_enable.set_output();
	vdd_out_enable	= false;
	vref_enable.set_input(Pin::IS_FLOAT);
	div_10_enable.set_input(Pin::IS_FLOAT);
	//div_10_enable.set_output();
	//div_10_enable = false;

	print_case_message("div10 result is %d", div_10_enable.return_value());

	return case_result();
}
