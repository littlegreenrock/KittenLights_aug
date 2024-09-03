#ifndef __androidRemoteXY_H__
#define __androidRemoteXY_H__

/*	you can enable debug logging to Serial at 115200	
 */
// #define REMOTEXY__DEBUGLOG
// #define REMOTEXY_ACCESS_PASSWORD "PUMBAH"
#ifndef REMOTEXY_ACCESS_PASSWORD 
#define REMOTEXY_ACCESS_PASSWORD ""
#endif 


/*
		The connection mode, connection settings, and includes may need to go in main.cpp
		because from time to time I want to switch it up between STA/Client Mode and AP/Access Point mode.
		Since those modes require different configurations to be present before calling RemoteXY,
		I will need to find a clever way to implement them as variable. 
 */

	//	----	CLIENT MODE	----
		#define REMOTEXY_MODE__WIFI								//			<== Client Mode	-(*)-
		#define REMOTEXY_WIFI_SSID _STA_SSID
		#define REMOTEXY_WIFI_PASSWORD _STA_PWORD
		#define REMOTEXY_SERVER_PORT 6377

	//	----	ACCESS POINT MODE	----
		// #define REMOTEXY_MODE__WIFI_POINT					//			<== AP Mode	-(*)-
		// #include <ESP8266WiFi.h>										//			<== AP Mode	-(*)-
		#define REMOTEXY_AP_SSID _AP_SSID					//		<-- library edit
		#define REMOTEXY_AP_PASSWORD _AP_PWORD		//		<-- library edit

/*		-		-		-		-		-		-		-		-		-		-		-		-		-		-		-		-		-		-		-	
					END connection mode and settings defs
			-		-		-		-		-		-		-		-		-		-		-		-		-		-		-		-		-		-		-	
 */

//	this include must before menu definitions and AFTER connection mode defs
#include <RemoteXY.h>


//  Define Menu ALPHA		-------------------------------------------------------
#ifdef XY_ALPHA
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 91 bytes
	{ 255,15,0,0,0,84,0,17,0,0,0,185,1,106,200,1,1,5,0,11,
	65,159,39,39,4,32,129,1,2,106,13,79,84,104,101,32,75,105,116,116,
	101,110,32,76,105,103,104,116,115,0,10,69,133,29,16,113,4,26,31,76,
	105,118,101,0,28,1,31,158,26,9,1,69,33,67,97,108,105,98,114,97,
	116,101,0,3,30,19,44,16,131,2,26 };		
// Variable and event defs for the control interface.  ORDER is IMPORTANT!!
struct {
	float ort_pitch; 						//	from -PI to PI
	float ort_roll;							//	from -PI to PI
	float ort_yaw;							//	from -PI to PI
	uint8_t pg_Live; 					//	1 state is ON, else 0
	uint8_t btn_Calibrate;			//	1 if pressed, else 0
	uint8_t sw_ModeSelect 					//	from 0 to 3
	uint8_t connect_flag;		//	1 if wire connected, else 0
} RemoteXY;   
#pragma pack(pop)
#endif	//	XY_ALPHA


//  Define Menu BETA		-------------------------------------------------------
#ifdef	XY_BigDreams

