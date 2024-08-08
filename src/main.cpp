#define DEBUG 1
#define _NAME "KittenLights"
	/*	Name:		Dinklage II
		Version: 	A
		Date: 		2024-Jun-18

	*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>
#include "fourChannelTransition.h"
#define NEO_PIN			 D7		//	GPIO13
#define LED_COUNT		150		//	number of pixels in led strip
#define BRIGHTNESS	 50		//	Set BRIGHTNESS to about 1/5 (max = 255)

byte dumb[4]{1,0,2,3};		//	byte order for R G B W
const int LED_numBytes = LED_COUNT*4;		//	Pixel byte array length
Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRBW + NEO_KHZ800);
/* 		Argument 1 = Number of pixels in NeoPixel strip
		Argument 2 = Arduino pin number (most are valid)
		Argument 3 = Pixel type flags, add together as needed:
		NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
		NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
		NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
		NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
		NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products) */

/*	Put your SSID & Password */
const char* ssid = "Old Iron 67";		//  Enter SSID here
const char* password = "needsomethingbettertodo";	//  Enter Password here
bool mode_STA = true;
ESP8266WebServer server(80);				//		instance server @ (port)

/*	Only for AP mode, put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

uint32_t sunsetA = 0x3aFBB03B;	//	w = 23% (b=2)
uint32_t sunsetB = 0x1cED1C24;	//	w = 11% (b=7)
uint32_t sunriseA = 0x8eff8f8f;	//	w = 56%
uint32_t sunriseB = 0x99fffc99;	//	w = 60%

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

/*	Serial and parse data Comms */
const byte numChars = 32;
char rcvdChars[numChars];   // an array to store the received data
boolean newCmd = false;

/*	Globals	*/
const int longestDelay = 50;
const int shortestDelay = 2;
char LeftRightMiddle{'m'};		//	l / r / m
int _pulseInterval{3000};
byte _decayArrayRGBW[4]{5,5,5,1};
int _priorityPixel{-1};
uint32_t _baselineColour{0x1000100};		//		0xwwbbrrgg
int step{0};
uint32_t timeout{0};
enum action: uint8_t 
{
	_start=0, _mirror, decay, radiate, sparkle, 			//		toggle bit
	comet, krider, dirFwd, 															//		exclusive bit
	_stop=16,	dirBkwd																	//		unmapped functions
};
struct Mode_t {
	uint16_t bitMap16{4};		//	0010 0100 : stop, baseline-decay
	
	bool set(action Ac) {
		switch (Ac) {
			default 			: return false; break;
			case _mirror	:	_toggle(int(Ac));	break;
			case decay  	:	bitSet(bitMap16,int(Ac));	bitClear(bitMap16,int(action::radiate)); break;
			case radiate	:	bitSet(bitMap16,int(Ac));	bitClear(bitMap16,int(action::decay)); break;
			case comet  	:	_clearExclusives(int(Ac)); break;
			case krider 	:	_clearExclusives(int(Ac)); break;
			case sparkle 	:	_toggle(int(Ac));	break;
			case _start		:	bitSet(bitMap16,int(Ac));  break;
			case _stop		:	bitClear(bitMap16,int(action::_start));  break;
		}
		return true;
	}
	bool get(action Ac) {
		bool result{false};
		switch (Ac) {
			default 			:	result = false; break;
			case _stop		:	result = !bitRead(bitMap16,int(action::_start));	break;
			case _start		:	;
			case _mirror	:	;
			case decay  	:	;
			case radiate	:	;		//	intentional fall through
			case sparkle 	:	;
			case comet  	:	;
			case krider 	:	result = bitRead(bitMap16,int(Ac));	break;
		}
		return result;
	}
	bool _toggle(int B) {
		bitWrite(bitMap16, B, !(bitRead(bitMap16,B)));
		return bitRead(bitMap16,B);
	}
	void _clearExclusives(int optional=-1) {
		if (optional > 15) optional = -1;
		uint16_t exclusivesBitmap = 0b0000000000011111;		//	0's are the exclusives
		if (optional>=0) {
			bitSet(exclusivesBitmap, optional);
			bitSet(bitMap16,optional);
		}
		bitMap16 &= exclusivesBitmap;
	}
};

Mode_t Para;			//		******** this is a global scope struct ********

