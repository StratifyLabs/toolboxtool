#include "Test.hpp"


#include "SCopeTest.hpp"
#include "TouchTest.hpp"
#include "WifiSpiTest.hpp"
#include "DisplayTest.hpp"
#include "SignalGeneratorTest.hpp"
#include "GraphTest.hpp"
#include "PioExtenderTest.hpp"
#include "ToolboxIoTest.hpp"

bool Test::handle_tests(sys::Cli & cli){

	if( cli.get_option("test") == "true" ){

		test::Test::initialize(
					test::Test::Name(cli.name()),
					test::Test::VersionEncodedString(cli.version()),
					test::Test::GitHash(cli.app_git_hash())
					);

		if( cli.get_option("touch") == "true" ){
			TouchTest test;
			test.execute(cli);
		}

		if( cli.get_option("wifispi") == "true" ){
			WifiSpiTest test;
			test.execute(cli);
		}

		if( cli.get_option("scope") == "true" ){
			ScopeTest test;
			test.execute(cli);
		}

		if( cli.get_option("signal") == "true" ){
			SignalGeneratorTest test;
			test.execute(cli);
		}

		if( cli.get_option("display") == "true" ){
			DisplayTest test;
			test.execute(cli);
		}

		if( cli.get_option("graph") == "true" ){
			GraphTest test;
			test.execute(cli);
		}

		if( cli.get_option("pio") == "true" ){
			PioExtenderTest test;
			test.execute(cli);
		}

		if( cli.get_option("io") == "true" ){
			ToolboxIoTest test;
			test.execute(cli);
		}

		test::Test::finalize();

		return true;
	}


	return false;
}
