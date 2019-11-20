#include "Test.hpp"


#include "SCopeTest.hpp"
#include "TouchTest.hpp"
#include "WifiSpiTest.hpp"
#include "DisplayTest.hpp"
#include "SignalGeneratorTest.hpp"

bool Test::handle_tests(sys::Cli & cli){

	if( cli.get_option(
			 arg::OptionName("test")
			 ) == "true" ){

		test::Test::initialize(
					arg::Name(cli.name()),
					arg::VersionEncodedString(cli.version()),
					arg::GitHash(cli.app_git_hash())
					);

		if( cli.get_option(
				 arg::OptionName("touch")
				 ) == "true" ){
			TouchTest test;
			test.execute(cli);
		}

		if( cli.get_option(
				 arg::OptionName("wifispi")
				 ) == "true" ){
			WifiSpiTest test;
			test.execute(cli);
		}

		if( cli.get_option(
				 arg::OptionName("scope")
				 ) == "true" ){
			ScopeTest test;
			test.execute(cli);
		}

		if( cli.get_option(
				 arg::OptionName("signal")
				 ) == "true" ){
			SignalGeneratorTest test;
			test.execute(cli);
		}

		if( cli.get_option(
				 arg::OptionName("display")
				 ) == "true" ){
			DisplayTest test;
			test.execute(cli);
		}

		test::Test::finalize();

		return true;
	}


	return false;
}
