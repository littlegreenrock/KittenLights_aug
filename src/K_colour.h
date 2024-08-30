#ifndef constrain		// Arduino.h provided 
	#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif
#ifndef __K_COLOUR__
#define __K_COLOUR__

#include <sys/_stdint.h>
#include <Arduino.h>


const uint8_t g_Table[256] = {			//	2.2(hex)
//			0			1			2			3			4			5			6			7			8			9			a			b			c			d			e			f
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,	//	0
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,	//	1
			0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06,	//	2
			0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0C,	//	3
			0x0C, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E, 0x0F, 0x0F, 0x10, 0x10, 0x11, 0x11, 0x12, 0x12, 0x13, 0x13,	//	4
			0x14, 0x14, 0x15, 0x16, 0x16, 0x17, 0x17, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1C, 0x1C, 0x1D,	//	5
			0x1E, 0x1E, 0x1F, 0x20, 0x21, 0x21, 0x22, 0x23, 0x23, 0x24, 0x25, 0x26, 0x27, 0x27, 0x28, 0x29,	//	6
			0x2A, 0x2B, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,	//	7
			0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,	//	8
			0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x51, 0x52, 0x53, 0x54, 0x55, 0x57, 0x58, 0x59, 0x5A,	//	9
			0x5B, 0x5D, 0x5E, 0x5F, 0x61, 0x62, 0x63, 0x64, 0x66, 0x67, 0x69, 0x6A, 0x6B, 0x6D, 0x6E, 0x6F,	//	a
			0x71, 0x72, 0x74, 0x75, 0x77, 0x78, 0x79, 0x7B, 0x7C, 0x7E, 0x7F, 0x81, 0x82, 0x84, 0x85, 0x87,	//	b
			0x89, 0x8A, 0x8C, 0x8D, 0x8F, 0x91, 0x92, 0x94, 0x95, 0x97, 0x99, 0x9A, 0x9C, 0x9E, 0x9F, 0xA1,	//	c
			0xA3, 0xA5, 0xA6, 0xA8, 0xAA, 0xAC, 0xAD, 0xAF, 0xB1, 0xB3, 0xB5, 0xB6, 0xB8, 0xBA, 0xBC, 0xBE,	//	d
			0xC0, 0xC2, 0xC4, 0xC5, 0xC7, 0xC9, 0xCB, 0xCD, 0xCF, 0xD1, 0xD3, 0xD5, 0xD7, 0xD9, 0xDB, 0xDD,	//	e
			0xDF, 0xE1, 0xE3, 0xE5, 0xE7, 0xEA, 0xEC, 0xEE, 0xF0, 0xF2, 0xF4, 0xF6, 0xF8, 0xFB, 0xFD, 0xFF,	//	f
};

class K_colour {
	/* Kelvin Temperature to RGB | Credit to Tanner Helland for the base algorithm
			Port to C++ for Unreal Engine by Jorge Valle Hurtado - byValle 
			This implementation to use with Arduino-like microcontrollers and the commonly
			used NEOPIXEL LED format by littlegreenrock, 2024.  
			I made this after finding this article... 
			https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html
			... and wanting very much to reproduce this image ...
				/images/Temperature_to_RGB_1500_to_15000.png
			... in a strip of 150 NEO PIXEL style LEDs
			 */
	
	/* 	Estimate, or lookup, the colour temp of the White led component of your RGBW leds	
			EU and regions outside of the tropic lines describe home lighting as:
				2700K - 3000K	:	Warm
				3000K - 4000K	:	Neutral
				4000K - 6500K	:	Cool white
			AUS and regions inside of the tropic lines describe home lighting as:
				2600K - 3200K	:	Warm
				3200K - 4500K	:	Neutral / Cool Whiteᴬ
				4500K+       	:	Cool    / Daylight Whiteᴬ
																	ᴬ: terms as per the Lighting Council of Australia  */


public:
	int NEOPIX_WhiteLED_temp = 5000;		//	4000, 5000, 6000, 7000
	uint32_t NEOPIX_col32;			//	post-gamma values for human observation
	uint8_t _chanByte[4];				//	pre-gamma or linear brightness level
	int _W_intensity;						//	percent
	
// alternatively...if you want more float in your bloated float algo
	uint8_t gamma8Calc(const uint8_t linearColourByte, const float G_value=2.2) {
		float PercentIntensity = (float)linearColourByte / (float)255.0;
		float gammaColourByte = pow(PercentIntensity, G_value) * 255.0 + 0.5;
		return (uint8_t)gammaColourByte; 
	}

	int WhiteLEDColourCorrection(int _temp_W) {
		if      (_temp_W <= 4000) return  5;
		else if (_temp_W <= 5000) return 10;
		else if (_temp_W <= 6000) return 15;
		else if (_temp_W <= 7000) return 20;
		else return 0; 
	}
	
	uint32_t Calculate(int Temperature_K) {
		//	in Kelvin. valid: 1000 to 40000 K. (RGB 'White' light = 6500K)
		float Red_f, Green_f, Blue_f;	//	RGB components
		float White_f;								//	optional W component
		float temp_k; 								//	necessary for color byte calculations;
		temp_k = (constrain(Temperature_K,1000,40000)) / 100;
	//	RED
		if (temp_k <= 66)	Red_f = 255;
		else Red_f = 329.698727446 * pow(temp_k - 60, -0.1332047592);
		_chanByte[2] = constrain(Red_f,0,255);
	//	GREEN
			if (temp_k <= 66)	Green_f = 99.4708025861 * log(temp_k) - 161.1195681661;
			else	Green_f = 288.1221695283 * pow(temp_k - 60, -0.0755148492);
			_chanByte[1] = constrain(Green_f,0,255);
	//	BLUE
			if (temp_k >= 66)	Blue_f = 255;
			else	{
				if (temp_k <= 19)	Blue_f = 0;
				else	Blue_f = 138.5177312231 * log(temp_k - 10) - 305.0447927307;
			}
			_chanByte[0] = constrain(Blue_f,0,255);
	//	WHITE
			_W_intensity = WhiteLEDColourCorrection(NEOPIX_WhiteLED_temp);
			White_f = Red_f * (_W_intensity/100);
			_chanByte[3] = constrain(White_f,0,255);

		// FINAL RESULT AS gamma(2.2) corrected values packaged in a NEO-PIXEL 32bit
		//	colour format,	ie: 0xwwrrggbb 
			byte* neo_ptr = (byte*)&NEOPIX_col32;
			neo_ptr[3] = g_Table[_chanByte[3]];		//	white
			neo_ptr[2] = g_Table[_chanByte[2]];		//	red
			neo_ptr[1] = g_Table[_chanByte[1]];		//	green
			neo_ptr[0] = g_Table[_chanByte[0]];		//	blue

			// Serial.printf("%iK\tg.%2x r.%2x b.%2x w.%2x - NEO_PIX memory format: %#010x\n", Temperature_K, _chanByte[1], _chanByte[2], _chanByte[0], _chanByte[3], NEOPIX_col32);
	return NEOPIX_col32;
	}
};	//	END class K_colour


#endif