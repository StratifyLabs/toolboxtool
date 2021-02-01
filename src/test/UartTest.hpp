#ifndef UARTTEST_HPP
#define UARTTEST_HPP

#include <ToolboxAPI/toolbox.hpp>

#include "Test.hpp"

class UartTest : public ::Test {
public:
  UartTest();

  bool execute_class_api_case();

private:
  bool execute_uart_api_case(u32 port);
  bool execut_uart_listen_case(u32 port);
};

#endif // UARTTEST_HPP
