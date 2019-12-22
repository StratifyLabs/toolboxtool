#include <sapi/hal.hpp>

#include "PioExtenderTest.hpp"

PioExtenderTest::PioExtenderTest() : Test("PioExtenderTest"){

}

/*
 * I2C
 *  - send address
 *  - send command byte
 *   - bits [0:2] = operation (read or write) plus affected registers
 *   - bit 6 used with extended features (output drive strength, input latch, pull up/down, interrupt mask
 *
 */

bool PioExtenderTest::execute_class_api_case(){

   I2C i2c(2); //internal I2C is on

   Pin reset(
            Pin::Port(3),
            Pin::Number(8)
            );

   reset.set_output();
   reset << false << Milliseconds(10) << true << Milliseconds(1);

   u16 register_pair;
   TEST_THIS_ASSERT(bool, i2c.initialize() < 0, false);

   TEST_THIS_ASSERT(bool, configure_pin(i2c, 0x21, Pin::SET_OUTPUT, (1<<15)), true);
   TEST_THIS_ASSERT(bool, configure_clrmask(i2c, 0x21, (1<<15)), true);
   show_registers(i2c, 0x21);

   TEST_THIS_ASSERT(bool, configure_clrmask(i2c, 0x20, 0xffff), true); //set DIR in for all
   TEST_THIS_ASSERT(bool, configure_pin(i2c, 0x20, Pin::SET_OUTPUT, 0x0fff), true);
   TEST_THIS_ASSERT(bool, configure_setmask(i2c, 0x20, 0x01), true); //set DIR out for 1
   show_registers(i2c, 0x20);

   Pin output(
            Pin::Port(1),
            Pin::Number(8)
            );

   Pin input(
            Pin::Port(1),
            Pin::Number(9)
            );

   input.set_input();
   output.set_output();
   Timer t;
   t.start();
   while( t < Seconds(5) ){
      output << true << Milliseconds(500);
      if( !input.get_value() ){
         print_case_failed("not tracking true");
      } else {
         print_case_message("tracking true");
      }

      output << false << Milliseconds(500);
      if( input.get_value() ){
         print_case_failed("not tracking false");
      } else {
         print_case_message("tracking false");
      }
   }

   return case_result();

   register_pair = 0xffff;
   TEST_THIS_ASSERT(bool, i2c.write(I2C::Location(0x02), Reference(register_pair) ) == 2, true);

   show_registers(i2c, 0x21);




   TEST_THIS_ASSERT(bool, i2c.prepare(0x20) < 0, false);
   show_registers(i2c, 0x20);

   TEST_THIS_ASSERT(bool, i2c.read(I2C::Location(0x40), Reference(register_pair)) == 2, true);
   print_case_message(
            "Start Configuration 0x%04X", register_pair
            );

   //set port 0 to all outputs on 0x20
   TEST_THIS_ASSERT(bool, i2c.prepare(0x20) < 0, false);

   register_pair = 0xfff0;
   TEST_THIS_ASSERT(bool, i2c.write(I2C::Location(0x06), Reference(register_pair)) == 2, true);

   register_pair = 0;
   TEST_THIS_ASSERT(bool, i2c.read(I2C::Location(0x06), Reference(register_pair)) == 2, true);
   TEST_THIS_EXPECT(u16, register_pair, 0xfff0);

   TEST_THIS_ASSERT(bool, i2c.read(I2C::Location(0x00), Reference(register_pair)) == 2, true);
   print_case_message("Read 0x%04X", register_pair);

   //write values to the output
   register_pair = 0xfff0;
   TEST_THIS_ASSERT(bool, i2c.write(I2C::Location(0x02), Reference(register_pair)) == 2, true);

   register_pair = 0;
   TEST_THIS_ASSERT(bool, i2c.read(I2C::Location(0x02), Reference(register_pair)) == 2, true);
   TEST_THIS_ASSERT(u16, register_pair, 0xfff0);

   return case_result();
}

bool PioExtenderTest::configure_pin(hal::I2C & i2c, u8 slave_address, int flags, u16 mask){
   u16 configuration;
   i2c.prepare(slave_address);
   if( i2c.read(I2C::Location(0x06), Reference(configuration)) != 2 ){
      return false;
   }
   if( flags & Pin::SET_OUTPUT ){
      configuration &= ~mask; //0 is output
   } else {
      configuration |= mask;
   }
   if( i2c.write(I2C::Location(0x06), Reference(configuration)) != 2 ){
      return false;
   }

   return true;
}

bool PioExtenderTest::configure_setmask(hal::I2C & i2c, u8 slave_address, u16 mask){
   u16 value;
   i2c.prepare(slave_address);
   if( i2c.read(I2C::Location(0x02), Reference(value)) != 2 ){
      return false;
   }

   value |= mask; //0 is output

   if( i2c.write(I2C::Location(0x02), Reference(value)) != 2 ){
      return false;
   }

   return true;
}

bool PioExtenderTest::configure_clrmask(hal::I2C & i2c, u8 slave_address, u16 mask){
   u16 value;
   i2c.prepare(slave_address);
   if( i2c.read(I2C::Location(0x02), Reference(value)) != 2 ){
      return false;
   }

   value &= ~mask; //0 is output

   if( i2c.write(I2C::Location(0x02), Reference(value)) != 2 ){
      return false;
   }

   return true;
}

void PioExtenderTest::show_registers(hal::I2C & i2c, u8 slave_address){
   u16 register_value;
   i2c.prepare(slave_address);
   for(u32 i=0x0; i < 0x8; i+= 2){
      i2c.read(I2C::Location(i), register_value);
      print_case_message("[%d] = 0x%04X", i, register_value);
   }
   for(u32 i=0x40; i < 0x50; i+= 2){
      i2c.read(I2C::Location(i), register_value);
      print_case_message("[0x%X] = 0x%04X", i, register_value);
   }
}
