#include <sapi/chrono.hpp>
#include <sapi/fs.hpp>

#include <ToolboxAPI/Io.hpp>
#include <ToolboxAPI/Logic.hpp>

#include "LogicTest.hpp"

using namespace toolbox;

LogicTest::LogicTest() : Test("LogicTest") {}

bool LogicTest::execute_class_api_case() {
  Io::set_vdd_out();

  Io io(Io::io_pin4);
  // Io(Io::io_pin2).set_input();
  Io(Io::io_pin8).set_input();
  Io(Io::io_pin9).set_input();
  Io(Io::io_pin12).set_input();

  io.set_output();

  toolbox::Logic logic;
  logic.start();

  Timer t;
  t.start();

  do {
    logic.test(0);
    wait(Milliseconds(1));
  } while (1);
#if 0
  for (u32 i = 0; i < 40; i++) {
    io.set_value(true);
    // wait(Milliseconds(1));
    io.set_value(false);
    // wait(Milliseconds(1));
  }
#endif
  t.stop();

  logic.stop();
  print_case_message("period:%ld", t.microseconds());

  toolbox::Logic::Capture capture = logic.read(Logic::channel_0);

  print_case_message("initial value is %d", capture.initial_value());

  return case_result();
}
