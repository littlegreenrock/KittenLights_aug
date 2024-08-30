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

commsMode_t		$CM;
displayMode_t	$DM;

const char* ssid = "Old Iron 67";
const char* password = "needsomethingbettertodo";


AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");

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
	void generateRandomSeed() ;
	void doLeds(void) ;
	void _actualDecayFunc(int, uint32_t* ) ;
	

	
Led_t onboard_led	= { LED_BUILTIN,	true  };
Button_t button		= { BTN_PIN,			true  };
Led_t	ledR				= { RED_LED,			false };
Led_t	ledG				= { GREEN_LED,		false };
Led_t	ledB				= { BLUE_LED,			false };

timeout_t Timeout;
decayColour32_t DK;
byte _decayArrayRGBW[4]{8,8,8,8};
ANIM direction{ANIM::_static};
uint32_t _baselineColour{0x00000000};		//		0xwwbbrrgg
char ERR_CODE[6]{'\0'};
const char* ERR_CODE_ptr = &ERR_CODE[0];
int _pulseInterval{3000};
int globalStep{0};


/*			Functions							°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸			*/

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

void init_Configuration() {
	$CM.set(COM::WiFi);
	$CM.set(COM::STA);
	$CM.set(COM::mDNS);
	$DM.set(DISP::decay);
	$DM.set(ANIM::_lateral);
}

void Init_mDNS(const char *name) {
  if ($CM.get(COM::mDNS)) {
		if (!MDNS.begin(name)) {
			Serial.println("mDNS failed to start");
		}
		else	{
			Serial.printf("(mDNS: 'http://%s.local/' )\n", name);
		}
	}
}
bool _searchSSID() {
	int retry{0};
	bool waiting{true};
	Timeout.Next(0);
	char feedback;
	while (waiting==true) {
		waiting = WiFi.waitForConnectResult() != WL_CONNECTED;
		if (Timeout.CheckPoint(500)) {
			feedback='.';
			if (retry%6==0) {		//	disconnect should we be connected. wait half second to allow disconnect.
				WiFi.disconnect(false);
				if (retry) feedback = 'r';
			}
			if (retry%6==1) {
				WiFi.begin(ssid, password);
			}
			Serial.print(feedback);
			retry++;
		}
		delay(1);
		if (retry>20) break;		//	waits upto 10 seconds to connect, retries every 3s
	}
	return waiting;
}
void Init_WiFiMode() {
	Serial.setDebugOutput(true);
	if ($CM.get(COM::WiFi, COM::STA)) {
		WiFi.mode(WIFI_STA);
		Serial.print("\nConnecting to: ");
		Serial.println(ssid);
		if (_searchSSID()) {
			WiFi.disconnect(false);
			Serial.printf("STA: Failed!\n");
		}
		else {
			Serial.print("\nWiFi connected with IP: ");
			Serial.println(WiFi.localIP());
			WiFi.printDiag(Serial);
		}
	}
	else if ($CM.get(COM::WiFi, COM::AP)) {
		WiFi.softAP(ssid_AP, password_AP);
		WiFi.softAPConfig(local_ip, gateway, subnet);
	}	
}
// Web server initialization		-----------------------------------------------
String processor(const String &var) {
	if (var == "STATE" )	return String("checked");
	return String("off");
}
void onRootRequest(AsyncWebServerRequest *request) {
	request->send(LittleFS, "/index.html", "text/html", false, processor);
}
void initWebServer() {
	server.on("/", onRootRequest);
	server.serveStatic("/", LittleFS, "/");
	server.begin();
}


