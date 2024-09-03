// first working test app menu.

	// you can enable debug logging to Serial at 115200
	// #define REMOTEXY__DEBUGLOG
	// #define REMOTEXY_ACCESS_PASSWORD "PUMBAH"
	#define REMOTEXY_MODE__WIFI		//	RemoteXY select connection mode
	// RemoteXY WIFI connection settings 
	#define REMOTEXY_WIFI_SSID "Old Iron 67"
	#define REMOTEXY_WIFI_PASSWORD "needsomethingbettertodo"
	#define REMOTEXY_SERVER_PORT 6377
	#include <RemoteXY.h>
	/* 	RemoteXY GUI Configuration Settings ------------------------------------ */
	#pragma pack(push, 1)  
		uint8_t RemoteXY_CONF[] =   // 91 bytes
			{ 255,15,0,0,0,84,0,17,0,0,0,185,1,106,200,1,1,5,0,11,
			65,159,39,39,4,32,129,1,2,106,13,79,84,104,101,32,75,105,116,116,
			101,110,32,76,105,103,104,116,115,0,10,69,133,29,16,113,4,26,31,76,
			105,118,101,0,28,1,31,158,26,9,1,69,33,67,97,108,105,98,114,97,
			116,101,0,3,30,19,44,16,131,2,26 };		
		// Variable and event defs for the control interface.  ORDER is IMPORTANT!!
			struct {
				float pitch; 						//	from -PI to PI
				float roll;							//	from -PI to PI
				float yaw;							//	from -PI to PI
				uint8_t Live; 					//	1 state is ON, else 0
				uint8_t Calibrate;			//	1 if pressed, else 0
				uint8_t Mode; 					//	from 0 to 3
				// int8_t Intensity;				//	{0..100}
				// uint8_t rgb_r; 					//	{0..255} Red color value
				// uint8_t rgb_g; 					//	{0..255} Green color value
				// uint8_t rgb_b; 					//	{0..255} Blue color value
				uint8_t connect_flag;		//	1 if wire connected, else 0
			} RemoteXY;   
	#pragma pack(pop)


	/*
	Dream Big
	
	uint8_t RemoteXY_CONF[] =   // 687 bytes
  { 255,22,0,110,0,168,2,17,0,0,0,88,1,106,200,3,1,6,6,9,
  0,66,84,68,15,125,1,19,26,4,6,65,13,127,32,8,26,10,72,25,
  24,24,48,90,89,92,79,78,0,88,112,111,119,101,114,0,1,7,26,24,
  24,0,174,177,226,156,168,0,3,39,55,24,44,2,2,26,67,31,46,40,
  10,1,2,26,11,129,19,3,73,12,17,75,105,116,116,101,110,32,76,105,
  103,104,116,115,0,131,32,179,42,11,10,236,31,242,99,111,108,111,117,114,
  32,115,101,116,116,105,110,103,115,0,38,131,38,122,26,19,9,130,2,134,
  108,105,118,101,0,26,9,0,130,0,122,107,39,1,28,129,27,151,55,9,
  26,100,101,99,97,121,32,97,100,106,117,115,116,0,72,255,1,106,106,140,
  25,24,0,17,0,0,0,0,0,0,127,67,0,0,0,0,26,0,0,76,
  66,0,0,204,66,28,0,0,204,66,0,0,25,67,30,0,0,25,67,0,
  0,76,67,31,0,0,76,67,0,0,127,67,6,15,24,74,74,250,24,72,
  12,125,24,24,141,50,24,22,38,0,0,0,0,0,0,127,67,0,0,0,
  0,49,0,0,92,66,0,0,210,66,48,0,0,210,66,0,0,27,67,47,
  0,0,27,67,0,0,77,67,46,0,0,77,67,0,0,127,67,72,35,125,
  24,24,141,120,24,22,38,0,0,0,0,0,0,127,67,0,0,0,0,119,
  0,0,92,66,0,0,210,66,118,0,0,210,66,0,0,27,67,117,0,0,
  27,67,0,0,77,67,116,0,0,77,67,0,0,127,67,72,58,125,24,24,
  141,176,24,22,38,0,0,0,0,0,0,127,67,0,0,0,0,175,0,0,
  92,66,0,0,210,66,174,0,0,210,66,0,0,27,67,173,0,0,27,67,
  0,0,77,67,172,0,0,77,67,0,0,127,67,72,81,125,24,24,141,30,
  24,22,38,0,0,0,0,0,0,127,67,0,0,0,0,29,0,0,92,66,
  0,0,210,66,28,0,0,210,66,0,0,27,67,27,0,0,27,67,0,0,
  77,67,26,0,0,77,67,0,0,127,67,131,30,181,42,11,10,116,2,31,
  72,111,109,101,0,41,29,0,11,19,8,65,65,6,101,65,4,110,4,4,
  117,65,8,110,4,4,117,65,12,110,4,4,117,65,16,110,4,4,117,65,
  20,110,4,4,117,65,24,110,4,4,117,65,28,110,4,4,117,65,32,110,
  4,4,117,65,36,110,4,4,117,65,40,110,4,4,117,65,44,110,4,4,
  117,65,48,110,4,4,117,65,52,110,4,4,117,65,56,110,4,4,117,65,
  60,110,4,4,117,65,64,110,4,4,117,65,68,110,4,4,117,65,72,110,
  4,4,117,65,76,110,4,4,117,65,80,110,4,4,117,65,84,110,4,4,
  117,65,88,110,4,4,117,65,92,110,4,4,117,65,96,110,4,4,117,65,
  100,110,4,4,117,67,24,96,60,10,5,107,26,18,1,41,138,24,24,0,
  106,26,67,97,108,105,98,114,97,116,101,0,131,32,183,42,11,10,116,2,
  31,72,111,109,101,0,41 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t page_1; // =1 if page is visible, else =0
  uint8_t page_2; // =1 if page is visible, else =0
  int8_t slider_01; // from -100 to 100
  uint8_t btn_start; // =1 if state is ON, else =0
  uint8_t btn_spkl; // =1 if button pressed, else =0
  uint8_t Mode; // from 0 to 2
  uint8_t rgb_r; // =0..255 Red color value
  uint8_t rgb_g; // =0..255 Green color value
  uint8_t rgb_b; // =0..255 Blue color value
  float pitch; // from -PI to PI
  float roll; // from -PI to PI
  float yaw; // from -PI to PI
  uint8_t btn_Calib; // =1 if button pressed, else =0

    // output variables
  int8_t level_01; // from 0 to 100
  char text_Mode[11]; // string UTF8 end zero
  int8_t circularbar_01; // from 0 to 255
  int8_t cdR; // from 0 to 255
  int8_t cdG; // from 0 to 255
  int8_t cdB; // from 0 to 255
  int8_t cdW; // from 0 to 255
  uint8_t led_01_r; // =0..255 LED Red brightness
  uint8_t led_01_g; // =0..255 LED Green brightness
  uint8_t led_01_b; // =0..255 LED Green brightness
  uint8_t led_02_r; // =0..255 LED Red brightness
  uint8_t led_02_g; // =0..255 LED Green brightness
  uint8_t led_02_b; // =0..255 LED Green brightness
  uint8_t led_03_r; // =0..255 LED Red brightness
  uint8_t led_03_g; // =0..255 LED Green brightness
  uint8_t led_03_b; // =0..255 LED Green brightness
  uint8_t led_04_r; // =0..255 LED Red brightness
  uint8_t led_04_g; // =0..255 LED Green brightness
  uint8_t led_04_b; // =0..255 LED Green brightness
  uint8_t led_05_r; // =0..255 LED Red brightness
  uint8_t led_05_g; // =0..255 LED Green brightness
  uint8_t led_05_b; // =0..255 LED Green brightness
  uint8_t led_06_r; // =0..255 LED Red brightness
  uint8_t led_06_g; // =0..255 LED Green brightness
  uint8_t led_06_b; // =0..255 LED Green brightness
  uint8_t led_07_r; // =0..255 LED Red brightness
  uint8_t led_07_g; // =0..255 LED Green brightness
  uint8_t led_07_b; // =0..255 LED Green brightness
  uint8_t led_08_r; // =0..255 LED Red brightness
  uint8_t led_08_g; // =0..255 LED Green brightness
  uint8_t led_08_b; // =0..255 LED Green brightness
  uint8_t led_09_r; // =0..255 LED Red brightness
  uint8_t led_09_g; // =0..255 LED Green brightness
  uint8_t led_09_b; // =0..255 LED Green brightness
  uint8_t led_10_r; // =0..255 LED Red brightness
  uint8_t led_10_g; // =0..255 LED Green brightness
  uint8_t led_10_b; // =0..255 LED Green brightness
  uint8_t led_11_r; // =0..255 LED Red brightness
  uint8_t led_11_g; // =0..255 LED Green brightness
  uint8_t led_11_b; // =0..255 LED Green brightness
  uint8_t led_12_r; // =0..255 LED Red brightness
  uint8_t led_12_g; // =0..255 LED Green brightness
  uint8_t led_12_b; // =0..255 LED Green brightness
  uint8_t led_13_r; // =0..255 LED Red brightness
  uint8_t led_13_g; // =0..255 LED Green brightness
  uint8_t led_13_b; // =0..255 LED Green brightness
  uint8_t led_14_r; // =0..255 LED Red brightness
  uint8_t led_14_g; // =0..255 LED Green brightness
  uint8_t led_14_b; // =0..255 LED Green brightness
  uint8_t led_15_r; // =0..255 LED Red brightness
  uint8_t led_15_g; // =0..255 LED Green brightness
  uint8_t led_15_b; // =0..255 LED Green brightness
  uint8_t led_16_r; // =0..255 LED Red brightness
  uint8_t led_16_g; // =0..255 LED Green brightness
  uint8_t led_16_b; // =0..255 LED Green brightness
  uint8_t led_17_r; // =0..255 LED Red brightness
  uint8_t led_17_g; // =0..255 LED Green brightness
  uint8_t led_17_b; // =0..255 LED Green brightness
  uint8_t led_18_r; // =0..255 LED Red brightness
  uint8_t led_18_g; // =0..255 LED Green brightness
  uint8_t led_18_b; // =0..255 LED Green brightness
  uint8_t led_19_r; // =0..255 LED Red brightness
  uint8_t led_19_g; // =0..255 LED Green brightness
  uint8_t led_19_b; // =0..255 LED Green brightness
  uint8_t led_20_r; // =0..255 LED Red brightness
  uint8_t led_20_g; // =0..255 LED Green brightness
  uint8_t led_20_b; // =0..255 LED Green brightness
  uint8_t led_21_r; // =0..255 LED Red brightness
  uint8_t led_21_g; // =0..255 LED Green brightness
  uint8_t led_21_b; // =0..255 LED Green brightness
  uint8_t led_22_r; // =0..255 LED Red brightness
  uint8_t led_22_g; // =0..255 LED Green brightness
  uint8_t led_22_b; // =0..255 LED Green brightness
  uint8_t led_23_r; // =0..255 LED Red brightness
  uint8_t led_23_g; // =0..255 LED Green brightness
  uint8_t led_23_b; // =0..255 LED Green brightness
  uint8_t led_24_r; // =0..255 LED Red brightness
  uint8_t led_24_g; // =0..255 LED Green brightness
  uint8_t led_24_b; // =0..255 LED Green brightness
  uint8_t led_25_r; // =0..255 LED Red brightness
  uint8_t led_25_g; // =0..255 LED Green brightness
  uint8_t led_25_b; // =0..255 LED Green brightness
  char text_Calib[18]; // string UTF8 end zero

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   

*/