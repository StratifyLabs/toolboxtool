#include <ToolboxAPI/toolbox.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fs.hpp>
#include <sapi/hal.hpp>
#include <sapi/var.hpp>

#include "SpiTest.hpp"

SpiTest::SpiTest() : Test("SpiTest") {}

bool SpiTest::execute_class_api_case() {

  print_case_message("MISO and MOSI must be shorted for this test");
  // are the pins available?
  Vector<enum Io::io_pins> pin_list
    = IoInfo::get_pin_list(Io::peripheral_function_spi, 0);

  print_case_message("uart has %d pins", pin_list.count());
  bool is_pins_free = true;
  for (auto pin : pin_list) {
    print_case_message("SPI port %d uses %d", 0, pin);

    // is the pin available
    IoInfo pin_state(pin);
    if (pin_state.peripheral_function() == Io::peripheral_function_gpio) {
      print_case_message("pin %d is free for use", pin);
    } else {
      print_case_message("pin %d is busy", pin);
      is_pins_free = false;
    }
  }

  if (!is_pins_free) {
    print_case_message("pins are busy, aborting test");
    return true;
  }

  print_case_message("Running loopback test");
  print_case_message("acquiring pins for test");

  TEST_THIS_ASSERT(
    bool,
    Io::lock_peripheral_pins(Io::peripheral_port_spi) < 0,
    false);

  TEST_THIS_ASSERT(
    bool,
    Io::unlock_peripheral_pins(Io::peripheral_port_spi) < 0,
    false);

  {
    Io::FunctionLock function_lock(Io::peripheral_port_spi);
    Spi peripheral(0);

    TEST_THIS_EXPECT(
      bool,
      peripheral.open(OpenFlags().set_read_write().set_non_blocking()) < 0,
      false);

    if (peripheral.return_value() < 0) {
      print_case_failed(peripheral.result());
      return case_result();
    }

    TEST_THIS_ASSERT(bool, peripheral.set_attributes() < 0, false);

    String outgoing = "Hello";
    Data incoming(64);

    Timer t;
    t.start();

    Aio aio(incoming);

    do {
      TEST_THIS_EXPECT(bool, peripheral.read(aio) == 0, true);

      TEST_THIS_ASSERT(
        bool,
        peripheral.write(outgoing) == outgoing.length_signed(),
        true);
      wait(Milliseconds(1));

      if (peripheral.return_value() > 0) {
        String incoming_string(
          incoming.to_const_char(),
          String::Length(peripheral.return_value()));
        print_case_message("outgoing '%s'", outgoing.cstring());
        print_case_message("incoming '%s'", incoming_string.cstring());

        if (outgoing != incoming_string) {
          print_case_failed("outgoing != incoming");
        }

      } else {
        print_case_failed(peripheral.result(), __LINE__);
      }
      wait(Milliseconds(1));
    } while (t < Seconds(2000));
  }

  return case_result();
}
