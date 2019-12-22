#include "WifiSpiTest.hpp"


#include <sapi/hal.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>

#define WIFI_PHY_IDENT_CHAR 'p'
#define I_WIFIPHY_GETVERSION _IOCTL(WIFI_PHY_IDENT_CHAR, I_MCU_GETVERSION)
#define I_WIFIPHY_GETINFO _IOCTLR(WIFI_PHY_IDENT_CHAR, I_MCU_GETINFO, u32)
#define I_WIFIPHY_SETATTR _IOCTLW(WIFI_PHY_IDENT_CHAR, I_MCU_SETATTR, u32)
#define I_WIFIPHY_SETACTION _IOCTLW(WIFI_PHY_IDENT_CHAR, I_MCU_SETACTION, mcu_action_t)
#define I_WIFIPHY_ASSERT_RESET _IOCTL(WIFI_PHY_IDENT_CHAR, I_MCU_TOTAL+1)
#define I_WIFIPHY_DEASSERT_RESET _IOCTL(WIFI_PHY_IDENT_CHAR, I_MCU_TOTAL+2)
#define I_WIFIPHY_SET_FULL_DUPLEX _IOCTL(WIFI_PHY_IDENT_CHAR, I_MCU_TOTAL+3)
#define I_WIFIPHY_SET_HALF_DUPLEX _IOCTL(WIFI_PHY_IDENT_CHAR, I_MCU_TOTAL+4)

WifiSpiTest::WifiSpiTest() : Test("WifiSpi"){

}


bool WifiSpiTest::execute_class_api_case(){


	Device wifi_phy;


	if( wifi_phy.open(
			 "/dev/wifi_phy",
			 fs::OpenFlags::read_write()
			 ) < 0 ){
		print_case_failed(wifi_phy.result(), __LINE__);
		return case_result();
	}


	wifi_phy.ioctl(
				Device::IoRequest(I_WIFIPHY_SET_HALF_DUPLEX)
				);

#if 1
	wifi_phy.ioctl(
				Device::IoRequest(I_WIFIPHY_ASSERT_RESET)
				);
	wait(Milliseconds(20));
	wifi_phy.ioctl(
				Device::IoRequest(I_WIFIPHY_DEASSERT_RESET)
				);
	wait(Milliseconds(100));
#endif


	Data data(5);

	data.at_u8(0) = 0xCF;
	data.at_u8(1) = 0xFF;
	data.at_u8(2) = 0xFF;
	data.at_u8(3) = 0xFF;
	data.at_u8(4) = 0xAA;

#if 0
	data.at_u8(0) = 0xCA;
	data.at_u8(1) = 0x00;
	data.at_u8(2) = 0xE8;
	data.at_u8(3) = 0x24;
	data.at_u8(4) = 0xBC;
#endif

	print_case_message("Writing SPI Bus");

	if( wifi_phy.write(data) != (int)data.size() ){
		print_case_failed(wifi_phy.result(), __LINE__);
		return case_result();
	}

	data.resize(1);

	//while(1){
		for(u32 i=0; i	< 10; i++){
			wait(Milliseconds(50));
			//print_case_message("Reading SPI Bus");
			if( wifi_phy.read(data) != (int)data.size() ){
				print_case_failed(wifi_phy.result(), __LINE__);
				return case_result();
			}
			wait(Milliseconds(3));


			print_case_message("Read 0x%X", data.at_const_u8(0));
		}
	//}

	return case_result();
}
