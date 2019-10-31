#ifndef TOUCHTEST_HPP
#define TOUCHTEST_HPP


#include "Test.hpp"

class TouchTest : public Test {
public:
	TouchTest();

	bool execute_class_api_case();


private:

	enum {
		ft_device_mode = 0x00,
		ft_reg_num_finger = 0x02,
		ft_tp1_reg = 0x03,
		ft_tp2_reg = 0x09,
		ft_id_g_lib_version = 0xa1,
		ft_id_g_mode = 0xa4,
		ft_id_g_thgroup = 0x80,
		ft_id_g_periodactive = 0x88,
		id_g_spec_gesture_enable = 0xd0
	};

	typedef struct MCU_PACK {
		u8 x_high;
		u8 x_low;
		u8 y_high;
		u8 y_low;
		u8 w;
		u8 m;
	} touch_t;

	bool is_press(const touch_t & touch){
		return (touch.x_high & 0xC0) == 0x00;
	}

	bool is_release(const touch_t & touch){
		return (touch.x_high & 0xC0) == 0x40;
	}

	bool is_contact(const touch_t & touch){
		return (touch.x_high & 0xC0) == 0x80;
	}

	bool touch_id(const touch_t & touch){
		return touch.y_high & 0xE0;
	}

	u16 x(const touch_t & touch){
		return ((touch.x_high << 8)|touch.x_low) & 0x0fff;
	}

	u16 y(const touch_t & touch){
		return ((touch.y_high << 8)|touch.y_low) & 0x0fff;

	}

	typedef struct MCU_PACK {
		u8 count;
		touch_t point[2];
	} touch_packet_t;



};

#endif // TOUCHTEST_HPP
