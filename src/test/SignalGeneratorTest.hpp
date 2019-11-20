#ifndef SIGNALGENERATORTEST_HPP
#define SIGNALGENERATORTEST_HPP

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/chrono.hpp>

#include "Test.hpp"

class SignalGeneratorTest : public Test
{
public:
	SignalGeneratorTest();

	bool execute_class_api_case();

private:
	bool write_signal(
			StreamFFifo & stream,
			const Data & frame,
			const MicroTime & duration
			);

};

#endif // SIGNALGENERATORTEST_HPP
