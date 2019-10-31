
#include <cstdio>

#include "test/Test.hpp"

#include <sapi/sys.hpp>

int main(int argc, char * argv[]){
	Cli cli(argc, argv, SOS_GIT_HASH);


	if( Test::handle_tests(cli) ){
		return 0;
	} else {
		printf("No Tests\n");
	}

	return 0;
}