#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 958 bytes
  { 255,34,0,107,0,183,3,18,0,0,0,238,1,106,200,3,7,2,2,8,
  0,10,72,25,24,24,49,92,89,90,79,78,0,100,112,111,119,101,114,0,
  1,7,26,24,24,0,183,177,226,156,168,0,3,8,65,24,44,2,77,74,
  67,0,54,40,10,1,229,11,129,19,3,73,12,192,228,75,105,116,116,101,
  110,32,76,105,103,104,116,115,0,131,30,181,42,11,2,235,31,229,240,159,
  142,168,226,154,153,239,184,143,0,38,131,38,143,26,19,1,131,2,134,108,
  105,118,101,0,26,12,51,63,50,11,194,228,237,46,203,179,239,189,165,239,
  189,161,46,226,139,134,203,154,240,159,140,159,0,226,150,170,239,184,143,239,
  184,143,194,183,32,194,183,32,226,150,170,32,226,150,170,226,156,166,0,226,
  139,134,32,203,154,46,32,203,179,32,226,156,167,32,203,154,46,0,226,139,
  133,46,203,179,203,179,46,226,139,133,226,129,186,226,139,133,46,203,179,203,
  179,46,226,139,133,0,20,0,130,0,131,107,39,25,239,129,27,157,55,8,
  0,228,99,104,46,32,100,101,99,97,121,32,114,97,116,101,0,72,12,19,
  81,81,140,138,0,25,241,0,0,0,0,0,0,127,67,0,0,0,0,26,
  0,0,76,66,0,0,204,66,28,0,0,204,66,0,0,25,67,30,0,0,
  25,67,0,0,76,67,31,0,0,76,67,0,0,127,67,6,15,24,74,74,
  250,24,72,12,134,24,24,137,166,22,50,24,0,0,0,0,0,0,200,65,
  0,0,0,0,49,0,0,160,64,0,0,32,65,48,0,0,32,65,0,0,
  112,65,47,0,0,112,65,0,0,160,65,46,0,0,160,65,0,0,200,65,
  72,35,134,24,24,137,166,22,120,24,0,0,0,0,0,0,200,65,0,0,
  0,0,119,0,0,160,64,0,0,32,65,118,0,0,32,65,0,0,112,65,
  117,0,0,112,65,0,0,160,65,116,0,0,160,65,0,0,200,65,72,58,
  134,24,24,137,166,22,176,24,0,0,0,0,0,0,200,65,0,0,0,0,
  175,0,0,160,64,0,0,32,65,174,0,0,32,65,0,0,112,65,173,0,
  0,112,65,0,0,160,65,172,0,0,160,65,0,0,200,65,72,81,134,24,
  24,137,166,22,30,24,0,0,0,0,0,0,200,65,0,0,0,0,29,0,
  0,160,64,0,0,32,65,28,0,0,32,65,0,0,112,65,27,0,0,112,
  65,0,0,160,65,26,0,0,160,65,0,0,200,65,131,30,181,42,11,10,
  228,2,31,32,240,159,143,160,239,184,142,32,32,226,164,180,0,41,1,76,
  92,14,12,1,241,31,240,159,165,130,0,4,4,19,8,82,0,83,240,1,
  10,123,13,10,1,241,31,43,0,1,33,123,13,10,1,241,31,43,0,1,
  56,123,13,10,1,241,31,43,0,1,79,123,13,10,1,241,31,43,0,1,
  10,166,13,10,1,241,31,45,0,1,33,166,13,10,1,241,31,45,0,1,
  56,166,13,10,9,241,31,45,0,1,79,166,13,10,1,241,31,45,0,73,
  93,19,8,82,140,128,0,89,240,0,0,0,0,0,0,127,67,0,0,0,
  0,90,0,0,76,66,0,0,204,66,91,0,0,204,66,0,0,25,67,95,
  0,0,25,67,0,0,76,67,97,0,0,76,67,0,0,127,67,29,0,11,
  19,8,65,65,6,239,65,5,110,4,4,113,65,9,110,4,4,113,65,13,
  110,4,4,113,65,17,110,4,4,113,65,21,110,4,4,113,65,25,110,4,
  4,113,65,29,110,4,4,113,65,33,110,4,4,113,65,37,110,4,4,113,
  65,41,110,4,4,113,65,45,110,4,4,113,65,49,110,4,4,113,65,53,
  110,4,4,113,65,57,110,4,4,113,65,61,110,4,4,113,65,65,110,4,
  4,113,65,69,110,4,4,113,65,73,110,4,4,113,65,77,110,4,4,113,
  65,81,110,4,4,113,65,85,110,4,4,113,65,89,110,4,4,113,65,93,
  110,4,4,113,65,97,110,4,4,113,67,24,92,60,10,69,232,237,18,1,
  39,141,24,24,0,106,26,67,97,108,105,98,114,97,116,101,0,131,30,181,
  42,11,10,228,2,31,32,240,159,143,160,239,184,142,32,32,226,164,180,0,
  41,2,28,119,51,14,1,117,25,120,116,79,110,0,79,102,102,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t pg_Home; // =1 if page is visible, else =0
  uint8_t pg_ColourWheel; // =1 if page is visible, else =0
  uint8_t pg_Live; // =1 if page is visible, else =0
  uint8_t sw_Power; // =1 if state is ON, else =0
  uint8_t btn_SparkleEffect; // =1 if button pressed, else =0
  uint8_t sw_fadeSelect; // from 0 to 2
  uint8_t opt_displaySelect; // from 0 to 4
  uint8_t cwh_r; // =0..255 Red color value
  uint8_t cwh_g; // =0..255 Green color value
  uint8_t cwh_b; // =0..255 Blue color value
  uint8_t btn_sendColourWheel; // =1 if button pressed, else =0
  int8_t cwh_w; // from 0 to 100
  uint8_t btn_dR_up; // =1 if button pressed, else =0
  uint8_t btn_dG_up; // =1 if button pressed, else =0
  uint8_t btn_dB_up; // =1 if button pressed, else =0
  uint8_t btn_dW_up; // =1 if button pressed, else =0
  uint8_t btn_dR_dn; // =1 if button pressed, else =0
  uint8_t btn_dG_dn; // =1 if button pressed, else =0
  uint8_t btn_dB_dn; // =1 if button pressed, else =0
  uint8_t btn_dW_dn; // =1 if button pressed, else =0
  float ort_pitch; // from -PI to PI
  float ort_roll; // from -PI to PI
  float ort_yaw; // from -PI to PI
  uint8_t btn_Calibrate; // =1 if button pressed, else =0
  uint8_t sw_followMe; // =1 if switch ON and =0 if OFF

    // output variables
  char txt_fadeSelect[11]; // string UTF8 end zero
  int8_t cbar_white; // from 0 to 255
  int8_t arc_dR; // from 0 to 25
  int8_t arc_dG; // from 0 to 25
  int8_t arc_dB; // from 0 to 25
  int8_t arc_dW; // from 0 to 25
  int8_t bar_cwh_w; // from 0 to 255
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
  char txt_Live[18]; // string UTF8 end zero

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)

#endif		//		XY_BigDreams

#endif				//		__androidRemoteXY_H__



/*

CRemoteXY *remotexy;

void RemoteXY_Init(bool AccessPoint) {
	if (AccessPoint) {
		remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, new CRemoteXYConnectionServer \
		(new CRemoteXYComm_WiFiPoint (REMOTEXY_WIFI_SSID, REMOTEXY_WIFI_PASSWORD), \
		REMOTEXY_SERVER_PORT), REMOTEXY_ACCESS_PASSWORD);
	}
	else {
		remotexy = new CRemoteXY(RemoteXY_CONF_PROGMEM, &RemoteXY, new CRemoteXYConnectionServer \
		(new CRemoteXYComm_WiFi (REMOTEXY_WIFI_SSID, REMOTEXY_WIFI_PASSWORD), \
		REMOTEXY_SERVER_PORT), REMOTEXY_ACCESS_PASSWORD); 
	}
}


*/