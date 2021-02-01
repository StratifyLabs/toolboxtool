#include "Test.hpp"

#include "DisplayTest.hpp"
#include "GraphTest.hpp"
#include "LogicTest.hpp"
#include "PioExtenderTest.hpp"
#include "SCopeTest.hpp"
#include "SignalGeneratorTest.hpp"
#include "SpiTest.hpp"
#include "ToolboxIoTest.hpp"
#include "TouchTest.hpp"
#include "UartTest.hpp"
#include "WifiSpiTest.hpp"
#include "WifiTest.hpp"

bool Test::handle_tests(sys::Cli &cli) {

  if (cli.get_option("test") == "true") {

    test::Test::initialize(
      test::Test::Name(cli.name()),
      test::Test::VersionEncodedString(cli.version()),
      test::Test::GitHash(cli.app_git_hash()));

    if (cli.get_option("touch") == "true") {
      TouchTest().execute(cli);
    }

    if (cli.get_option("uart") == "true") {
      UartTest().execute(cli);
    }

    if (cli.get_option("spi") == "true") {
      SpiTest().execute(cli);
    }

    if (cli.get_option("wifispi") == "true") {
      WifiSpiTest().execute(cli);
    }

    if (cli.get_option("wifi") == "true") {
      WifiTest().execute(cli);
    }

    if (cli.get_option("scope") == "true") {
      ScopeTest().execute(cli);
    }

    if (cli.get_option("signal") == "true") {
      SignalGeneratorTest().execute(cli);
    }

    if (cli.get_option("display") == "true") {
      DisplayTest().execute(cli);
    }

    if (cli.get_option("graph") == "true") {
      GraphTest().execute(cli);
    }

    if (cli.get_option("pio") == "true") {
      PioExtenderTest().execute(cli);
    }

    if (cli.get_option("logic") == "true") {
      LogicTest().execute(cli);
    }

    if (cli.get_option("io") == "true") {
      ToolboxIoTest().execute(cli);
    }

    test::Test::finalize();

    return true;
  }

  return false;
}