/*			Functions							°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸			*/
void generateRandomSeed() {
	// not trng, but an unpredicitible starting place for prng
	int32_t seed;
	for (int i = 0; i < 6; i++) {
		seed += analogRead(i);
		seed *= analogRead(i);
	}
	seed = abs(seed);
	randomSeed(seed);
	// burn a random lot of numbers from the rng seed
	int r = random(8 * (1 + analogRead(0)));
	while (r-- > 0) random(r);
}

uint8_t gamma(uint8_t ColourByte) {
	const float G = 2.2;
	float PercentIntensity = (float)ColourByte / (float)255.0;
	float Res = pow(PercentIntensity, G) * 255.0 + 0.5;
	return (uint8_t)Res;
}
// A 32-bit variant of gamma8() 
uint32_t gamma32(uint32_t x) {
  uint8_t *y = (uint8_t *)&x;
  // All four bytes of a 32-bit value are filtered even if not WRGB
  for (uint8_t i = 0; i < 4; i++)
		y[i] = gamma(y[i]);
  return x; // Packed 32-bit return
}

void setBaselineColourFromRGBW(byte rr, byte gg, byte bb, byte ww=0) {
	_baselineColour = strip.Color(bb, rr, gg, ww);		//	yes, Adafruit_NeoPixel.Color() is stupid!
}

int risingHalfSineWave(uint16_t Y_min, uint16_t Y_max, int16_t X, uint16_t X_max) {
	// T:0.11µs
	// https://www.wolframalpha.com/input?i=plot+%28%28sin+0.5x*pi%2F150%29%5E2%29+*+120%3B++between+0+and+150
	return pow(sin (0.5*X*PI/(X_max)),2) *(Y_max-Y_min) + Y_min;
}

int halfCircle(int Radius, int X, bool invert=false) {
	// T: 7µs.
	//	https://www.wolframalpha.com/input?i=75+-+sqrt%2875%5E2+-+%28x+-+75%29%5E2%29&assumption=%22ClashPrefs%22+-%3E+%7B%22Math%22%7D
	Radius = min(255,max(2,Radius));
	X = min(2*Radius,max(0,X));
	int res = sqrt(pow(Radius,2) - pow(X-Radius,2));		//	◠
	if (invert) res = Radius-res;												//	◡
	return res;
}

bool _decayTick(uint8_t newValue = 0) {
	/*	set via global value _fps 
			controls animation speed precisely without using delay()
			20 =~ 50ms,	30 =~ 33.3ms,	40 =~25ms,	60 =~ 16.6ms,	90 =~ 11ms,	120 =~ 8ms.
	*/
	static uint8_t _fps{40};
	static uint32_t _dTick{};
	if (newValue > 0) _fps = newValue;					//	set
	else if (newValue < 0) _dTick = millis();		//	reset
	else if (millis() >= _dTick) {							//	f()
		_dTick = millis() + (1000/_fps);
		return true;
	}
	return false;
}

void rainbow(uint8_t wait) {
	// simple way to test all lights are working fine
	static byte W{0};
	if (timeout < millis()) {
		timeout = millis() + wait;
		for(int i=0; i<strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel((i+W) & 255));
		}
		W++;
	}
}

void rainbowCycle(uint8_t wait) {
	// Slightly different, this makes the rainbow equally distributed throughout
  static byte W{0};
	if (timeout < millis()) {
		timeout = millis() + wait;
		for(int i=0; i< strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + W) & 255));
		}
		W++;
	}
}

