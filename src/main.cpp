#define _NAME "KittenLights"

	/*	Name:		Dinklage III
		Version: 	E
		Date: 		2024-Aug-23

	*/

#define DEBUG 1
#define ARDUINOJSON_SLOT_ID_SIZE 2					//	1: max nodes 256;  2: 65635
#define ARDUINOJSON_STRING_LENGTH_SIZE 2		//	1: max characters 255;	2: 65635
#include <Arduino.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>			// Copyright © 2014-2024, Benoit BLANCHON
// #include "fourChannelTransition.h"
// #include "morseEncoder.h"
// #include "calibrate3dOrientation.h"
#include "structs.h"
// #include "appRemote.h"

// Definition of macros		-----------------------------------------------------
#define PULLUP			true			//	true if GPIO pins are Vcc-pullup, false if gnd-pulldown.
#define RED_LED				15			//	blue 13, green 12, red 5
#define GREEN_LED			12
#define BLUE_LED			13
#define BTN_PIN				04			//	onboard flash button 04
#define HTTP_PORT			80
// #define NEO_PIN				D7			//	GPIO13
#define NEO_PIN				D7			
#define LED_COUNT		 150			//	hardware number of pixels in led strip
#define BRIGHTNESS		50			//	Set BRIGHTNESS to about
// NEO_GRBW=bbggrrww
Adafruit_NeoPixel Strip(LED_COUNT, NEO_PIN, NEO_GRBW + NEO_KHZ800);
const int LED_numBytes = LED_COUNT*4;		//	Pixel byte array length
// byte dumb[4]{1,0,2,3};		//	byte order for R G B W

const char* ssid = "Old Iron 67";
const char* password = "needsomethingbettertodo";

// AsyncWebServer server(HTTP_PORT);
// AsyncWebSocket ws("/ws");

/*	Only for AP mode, put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
const char* ssid_AP = "ESP8266";
const char* password_AP = "deeznutz";


#ifdef DEBUG 
	namespace Debug {
		// Debug Serial Print Line: dspl
		// quick insert Serial.println() statemnents all through the code for debugging purposes. 
		void dspl() {
			Serial.println();
		}
		void dspl(char noCR) {
			if (noCR != '\0') Serial.println(noCR);
		}
		template<typename TT, typename... Types>
		void dspl(TT printableType, Types... Queued) {
			if (DEBUG) {
				Serial.print(printableType);
				Serial.print(' ');
				dspl(Queued...);
			}
			else NULL;		//	compiler should automatically strip all dspl statements
		}
	};		//		END namespace Debug
	using namespace Debug;
#endif



/*		Forward Declarations		°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸			*/
	void	generateRandomSeed() ;
	void doLeds(void) ;
	

	
Led_t onboard_led	= { LED_BUILTIN,	true  };
Button_t button		= { BTN_PIN,			true  };
Led_t	ledR				= { RED_LED,			false };
Led_t	ledG				= { GREEN_LED,		false };
Led_t	ledB				= { BLUE_LED,			false };

displayMode_t Para;
timeout_t Timeout;
decayColour32_t DK;
byte _decayArrayRGBW[4]{8,8,8,8};
animationDirection direction{_static};
uint32_t _baselineColour{0x00000033};		//		0xwwbbrrgg
char ERR_CODE[6]{'\0'};
const char* ERR_CODE_ptr = &ERR_CODE[0];
int _pulseInterval{3000};
int _priorityPixel{3};
int step{0};


/*			Functions							°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸			*/
uint32_t _seekAnswers() {
	uint32_t answer{};
	for (int i = 0; i < 9; i++) {		answer += analogRead(i);		answer *= analogRead(i);	}
	return answer;
}
void generateRandomSeed() {
	randomSeed((int32_t)_seekAnswers());
	int r = random(8 * (1 + (int)_seekAnswers()));
	while (r-- > 0) (void)random(r);		//	burn rnd lot of rnd generated numbers
}

