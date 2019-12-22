#ifndef GRAPHTEST_HPP
#define GRAPHTEST_HPP


#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/chrono.hpp>

#include "Test.hpp"

class GraphTest : public Test
{
public:
	GraphTest();

	bool execute_class_api_case();

};

#endif // GRAPHTEST_HPP
