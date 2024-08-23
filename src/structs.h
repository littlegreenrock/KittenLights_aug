/*	enums and structs 	------------------------------------------------	*/
#include <Arduino.h>
#ifndef __STRUCTS_H__
#define __STRUCTS_H__

extern const uint8_t DEBOUNCE_DELAY;
extern const byte NEOPixelChannelOrder[4];

	const uint8_t g_table[256] = {		// 2.2
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
			  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
			  3,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  6,  6,  6,
			  6,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 11, 11, 11, 12,
			 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19,
			 20, 20, 21, 22, 22, 23, 23, 24, 25, 25, 26, 26, 27, 28, 28, 29,
			 30, 30, 31, 32, 33, 33, 34, 35, 35, 36, 37, 38, 39, 39, 40, 41,
			 42, 43, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55,
			 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
			 73, 74, 75, 76, 77, 78, 79, 81, 82, 83, 84, 85, 87, 88, 89, 90,
			 91, 93, 94, 95, 97, 98, 99,100,102,103,105,106,107,109,110,111,
			113,114,116,117,119,120,121,123,124,126,127,129,130,132,133,135,
			137,138,140,141,143,145,146,148,149,151,153,154,156,158,159,161,
			163,165,166,168,170,172,173,175,177,179,181,182,184,186,188,190,
			192,194,196,197,199,201,203,205,207,209,211,213,215,217,219,221,
			223,225,227,229,231,234,236,238,240,242,244,246,248,251,253,255 
	};


enum animationDirection: byte {
	// previous _mirror is replaced by _lateral/_medial
	_static, _proximal, _distal, _lateral, _medial,
};

enum colCh: byte  {
	rr  =0, Red		=0, 
	gg  =1, Green	=1, 
	bb  =2, Blue	=2, 
	ww  =3, White	=3, 
	rgb =4, RGB		=4,	//	instructional. referring to the 3 colour channels. there's no byte 4!
	rgbw=5, RGBW	=5,	//	reffering to all 4 channels. There's no byte 5!
};

