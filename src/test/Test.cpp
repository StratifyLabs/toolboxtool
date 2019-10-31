#include "Test.hpp"


#include "TouchTest.hpp"

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
				 arg::OptionName("test")
				 ) == "true" ){
			TouchTest test;
			test.execute(cli);

		}


		test::Test::finalize();

		return true;
	}


	return false;
}
