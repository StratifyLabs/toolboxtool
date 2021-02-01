#include <sapi/chrono.hpp>
#include <sapi/fs.hpp>
#include <sapi/hal.hpp>
#include <sapi/test.hpp>
#include <sapi/var.hpp>

#include "UartTest.hpp"

UartTest::UartTest() : Test("UartTest") {}

bool UartTest::execute_class_api_case() {

  print_case_message(
    "This is a UART loopback test, the RX/TX pins need to be shorted");

  TEST_THIS_ASSERT(bool, execut_uart_listen_case(1), true);

  // TEST_THIS_ASSERT(bool, execute_uart_api_case(0), true);
  // TEST_THIS_ASSERT(bool, execute_uart_api_case(1), true);
  // TEST_THIS_ASSERT(bool, execute_uart_api_case(2), true);
  // TEST_THIS_ASSERT(bool, execute_uart_api_case(3), true);

  return case_result();
}

bool UartTest::execut_uart_listen_case(u32 port) {
  test::Case cg(this, "UartListen " + String::number(port));

  enum Io::peripheral_ports peripheral_port
    = Io::get_peripheral_port(Io::peripheral_function_uart, port);

  Io::FunctionLock function_lock(peripheral_port);
  Uart uart(port);

  print_case_message("Open UART");
  TEST_THIS_EXPECT(
    bool,
    uart.open(OpenFlags().set_read_write().set_non_blocking()) < 0,
    false);

  if (uart.return_value() < 0) {
    print_case_failed(uart.result());
    return case_result();
  }

  print_case_message("set attributes");
  TEST_THIS_ASSERT(bool, uart.set_attributes() < 0, false);

  Data incoming(64);

  Timer t;
  t.start();

  do {
    uart.read(incoming);

    if (uart.return_value() > 0) {
      String incoming_string(
        incoming.to_const_char(),
        String::Length(uart.return_value()));
      print_case_message("incoming '%s'", incoming_string.cstring());
    }
    wait(Milliseconds(1));
  } while (t < Seconds(2000));

  return case_result();
}

bool UartTest::execute_uart_api_case(u32 port) {
  test::Case cg(this, "Uart " + String::number(port));

  // are the UART pins available?
  Vector<enum Io::io_pins> uart_pin_list
    = IoInfo::get_pin_list(Io::peripheral_function_uart, port);

  print_case_message("uart has %d pins", uart_pin_list.count());
  bool is_uart_pins_free = true;
  for (auto pin : uart_pin_list) {
    print_case_message("Uart port %d uses %d", port, pin);

    // is the pin available
    IoInfo pin_state(pin);
    if (pin_state.peripheral_function() == Io::peripheral_function_gpio) {
      print_case_message("pin %d is free for UART use", pin);
    } else {
      print_case_message("pin %d is busy", pin);
      is_uart_pins_free = false;
    }
  }

  if (!is_uart_pins_free) {
    print_case_message("UART pins are busy, aborting test");
    return true;
  }

  print_case_message("Running loopback test");

  print_case_message("acquiring pins for test");
  enum Io::peripheral_ports peripheral_port
    = Io::get_peripheral_port(Io::peripheral_function_uart, port);
  TEST_THIS_ASSERT(bool, Io::lock_peripheral_pins(peripheral_port) < 0, false);

  TEST_THIS_ASSERT(
    bool,
    Io::unlock_peripheral_pins(peripheral_port) < 0,
    false);

  {
    Io::FunctionLock function_lock(peripheral_port);
    Uart uart(port);

    TEST_THIS_EXPECT(
      bool,
      uart.open(OpenFlags().set_read_write().set_non_blocking()) < 0,
      false);

    if (uart.return_value() < 0) {
      print_case_failed(uart.result());
      return case_result();
    }

    TEST_THIS_ASSERT(bool, uart.set_attributes() < 0, false);

    String outgoing = "Hello";
    Data incoming(64);

    Timer t;
    t.start();

    do {
      TEST_THIS_ASSERT(
        bool,
        uart.write(outgoing) == outgoing.length_signed(),
        true);
      wait(Milliseconds(50));
      TEST_THIS_EXPECT(
        bool,
        uart.read(incoming) == outgoing.length_signed(),
        true);

      if (uart.return_value() > 0) {
        String incoming_string(
          incoming.to_const_char(),
          String::Length(uart.return_value()));
        print_case_message("outgoing '%s'", outgoing.cstring());
        print_case_message("incoming '%s'", incoming_string.cstring());

        if (outgoing != incoming_string) {
          print_case_failed("outgoing != incoming");
        }

      } else {
        print_case_failed(uart.result());
      }
      wait(Milliseconds(1));
    } while (t < Seconds(2000));
  }

  return case_result();
}