enum action: uint8_t 
{
	 	//		toggle bits
	_start=0, decay, radiate, sparkle,
		//		exclusive bits
	cometTail, larsonSeq, twinkle, festoon, dirFwd, calibrateOrientation, LiveFollow,
		//		unmapped functionss
	_stop=16,	
};
struct commsMode_t { 
	uint8_t bitMap8{};
	enum mod: byte {
		WiFi=0, STA, AP, mDNS, WS, RemoteXY, BT, leBT,  	//	bits  0b00101011
		offline=0xf0, none=0xff,											//	functions
	};
	bool set(mod M) {
		switch (M) {
			default 			:	return false; 												break;
			case offline	:	_clr(WiFi); 													break;
			case WiFi			:	_set(M); 															break;
			case STA			:	_set(WiFi);	_set(M);		_clr(AP);			break;
			case AP 			:	_set(WiFi);	_set(M);		_clr(STA);		break;
			case mDNS			:	_set(WiFi);	_set(M);									break;
			case RemoteXY	:	_set(M);		_clr(WS);									break;
			case WS				:	_set(M);		_clr(RemoteXY);						break;
			case BT 			:	_set(M);		_clr(leBT);	_clr(WiFi);		break;
			case leBT			:	_set(M);		_clr(BT);		_clr(WiFi);		break;
		}
		return true;
	}
	bool get(mod M) {
		return bitRead(bitMap8,int(M));
	}
	bool get(mod M, mod N) {
		return get(M) && get(N);
	}
	private:
	bool _toggle(mod B) {
		bitWrite(bitMap8, int(B), !(bitRead(bitMap8, int(B))));
		return bitRead(bitMap8, int(B));
	}
	void _set(mod B) {
		bitSet(bitMap8, int(B));
	}
	void _clr(mod B) {
		bitClear(bitMap8, int(B));
	}
};
struct displayMode_t {
	uint16_t bitMap16{2};		//	0000 0010 : stop, baseline-decay
	animationDirection Dir;
	bool set(action Ac) {
		switch (Ac) {
			default 				:	return false; break;
			case decay  		:	bitSet(bitMap16,Ac);	bitClear(bitMap16,int(action::radiate)); break;
			case radiate		:	bitSet(bitMap16,Ac);	bitClear(bitMap16,int(action::decay)); break;
			case cometTail  :	_clearExclusives(Ac); break;
			case larsonSeq 	:	_clearExclusives(Ac); break;
			case sparkle 		:	_toggle(Ac);	break;
			case twinkle		:	_clearExclusives(Ac); break;
			case festoon		:	_clearExclusives(Ac); break;
			case LiveFollow	: _clearExclusives(Ac); break;
			case _start			:	bitSet(bitMap16,Ac);  break;
			case _stop			:	bitClear(bitMap16,int(action::_start));  break;
		}
		return true;
	}
	bool get(action Ac) {
		bool result{false};
		switch (Ac) {
			case _stop			:	result = !bitRead(bitMap16,int(action::_start));	break;
			case _start			:	;
			case decay  		:	;
			case radiate		:	;		//	intentional fall through
			case sparkle 		:	;
			case cometTail	:	;
			case twinkle		:	;
			case festoon		: ;
			case larsonSeq	:	;
			default 				:	result = bitRead(bitMap16,int(Ac));	break;
		}
		return result;
	}
	bool _toggle(action Ac) {
		bitWrite(bitMap16, int(Ac), !(bitRead(bitMap16, int(Ac))));
		return bitRead(bitMap16, int(Ac));
	}
	void _clearExclusives(action bitParameter=_stop) {
		const uint16_t exclusivesBitmap = 0b0000000000011111;		//	0's are the exclusives
		if (bitParameter > sparkle && bitParameter < _stop) {		//	0's are the exclusives
			bitMap16 &= exclusivesBitmap;
			bitSet(bitMap16, int(bitParameter));
		}
	}
};
struct timeout_t {
	uint32_t time{};
	void add(int _ms) {
		time += _ms;	}
	void set(int _ms) {
		time = millis() + _ms;	}
	bool valid() {
		return time > millis();	}
	bool expired() {
		return time <= millis();	}
};
struct Led_t {
	//	Definition of single/onboard LED		-----------------------------------------
	//		calling with bool paramater true sets this led to behave as GND = on. 
	//			or - this is a +Vcc LED that is GNDed to turn on. 
	//			or - it's illumination logic is inverted.
	const uint8_t pin;
	const bool inverted;
	bool state;
	Led_t(uint8_t p, bool Onboard_LED_has_inverted_logic=0) : 
			pin{p}, inverted{Onboard_LED_has_inverted_logic} {};
	void update() {
		digitalWrite(pin, (state^(inverted) ? HIGH:LOW));
	} 
	void  on()	{	state = true;		}
	void off()	{	state = false;	}
	void tog()	{	state = !state;	}
};
struct Button_t {
	// Definition of the Physical Button 		---------------------------------------
	const uint8_t  pin;
	const bool a_btn_press_is_LOW;
	Button_t(uint8 assignedPin, bool button_is_wired_as_PULLUP) : pin{assignedPin}, a_btn_press_is_LOW{button_is_wired_as_PULLUP} {};
	bool     lastReading{0};
	uint32_t lastDebounceTime{0};
	uint16_t _state{0};
	bool pressed()			{ return _state == 1; }
	bool released()			{ return _state == 0xffff; }
	bool held(uint16_t _ms = 0) {
		//	use: has button been held for over x milliseconds?  default: held irregardless of time
		//	after ~40s might encounter rollover issues.
		return (_state<0xffff && _state*DEBOUNCE_DELAY > _ms+DEBOUNCE_DELAY); 
	}
	void read() {
		bool thisRead = digitalRead(pin);
		if (thisRead != lastReading) {
			// no match, reset timer
			lastDebounceTime = millis();
		}
		if (millis() - lastDebounceTime > DEBOUNCE_DELAY) {
			//	is that read-pin a pullup, Vcc; or pulldown, GND?
			bool pressed = thisRead!=a_btn_press_is_LOW;		
			if (pressed) {
				if (_state  < 0xfffe) _state++;
				else if (_state == 0xfffe) _state = 2;
			} else if (_state) _state = _state == 0xffff ? 0 : 0xffff;
		}
		lastReading = thisRead;
	}
};
struct decayRate_t {
	byte R,G,B,W;
	bool isZero()	{
		return (R+G+B+W == 0);	
	}
	int dAmt(int ch) {
		switch (NEOPixelChannelOrder[ch]) {		//	previously called dumb[]
		// give channel number, returns byte number.
			case 1 : return R;		//	0
			case 0 : return G;		//	1
			case 2 : return B;		//	2
			case 3 : return W;		//	3
		}
		return 0;
	}
	void valid() {
		R=min(25,int(R)); R=max(0,int(R));
		G=min(25,int(R)); G=max(0,int(R));
		B=min(25,int(R)); B=max(0,int(R));
		W=min(25,int(R)); W=max(0,int(R));
	}
	void setR(byte val)	{	R=val;	}
	void setG(byte val)	{	G=val;	}
	void setB(byte val)	{	B=val;	}
	void setW(byte val)	{	W=val;	}
	void setRGB(byte val) {	setR(val);		setG(val);	setB(val);	}
	void setAll(byte val) {	setRGB(val);	setW(val);	}
};
struct ddd_t {
	int _orientation[3] {0};
	
	bool operator==(const ddd_t& R) const {
		return (	(R._orientation[0] == ddd_t::_orientation[0]) &&
							// (R._orientation[1] == ddd_t::_orientation[1]) &&
							(R._orientation[2] == ddd_t::_orientation[2]) )
							;}
	enum axis : byte {
		// just an alias for P/Pitch to be used as array pointer.
		P=0, R=1, Y=2, Pitch=0, Roll=1, Yaw=2,
	};
	int plug(float piValue) {
		return int(573*piValue);
	}
	void set(axis A, float val) {
		_orientation[int(A)] = plug(val);
	}
	void set(float pitch, float roll, float yaw) {
		_orientation[int(Pitch)]	=	plug(pitch);
		_orientation[int(Roll)]		=	plug(roll);
		_orientation[int(Yaw)]		=	plug(yaw);
	}
	int get(axis A) {
		return _orientation[int(A)];
	}
	int p() {	return _orientation[0];	}
	int r() {	return _orientation[1];	}
	int y() {	return _orientation[2];	}
};

