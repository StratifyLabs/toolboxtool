#ifndef POWERTOUCHTEST_HPP
#define POWERTOUCHTEST_HPP

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/chrono.hpp>

#include "Test.hpp"

class PowerTouchTest : public Test {
public:
   PowerTouchTest();

   bool execute_class_api_case();

};

#endif // POWERTOUCHTEST_HPP
