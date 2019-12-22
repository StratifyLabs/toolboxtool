#ifndef TEST_HPP
#define TEST_HPP


#include <sapi/test/Test.hpp>
#include <sapi/sys/Cli.hpp>

class Test : public test::Test {
public:
	Test(const var::String & name) : test::Test(name){}

	static bool handle_tests(sys::Cli & cli);

};

#endif // TEST_HPP