struct new_decayRate_t {
	/*		decay rate is now assigned as ms delay between full and zero led illumination.
			or, delay to traverse 255 linear-levels of brightness.  higher numbers take longer
			to fade. requires frame rate (fps) and access to gamma algorithm.
	*/
private:
	int dTime_ch[4];		//	given ms decay time
	float LBL[4];				//	calculated Linear-Brightness-Levels per frame

	int _reverseLookupLoop(byte lookup, int j) {
		while (lookup < g_table[j]) { j--; }
		return j;		//	returns index
	}

	byte _findLinearIndex(byte lookup) {
		// quick determine which third lookup value can be found, now we only need to 
		// compare up to 85 values, rather than 255.
		int R;
		if (lookup < g_table[85]) {
			// lowest one-third of values
			R = _reverseLookupLoop(lookup, 85);
		}
		else if (lookup < g_table[170]) {
			// middle third of values
			R = _reverseLookupLoop(lookup, 170);
		}
			// highest third.
		else R = _reverseLookupLoop(lookup, 255);
		R = (byte)min(255,max(0,R));		//	ensure R stays within byte range.
		return R;
	}
	void _calcLBL_values() {
		for (int ch = 0; ch< 4; ch++) {
			float denominator = fps * (float)(dTime_ch[ch]/1000);
			if ((denominator>0) == false) denominator = 1;
			LBL[ch] = float(256.0 / denominator);
		}
	}
public:
	unsigned int fps{40};

	byte getDecay(colCh chan) {
		return dTime_ch[int(chan)];
	}
	void setDecay(byte value, colCh channelCode) {
		int i=0;
		int j = int(channelCode);
		switch (channelCode) {
			case rr :
			case gg :
			case bb :
			case ww : i = j; break;
			case rgb:	j = 2; break;
			case rgbw : j = 3; break;
			default : break;
		}
		for (; i <= j; i++) {
			dTime_ch[i] = value;
		}	
		_calcLBL_values();
	}
	byte decay(byte unknownPix, byte targetValue, float* lbl) {
		//	first, try to shortcut the process.  Near enough is good enough,
		//	and you won't notice the difference anyway ;)
		if (abs(unknownPix-targetValue)<2) return targetValue;
		//	is unknown above or below the target value?
		bool HI = (unknownPix > targetValue);
		byte newValue = _findLinearIndex(unknownPix) + (HI? -*lbl : *lbl) ;
		if (HI) return max(newValue,targetValue);
		else return min(newValue, targetValue);
	}
	uint32_t decay(uint32_t unknownColour, uint32_t targetColour) {
		uint32_t newColour{0};
		byte* uCp = (byte*)&unknownColour;
		byte* tCp = (byte*)&targetColour;
		byte* nCp = (byte*)&newColour;
		for (int i = 0; i<4; i++) {
			nCp[i] = decay(uCp[i], tCp[i], &LBL[NEOPixelChannelOrder[i]]);
		}
		return newColour;
		// for (int i = 0; i< 4; i++) {
		// 	byte uC = unknownColour>>(i*4) & 0x000000ff;
		// 	byte tC = targetColour>>(i*4) & 0x000000ff;
		// 	byte nC = decay(uC, tC);
		// 	newColour<<=(i*4);
		// 	newColour&=nC;
		// }
		// return newColour;
	}
};


	
byte Gamma(byte linear) {
	/*
			if using : const uint8_t PROGMEM gamma8[]  = {...};
			use: return pgm_read_byte(&gamma8[linear]);
	*/
	return g_table[linear];
}

// attempts to estimate what the linear value was, rather than use the lookup table
byte deGamma(byte gammaValue) {
	int Lin=0;		//		index of g_table[], and our estimated original linear value
	int n=0;		//		number of times (if any) Val appears in the table
	if (gammaValue >= 255) return g_table[255];
	else {			
		while (gammaValue <= g_table[Lin]) {
			if (g_table[Lin++] == gammaValue) n++;
		}
	}
	if (n<3) return Lin-1;
	else return (byte)((Lin-n)+(n/2));
}
	


//	Generate an LED gamma-correction table for Arduino sketches, given any gamma factor.
//	2.8 is a common normal.  I personally like 2.2.
void generateGamma8Table(float gamma=2.8) {
	int	max_in  = 255,		//	Top end of INPUT range
			max_out = 255;		//	Top end of OUTPUT range
	Serial.printf("const uint8_t gamma[] = {\t\t\\\\\tbased on gamma factor %1.1f", gamma);
  for(int i=0; i<=max_in; i++) {
		if(i > 0) Serial.print(',');
		if((i & 15) == 0) Serial.print("\n  ");
		Serial.printf("%3d", (int)(pow((float)i / (float)max_in, gamma) * max_out + 0.5));
	}
	Serial.println(" };");
}


#endif 		//		__STRUCTS_H__