void theaterChaseRainbow(uint8_t wait, bool useBaselineColourInstead=0) {
	//	Theatre-style crawling lights with rainbow effect
	static byte _cWheel{};
	static byte _Round{};
	if (timeout < millis()) {
		timeout = millis() + wait;
			for (uint16_t i=0; i < strip.numPixels(); i=i+3) {					//	every 3rd pixel
				strip.setPixelColor(i+_Round+1, useBaselineColourInstead? _baselineColour: Wheel((i+_cWheel)%255) );		//	set colour variation
				strip.setPixelColor(i+_Round, 0);												//	prev round off
			}
			_Round++;
			if (_Round > 3) {
				_Round = 0;
				_cWheel++;		//	expect byte to rollover back to 0x0. 256 colours
			}
	}
}
uint32_t Wheel(byte WheelPos) {
	// Input a value 0 to 255 to get a color value. Colours transition r - g - b - back to r.
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void Init_mDNS(const char *name) {
  if (!MDNS.begin(name)) {
    Serial.println("mDNS failed to be started");
  }
  else	{
    Serial.print("mDNS started, hostname:");
    Serial.println(name);
    Serial.printf("(try address 'http://%s.local/' from a computer browser)\n", name);
  }
}
void Init_WiFiMode(bool comingFromSetup = false) {
	// setup network.  Different for first time (from setup).  Respects STA/AP mode.
	if (comingFromSetup) {
		Init_mDNS("striplight");
	}
	else {
		WiFi.disconnect();
		delay(100);
	}
	if (mode_STA) {
		Serial.println("Connecting to ");
		Serial.println(ssid);
		WiFi.begin(ssid, password);		//connect to your local wi-fi network
		//check wi-fi is connected to wi-fi network
		while (WiFi.status() != WL_CONNECTED) {
			delay(1000);
			Serial.print(".");
		}
		Serial.println("");
		Serial.println("WiFi connected..!");
		Serial.print("Got IP: ");
		Serial.println(WiFi.localIP());
	}
	else {
		WiFi.softAP(ssid, password);
		WiFi.softAPConfig(local_ip, gateway, subnet);
	}	
	delay(100);
}
void Init_ServerCmds() {
	server.on("/",				handle_OnConnect);
	server.on("/led1on",	handle_led1on);
	server.on("/led1off",	handle_led1off);
	server.on("/led2on",	handle_led2on);
	server.on("/led2off",	handle_led2off);
	server.onNotFound		 (handle_NotFound);				//	OR: handle file system retrieve
	server.begin();																//	start server
	Serial.println("HTTP server started");
}

void handle_OnConnect() {
	Serial.println("Status: reset | ");
	server.send(200, "text/html", SendHTML(0,0)); 
}
void handle_NotFound() {
	server.send(404, "text/plain", "Not found");
}
void handle_led(uint8_t LED, bool On) {
	strip.setPixelColor(LED, On?0x00005500:0x00);		//	green
	strip.show();
	if (LED%2==0) server.send(200, "text/html", SendHTML(false, On)); 
	else server.send(200, "text/html", SendHTML(On, false)); 
}
void handle_led1on() {
	strip.setPixelColor(75,0x00005500);	//	green
	strip.show();
	Serial.println("strip Status: ON");
	server.send(200, "text/html", SendHTML(true,false)); 
	int i = 75;
	while (i>=0) {
		//Copy();
		delay(10);		
	}
}
void handle_led1off() {
	Serial.println("strip Status: OFF");
	server.send(200, "text/html", SendHTML(false,false)); 
	strip.clear();
	strip.show();
}
void handle_led2on() {
	server.send(200, "text/html", SendHTML(0,1)); 
	theaterChaseRainbow(50);
}
void handle_led2off() {
	server.send(200, "text/html", SendHTML(0,0)); 
	
}

void handleCLI() {
	int myarg{};
	char * I{};
	if (newCmd) {
		I = strchr(rcvdChars,' ');
		// myarg = atoi(strchr(rcvdChars,' '));
		myarg = atoi(I);
		if (strstr(rcvdChars, "help")) dspl("\tsetdir:\t[l|r|m]\n", 
			"\tpause:\t in ms. ie: 3000\n",
			"\tdRGB, dR, dG, dB, dW:\tdecay channel(s), in ms. ie: 5\n",
			"\tmirror:\ttoggle mirror setting\n",
			"\tstart/stop:\tstart or stop the current sequence\n",
			"\tbl: set uniform baseline colour\n",
			"\tfps: set global frame speed\n",
			"\tstats: display in-use global values\n",
			"\thelp:\tthis information.\n",
			"\t----------\n");
		else if (strstr(rcvdChars, "setdir")) {
			char* pC = strchr(rcvdChars,' ')+1;
			// char C = rcvdChars[7];
			char C = *pC;
			if (C=='l' || C=='L') {
				LeftRightMiddle='l';
				if (Para.get(_mirror)) Para.set(_mirror);		//	 ->false
			}
			else if (C=='r' || C=='R') {
				LeftRightMiddle='r';
				if (Para.get(_mirror)) Para.set(_mirror);		//	-->false
			}
			else if (C=='m' || C=='M') {
				LeftRightMiddle='m';
				if (Para.get(_mirror)==false) Para.set(_mirror);		//	-->true
			}
		}
		else if (strstr(rcvdChars, "pause"))		_pulseInterval=min(max(myarg,100),10000);
		else if (strstr(rcvdChars, "dRGB"))			{
			_decayArrayRGBW[0]=myarg;	_decayArrayRGBW[1]=myarg;	_decayArrayRGBW[2]=myarg;	}
		else if (strstr(rcvdChars, "dR"))				_decayArrayRGBW[0]=myarg;
		else if (strstr(rcvdChars, "dG"))				_decayArrayRGBW[1]=myarg;
		else if (strstr(rcvdChars, "dB"))				_decayArrayRGBW[2]=myarg;
		else if (strstr(rcvdChars, "dW"))				_decayArrayRGBW[3]=myarg;
		else if (strstr(rcvdChars, "fps"))			_decayTick(myarg);
		else if (strstr(rcvdChars, "mirror"))		Para.set(_mirror);
		else if (strstr(rcvdChars, "start"))		Para.set(_start);
		else if (strstr(rcvdChars, "stop"))			Para.set(_stop);
		else if (strstr(rcvdChars, "pp"))				_priorityPixel=myarg;
		else if (strstr(rcvdChars, "rad"))			Para.set(radiate);
		else if (strstr(rcvdChars, "decay"))		Para.set(decay);
		else if (strstr(rcvdChars, "kr"))				{Para.set(krider); Para.set(decay);}
		else if (strstr(rcvdChars, "comet"))		{Para.set(comet); Para.set(decay);}
		else if (strstr(rcvdChars, "spark"))		{Para.set(sparkle); Para.set(decay);}
		else if (strstr(rcvdChars, "bl"))				{
			byte newBL[4]{0,0,0,0};
			int i{0};
			while (I != NULL & i<4) {			//	r g b w
				newBL[i++] = (byte)myarg;
				I = strchr(I+1,' ');				//	delimiters are uniformly single spaces.
				(I!=NULL)?myarg = (byte)atoi(I):myarg = 0;
			}
			setBaselineColourFromRGBW(newBL[0], newBL[1], newBL[2], newBL[3]);
		}
		else if (strstr(rcvdChars, "gamma")) 		_baselineColour = gamma32(_baselineColour);
		else if (strstr(rcvdChars, "Y")) 				strip.setPixelColor(_priorityPixel,sunriseB);
		else if (strstr(rcvdChars, "X")) 				strip.setPixelColor(_priorityPixel,sunriseA);
		else if (strstr(rcvdChars, "stat"))		{
			dspl("\tdRGBW:", _decayArrayRGBW[0], _decayArrayRGBW[1], _decayArrayRGBW[2], _decayArrayRGBW[3], "ms.", "\tpause:", _pulseInterval, "\tmirror:", Para.get(_mirror)?"Y":"N", "\tsparkles:", Para.get(sparkle)?"Y":"N");
			Serial.print("\tbl.colour(0x");
			Serial.print(_baselineColour,HEX);
			Serial.print(")");
			dspl("\tvip:", _priorityPixel, "\tdir:", LeftRightMiddle);
			// dspl("\tcurrent mode:", Para.get(_stop)?"not running":Para.get(decay)?"baseline decay":Para.get(radiate)?"radiate":"", Para.get(comet)?"comet":Para.get(krider)?"kRider":"\n");
			dspl("\tcurrent mode:", Para.get(_stop)?"not-running":Para.get(comet)?"comet":Para.get(krider)?"kRider":"X", Para.get(decay)?"baseline-decay":Para.get(radiate)?"radiate":"X");
			dspl("    - - - - - - - - - - ");
		}
		else dspl("  --> type 'help' for cmd list.");
		newCmd=false;
	}
}

bool parse_delimiter(char CH) {
	static char validChars[] {' ', ',', '_', '+', '&'};
	for (int i = 0; i< sizeof(validChars)-1;i++){
		if (CH == validChars[i]) return true;
	}
	return false;
}

bool parse_eol(char CH) {
	static char validChars[] {'\n', '\r', '>', ']', '\0'};
	for (int i = 0; i< sizeof(validChars)-1;i++){
		if (CH == validChars[i]) return true;
	}
	return false;
}

void serialReceived() {
	// ideal cmd strings look like "word 0123 \0" < end with one space, and a null (3)
	//				(1)	no leading spaces^    ^single space (2)
	static byte ndx{0};
	char rc;
	while (Serial.available() > 0 && newCmd == false) {
		rc = Serial.read();
		if (parse_eol(rc)==false) {
			if (parse_delimiter(rc)) {
				rc = ' ';		//	standardise delimiter to space char &#x20;
				if (ndx==0) continue;		//	skip *initial* delimiters (1)
				else if (rcvdChars[ndx-1] == rc) continue;		//	one is enough (2)
			}	
			rcvdChars[ndx++] = rc;			//	record this char
			if (ndx >= numChars-1)	ndx = numChars-2;		//	truncate. reserve for padding.
		}
		else {
			rcvdChars[ndx] = ' ';				//	PADDING	(3)
			rcvdChars[ndx+1] = '\0';		//	terminate the string	(3)
			if (ndx>0) {								//	empty strings get tossed
				ndx = 0;
				newCmd = true;				
			}
		}
	}
}


String SendHTML(uint8_t led1stat,uint8_t led2stat) {
	String ptr = "<!DOCTYPE html> <html>\n";
	ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
	ptr +="<title>Kitten Lights Control</title>\n";
	ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
	ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
	ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
	ptr +=".button-on {background-color: #1abc9c;}\n";
	ptr +=".button-on:active {background-color: #16a085;}\n";
	ptr +=".button-off {background-color: #34495e;}\n";
	ptr +=".button-off:active {background-color: #2c3e50;}\n";
	ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
	ptr +="</style>\n";
	ptr +="</head>\n";
	ptr +="<body>\n";
	ptr +="<h1>ESP8266 Web Server</h1>\n";
	if (mode_STA) ptr +="<h3>Using Station(STA) Mode</h3>\n";
	else ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
	
	 if(led1stat)
	{ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
	else
	{ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

	if(led2stat)
	{ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
	else
	{ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

	ptr +="</body>\n";
	ptr +="</html>\n";
	return ptr;
}

void Shift(int places, int startAt, int endAt) {
	int _head, _tail, pp;
	bool shifdn{false};
	//	begin safety inspector
	if (startAt<0 || startAt>strip.numPixels()) startAt = strip.numPixels();
	if (endAt<0 || endAt >strip.numPixels()) endAt = 0;
	if (abs(places)>abs(startAt-endAt) || places==0) places = 1;
	//	begin direction finding
	shifdn = (startAt>endAt);
	_tail = (shifdn?min(startAt, endAt):max(startAt, endAt));
	_head = (shifdn?max(startAt, endAt):min(startAt, endAt));
	shifdn?places=abs(places):places=-abs(places);
	pp = _tail+places;
	//	shift array values
	while (shifdn?pp<=_head:pp>=_head) {
		strip.setPixelColor(pp-places, strip.getPixelColor(pp));
		shifdn?pp++:pp--;
	}
}

void Radiate() {
	// copy the centre LED value gradually to the head/tail ends
	int midway = strip.numPixels()/2;
	Shift(1, midway, 0);
	Shift(1, midway, strip.numPixels());
	_pixelDecay(midway);
}
void _pixelDecay(int led) {
	//	baseline decay against ONE led.
	uint8_t* Ptr = _getSpecificLED_ptr(led);
	uint8_t* blCh = (uint8_t*)&_baselineColour;
 	for (int ch = 0; ch < 4; ch++) {
		int d = _decayArrayRGBW[dumb[ch]]; 		//	0xwwbbrrgg
		// T: 1.87 µs.
		if      (Ptr[ch]>blCh[ch]) Ptr[ch]= max( (int)blCh[ch], (Ptr[ch]- d));
		else if (Ptr[ch]<blCh[ch]) Ptr[ch]= min( (int)blCh[ch], (Ptr[ch]+ d));
		else Ptr[ch] = blCh[ch];
		// T: 2.02 µs.  neater but slower (?)
		// while (Ptr[ch]>blCh[ch] && d>0) {Ptr[ch]--; d--;}
		// while (Ptr[ch]<blCh[ch] && d>0) {Ptr[ch]++; d--;}
	}
}
uint8_t* _getSpecificLED_ptr(int LEDpixel_number) {
	uint8_t* ptr = strip.getPixels();
	if (LEDpixel_number>0 && LEDpixel_number<=strip.numPixels()) ptr += (LEDpixel_number*4);
	return ptr;
}
void baselineDecay() {
	uint32_t* SUM = (uint32_t*) &_decayArrayRGBW;
	if (*SUM==0) return;		//	don't waste time on array when no changes will be made. 
	for (int i = 0; i < strip.numPixels(); i++) {
		_pixelDecay(i);
	}
}

int CometTail(uint32_t colA) {
	// T:6250µS
	if (timeout < millis()) {
		int p_from, p_to;
		if (LeftRightMiddle=='l') {p_from=0; p_to=LED_COUNT;}
		if (LeftRightMiddle=='r') {p_from=LED_COUNT; p_to=0;}
		if (LeftRightMiddle=='m') {p_from=(int)LED_COUNT/2; p_to=LED_COUNT;}
		int maximum = abs(p_to - p_from);
		timeout = millis() + risingHalfSineWave(shortestDelay, longestDelay, step, maximum);
		if (step==0) strip.setPixelColor(p_from, colA);
		else {
			strip.setPixelColor(p_from +step, (colA));
			if (Para.get(_mirror)) strip.setPixelColor(LED_COUNT -(p_from +step), (colA));
		}
		(p_from<p_to)?step++:step--;
		if (abs(step)>maximum) {
			step = 0;
			timeout+=_pulseInterval;
		}
		return step;
	}
	return 0;
}

void Sparkles(uint32_t Pop) {
	int R = random(0,150);
	//bitwise OR addition of Pop colour to existing pixel colour
	strip.setPixelColor(R, strip.getPixelColor(R)|Pop);
}

void Knightrider(uint32_t colA, int width) {
	// T:
	static bool swing{false};
	int p_mid=(strip.numPixels()/2)-width;
	int p_high=strip.numPixels()-width;
	int p_low{0};
	if (timeout < millis()) {
		timeout = millis() + halfCircle(p_mid, step, true);
		int w{0};
		while (w<width) {
			strip.setPixelColor(step+w, colA);
			w++;
		}
		if (step>=(p_high) || step<=(p_low)) 	swing= !swing;
		(swing)?step++:step--;
	}
}

int cpuCycles(void (*fPoint)(void)) {
	int iter = 1000;
	uint32_t T = micros();
	float T2;
	for (int i = 0; i<iter; i++) {
			fPoint();
	}
	T2=micros()-T;
	dspl("T:", T2/iter, "µs.");
	return T2/iter;
}
void  cpuCycles() {
	int iter = 100000;
	uint32_t Tu = micros();
	uint32_t Tm = millis();
	float Tu2;
	float Tm2;
	int v=0;
	for (int i = 0; i<iter; i++) {
			_pixelDecay(v);
			v<strip.numPixels()?v++:v=0;
	}
	Tm2=millis()-Tm;
	Tu2=micros()-Tu;
	dspl("T:", Tu2/iter, "µs.", Tm2/iter, "ms.");
	return;
}

void setup() {
	/* feedback */
	Serial.begin(115200);
	// Serial.setDebugOutput(true);		//		only needed for wifi
	/* LED strip */
	strip.begin();						//	INITIALIZE NeoPixel object
	strip.clear();						//	clear led array
	/* WiFi and server */
	Init_WiFiMode(true);			//	wifi and IP address
	Init_ServerCmds();				//	list of f() callbacks
	Para.set(radiate);
	// cpuCycles();
	
}		//		- = - = - = - = - E N D   s e t u p ( ) - = - = - = - = - 

fourChannelTransition fCT(0, sunsetB);

void loop() {	
	uint32_t _from, _to;		//	0xWwBbRrGg
	server.handleClient();
	MDNS.update();
	serialReceived();
	handleCLI();
	if (Para.get(_start)) {
		if (Para.get(comet)) 	CometTail(strip.Color(random(0x40,0xf0), random(0x40,0xf0), random(0x40,0xf0), random(0x00,0x10)));
		else if (Para.get(krider)) 	Knightrider(sunsetA, 12);
	}
	else {
		if (Para.get(sparkle) && (random(0,1000)<1)) {
			Sparkles(0x6f000000);	Sparkles(0x006f6f6f);	}		
	}
	
	if (_decayTick()) {
		strip.show();				//	this should be the *only* time strip.show() is called. 
		if (Para.get(decay)) baselineDecay();		//	to disable set _dRGB/W to 0.
		else if (Para.get(radiate)) Radiate();
	}
	
}		//		- = - = - = - = - E N D   l o o p ( ) - = - = - = - = - 

