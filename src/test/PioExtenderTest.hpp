#ifndef PIOEXTENDERTEST_HPP
#define PIOEXTENDERTEST_HPP

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/chrono.hpp>

#include "Test.hpp"

class PioExtenderTest : public Test {
public:
   PioExtenderTest();

   bool execute_class_api_case();

private:
   void show_registers(hal::I2C & i2c, u8 slave_address);

   bool configure_pin(hal::I2C & i2c, u8 slave_address, int flags,  u16 mask);
   bool configure_setmask(hal::I2C & i2c, u8 slave_address, u16 mask);
   bool configure_clrmask(hal::I2C & i2c, u8 slave_address, u16 mask);

};

#endif // PIOEXTENDERTEST_HPP
