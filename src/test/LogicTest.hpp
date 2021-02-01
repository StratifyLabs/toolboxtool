#ifndef LOGICTEST_HPP
#define LOGICTEST_HPP

#include <sapi/chrono.hpp>
#include <sapi/hal.hpp>
#include <sapi/var.hpp>

#include "Test.hpp"

class LogicTest : public Test {
public:
  LogicTest();

  bool execute_class_api_case();

private:
};

#endif // PIOEXTENDERTEST_HPP
