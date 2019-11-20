#ifndef DISPLAYTEST_HPP
#define DISPLAYTEST_HPP

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/chrono.hpp>

#include "Test.hpp"

class DisplayTest : public Test
{
public:
	DisplayTest();

	bool execute_class_api_case();

};

#endif // DISPLAYTEST_HPP
