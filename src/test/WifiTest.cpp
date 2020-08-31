
#include <sapi/inet.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>

#include "WifiTest.hpp"



WifiTest::WifiTest() : Test("WifiTest"){

}


bool WifiTest::execute_class_api_case(){

	Wifi wifi;

	if( wifi.initialize() < 0 ){
		print_case_failed(wifi.result(), __LINE__);
		return case_result();
	}

	Vector<WifiSsidInfo> ssid_list = wifi.scan();

	print_case_message("found %d access points", ssid_list.count());
	for(const WifiSsidInfo & info: ssid_list){
		print_case_message(
					"found %s with RSSI %d",
					info.get_name().cstring(),
					info.rssi()
					);
	}

	WifiSsidInfo & home_ap = ssid_list.match(
				WifiSsidInfo()
				.set_name("Santaslittlehelper")
				);

	if( home_ap.is_valid() == false ){
		print_case_failed("failed to find home AP");
		return case_result();
	}

	wait(Seconds(2));
	WifiIpInfo ip_info = wifi.connect(
				home_ap,
				WifiAuthInfo("anniedog")
				);

	if( ip_info.is_valid() ){
		print_case_message("connected to %s", home_ap.get_name().cstring());
		print_case_message("IP: %s", ip_info.get_ip_address().to_string().cstring());
	} else {
		print_case_failed("failed to connect to %s", home_ap.get_name().cstring());
	}


	//gethostbyname("https://www.google.com");


	return case_result();
}