// FS initialization		-------------------------------------------------------
void init_LittleFS() {
	if (!LittleFS.begin()) {
		Serial.println("Cannot mount LittleFS volume...");
		ERR_CODE_ptr = "fs";
		while (1) {
			doLeds(); 
		}
	}
}
void doLeds(void) {
	ledR.state = 0;
	ledG.state = 0;
	ledB.state = 0;
	// onboard_led.state = !Morse.iterate(ERR_CODE_ptr);
	onboard_led.state = button.held(100);
	onboard_led.update();
	ledR.update();
	ledG.update();
	ledB.update();
};


bool baselineDecay(bool Enable) {
	/*	This function updates the led strip, once per animation frame or fps times
	 	per second.  While other functions may alter the led configuration, 
		the result is only sent once, here and now. However after the led update, this function
		will call upon the decay routine, which *does* change the led array data. So, first 
		we update the LED strip. Second, we implement a decay routine on the array.
	*/
	//	check for fps tick
	if (DK.Tik()==false) return false;
	else {
		//	this is the *only* time strip.show() is called.
		Strip.show();
		//	checkpoints: configuration, & decay rate is not zero.
		if (Enable) {
			int LED_bytes_Counter = 0;
			byte* PixBytePointer = Strip.getPixels();
			byte baselineByte[4] = {0x00, 0x00, 0xff, 0x00}; // 0123 GR_B
			
			while (LED_bytes_Counter < Strip.numPixels()) {
				if (LED_bytes_Counter>9 && LED_bytes_Counter<16)	Serial.printf("%3i :\tpCol %2x %2x %2x %2x, ", LED_bytes_Counter, PixBytePointer[0], PixBytePointer[1], PixBytePointer[2], PixBytePointer[3]);
				DK.Decay(PixBytePointer, _baselineColour);
				if (LED_bytes_Counter>9 && LED_bytes_Counter<16)	Serial.printf("%2x %2x %2x %2x\n", PixBytePointer[0], PixBytePointer[1], PixBytePointer[2], PixBytePointer[3]);
				PixBytePointer+=4;
				LED_bytes_Counter++;			//	inc loop count
			}		//	END while 
		}		//	END if
	}
	return true;
}


void setup() {
	Serial.begin(115200); delay(250);
	pinMode(onboard_led.pin, OUTPUT);
	pinMode(button.pin, INPUT);
	pinMode(ledR.pin, OUTPUT);
	pinMode(ledG.pin, OUTPUT);
	pinMode(ledB.pin, OUTPUT);
	uint32_t Delay = millis()+250;
	while (millis()<Delay)	button.read();
	//	if button held during poweron, esp enters mode ~~WiFi AP~~ RemoteXY instead of WiFi-STA
	if (!button.held(100)) {
	
	}
	else {
	
	}
	Strip.begin();						//	INITIALIZE NeoPixel object
	Strip.clear();						//	clear led array
	Strip.setPixelColor(10,0x000000ff);	
	Strip.setPixelColor(12,0x0000ff00);	
	Strip.setPixelColor(14,0x00ff0000);	
	Strip.setPixelColor(16,0xff000000);	
	Strip.show();
	delay(2000);
	DK.setFPS(40);
	Timeout.Add(2000);
	// DK.setDecayChannel(RGBW, 850);

}		//		- = - = - = - = - E N D   s e t u p ( ) - = - = - = - = - 


void loop() {
	uint32_t SadColour;
	if (Timeout.Expired())	{
		Timeout.Add(5000);
		SadColour = (random(0x00,0xff) << 16) + (random(0x00,0xff) << 8) + random(0x00,0xff);
		Serial.printf("%#010x\n", SadColour);
		for (int l=0; l<150; l++) {
			Strip.setPixelColor(l, SadColour);
		}
	}
	if (baselineDecay(button.held(100))) doLeds();
	//yield();		//	specific to esp32 and 8266 to have time to do its own utility overheads (ie: wifi)
	button.read();

	
}		//		- = - = - = - = - E N D   l o o p ( ) - = - = - = - = - 