// WebSocket initialization		-------------------------------------------------
void notifyClients(int ClientID=0) {
		JsonDocument jsonDoc;
		jsonDoc["mode"]	= "dec";
		char buffer[20];
		size_t docLen = serializeJson(jsonDoc, buffer);
		ClientID==0?ws.textAll(buffer, docLen):ws.text(ClientID, buffer, docLen);
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
	AwsFrameInfo* info = (AwsFrameInfo*)arg;
	if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
		JsonDocument json;
		DeserializationError err = deserializeJson(json, data);
		if (err) {
			Serial.print(F("deserializeJson() failed with code "));
			Serial.println(err.c_str());		//	a string representation of the error.
			return;
		}
		return;			//	unknown command recieved
		notifyClients();
	}
}
void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type,
			 void* arg, uint8_t* data, size_t len) {
	switch (type) {
		case WS_EVT_CONNECT:
			Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
			break;
		case WS_EVT_DISCONNECT:
			Serial.printf("WebSocket client #%u disconnected\n", client->id());
			break;
		case WS_EVT_DATA:
			Serial.printf("WS data rcvd: client #%u D:%i(%u)\n", client->id(), *data, len);
			handleWebSocketMessage(arg, data, len);
			break;
		case WS_EVT_PONG:
			Serial.printf("WebSocket client #%u PONG(?)D:%u(%u)\n", client->id(), *data, len);
		case WS_EVT_ERROR:
			Serial.printf("WebSocket client #%u ERROR(?)D:%u(%u)\n", client->id(), *data, len);
			break;
	}
}
void initWebSocket() {
		ws.onEvent(onEvent);
		server.addHandler(&ws);
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




void eff_Shift(int places, int startAt, int endAt) {
	int _head, _tail, pp;
	bool shifdn;
	//	begin safety inspector
	if (startAt<0	||	startAt>Strip.numPixels())			startAt	= Strip.numPixels();
	if (endAt<0		||	endAt >Strip.numPixels())				endAt		= 0;
	if (places==0	||	abs(places)>abs(startAt-endAt))	places	= 1;
	//	begin direction finding
	shifdn	=	(startAt>endAt);
	_tail		=	(shifdn?min(startAt, endAt):max(startAt, endAt));
	_head		=	(shifdn?max(startAt, endAt):min(startAt, endAt));
	shifdn?places=abs(places):places=-abs(places);
	pp			=	_tail+places;
	//	begin shift all nominated array values by places 
	while (shifdn?pp<=_head:pp>=_head) {
		Strip.setPixelColor(pp-places, Strip.getPixelColor(pp));
		shifdn?pp++:pp--;
	}
}

void eff_Radiate() {
	// copy the centre LED value gradually to the head/tail ends
	int midway = Strip.numPixels()/2;
	eff_Shift(1, midway, 0);
	eff_Shift(1, midway, Strip.numPixels());
	_actualDecayFunc(midway, &_baselineColour);
}


void _actualDecayFunc(int LED, uint32_t* baselineToUse) {
	Strip.setPixelColor(LED, DK.Decay(Strip.getPixelColor(LED), *baselineToUse));
}
bool baselineDecay(bool Enable) {
	/*	This function updates the led strip, once per animation frame or fps times
	 	per second.  While other functions may alter the led configuration, 
		the result is only sent once, here and now. However after the led update, this function
		will call upon the decay routine, which *does* change the led array data. So, first 
		we update the LED strip. Second, we implement a decay routine on the array.
	*/
	//	check for fps tick
	if (DK.Tick()==false) return false;
	else {
		//	this is the *only* time strip.show() is called.
		Strip.show();
		//	checkpoints: configuration, & decay rate is not zero.
		if (Enable) {	//	begin loop entire array at once. 
			int LED_bytes_Counter = 0;
			while (LED_bytes_Counter < Strip.numPixels()) {
				_actualDecayFunc(LED_bytes_Counter, &_baselineColour);
				LED_bytes_Counter++;
			}		//	END while loop 
		}			//	END if(Enable)
	}
	return true;
}

void HellandSpread(const int LEDCOUNT, int maxKelvin) {
	const int minKelvin = 1000;
	K_colour Helland;
	maxKelvin = constrain(maxKelvin,6500,40000);
	int K_step = (maxKelvin-minKelvin)/(LEDCOUNT);
	int _LedNo{0};
	for (int Kelvin = 1000; Kelvin <= maxKelvin; Kelvin+=K_step) {
		Strip.setPixelColor(_LedNo++, Helland.Calculate(Kelvin));
		// serial feedback
		if (_LedNo==1) Serial.printf("from %5i K  -", Kelvin);
		if (_LedNo==LEDCOUNT) Serial.printf("to-  %5i K\n", Kelvin);
		if (_LedNo>LEDCOUNT) break;		//	 you have run out of LEDs to illuminate
		Strip.show();
	}
}

void doHS(int pauseInterval) {
	uint32_t _pausetime;
	for (int Tmax = 19000; Tmax > 9500; Tmax -= 1000){
			_pausetime = millis() + pauseInterval;
			HellandSpread(LED_COUNT, Tmax);
				while (millis() < _pausetime){
					// visual indication that the loop is still running
					Strip.setPixelColor(0, uint32_t(_pausetime-millis()));
					Strip.show();
					delay(1);
				}
	}
	delay(pauseInterval);
}

void NEO_memory_alignment_test(byte no_LEDs_per_colour) {
	/* if you see blue then green then red then white LEDs
	in THAT exact order, then your NEO_GRBW-esq configuration is 
	correct.  Otherwise it is wrong. */
	uint32_t bgrw {0x000000ff};	//	must not change this value
	for (int i=0; i<(no_LEDs_per_colour*5);i++) {
		if (i>0 && i%no_LEDs_per_colour==0) bgrw<<=8;
		Strip.setPixelColor(i,bgrw);
	}
	Strip.show();
	delay(5000);
}

bool buttonHeldDuringPowerOn(int t) {
	//	checks if button is held at powerOn for half+ timeout duration
	// 	f *will* take t ms until return control
	uint32_t Delay = millis()+t;
	while (millis()<Delay)	button.read();
	return (button.held(t/2));
}

void fly () {
	sigmoid_fly spider;
	spider.build(-75, 75, 0 );
	Serial.println("\nFly: -75 to 75, normal curve");
	for (int i=0; i< 64; i++) {
		Serial.printf("%2i - %2i\n", i, spider.get(i));
	}
	
	Serial.println("\nFly: 0 to 75, softer curve");
	spider.build(0, 75, 1 );
	for (int i=0; i< 64; i++) {
		Serial.printf("%2i - %2i\n", i, spider.get(i));
	}
}


void setup() {
	Serial.begin(115200); delay(250);
	Serial.setDebugOutput(true);
	pinMode(onboard_led.pin,	OUTPUT);
	pinMode(button.pin, 			INPUT );
	pinMode(ledR.pin, 				OUTPUT);
	pinMode(ledG.pin, 				OUTPUT);
	pinMode(ledB.pin, 				OUTPUT);
	Strip.begin();						//	INITIALIZE NeoPixel object
	Strip.clear();						//	clear led array
	
	init_LittleFS();
	init_Configuration();
	Init_WiFiMode();
	Init_mDNS("striplight");
	if (buttonHeldDuringPowerOn(250)) {
		dspl("\nDetected button held during PowerOn.");
		// doHS(1000);
		}
	else {
		dspl("\nBegin");
	 }
	initWebServer();			delay(100);
	initWebSocket();			delay(100);
	DK.Begin(42, 800);
	fly();

}		//		- = - = - = - = - E N D   s e t u p ( ) - = - = - = - = - 


void loop() {
	//	housekeeping
	// ArduinoOTA.handle();
	ws.cleanupClients();
	//	END housekeeping

	uint32_t SadColour;
	if (Timeout.CheckPoint(5000))	{
		SadColour = (random(0x00,0x02) << 24) + (random(0x00,0xff) << 16) + (random(0x00,0xff) << 8) + random(0x00,0xff);
		//Serial.printf("%#010x\n", SadColour);
		for (int l=0; l<150; l++) {
			Strip.setPixelColor(l, SadColour);
		}
	}
	if (baselineDecay(button.held(100))) doLeds();
	//yield();		//	specific to esp32 and 8266 to have time to do its own utility overheads (ie: wifi)
	button.read();

	
}		//		- = - = - = - = - E N D   l o o p ( ) - = - = - = - = - 


