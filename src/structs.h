/*	enums and structs 	------------------------------------------------	*/
#include <Arduino.h>
#ifndef __STRUCTS_H__
#define __STRUCTS_H__

const uint8_t DEBOUNCE_DELAY = 20;		//	milliseconds
byte gamma8(byte);

const uint8_t g_table[256] = {			// 2.2
//			0		1		2		3		4		5		6		7		8		9		a		b		c		d		e		f
				0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,	//	0
				1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,	//	1
				3,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  6,  6,  6,	//	2
				6,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 11, 11, 11, 12,	//	3
			 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19,	//	4
			 20, 20, 21, 22, 22, 23, 23, 24, 25, 25, 26, 26, 27, 28, 28, 29,	//	5
			 30, 30, 31, 32, 33, 33, 34, 35, 35, 36, 37, 38, 39, 39, 40, 41,	//	6
			 42, 43, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55,	//	7
			 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,	//	8
			 73, 74, 75, 76, 77, 78, 79, 81, 82, 83, 84, 85, 87, 88, 89, 90,	//	9
			 91, 93, 94, 95, 97, 98, 99,100,102,103,105,106,107,109,110,111,	//	a
			113,114,116,117,119,120,121,123,124,126,127,129,130,132,133,135,	//	b
			137,138,140,141,143,145,146,148,149,151,153,154,156,158,159,161,	//	c
			163,165,166,168,170,172,173,175,177,179,181,182,184,186,188,190,	//	d
			192,194,196,197,199,201,203,205,207,209,211,213,215,217,219,221,	//	e
			223,225,227,229,231,234,236,238,240,242,244,246,248,251,253,255 	//	f
	};
const uint8_t l_table[256] = {
//			0			1			2			3			4			5			6			7			8			9			a			b			c			d			e			f
			0x07,	0x13,	0x1c,	0x22,	0x26,	0x2a,	0x2e,	0x32, 0x35, 0x38, 0x3a,	0x3d, 0x40, 0x42, 0x45, 0x47,	//	0
			0x49, 0x4b, 0x4d, 0x4f, 0x51, 0x52, 0x54, 0x56, 0x57, 0x59, 0x5b, 0x5c, 0x5e, 0x5f, 0x61, 0x62,	//	1
			0x63, 0x65, 0x66, 0x68, 0x69, 0x6a, 0x6b, 0x6d, 0x6e, 0x6f, 0x70, 0x72, 0x73, 0x74, 0x75, 0x76,	//	2
			0x77, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, //	3
			0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,	//	4
			0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa0, 0xa1, 0xa2, 0xa3,	//	5
			0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa7, 0xa8, 0xa9, 0xa9, 0xaa, 0xab, 0xac, 0xac, 0xad, 0xae, 0xaf,	//	6
			0xaf, 0xb0, 0xb1, 0xb1, 0xb2, 0xb3, 0xb3, 0xb4, 0xb5, 0xb6, 0xb6, 0xb7, 0xb8, 0xb8, 0xb9, 0xba,	//	7
			0xba, 0xbb, 0xbc, 0xbc, 0xbd, 0xbe, 0xbe, 0xbf, 0xbf, 0xc0, 0xc1, 0xc1, 0xc2, 0xc3, 0xc3, 0xc4,	//	8
			0xc4, 0xc5, 0xc6, 0xc6, 0xc7, 0xc8, 0xc8, 0xc9, 0xc9, 0xca, 0xcb, 0xcb, 0xcc, 0xcc, 0xcd, 0xce,	//	9
			0xce, 0xcf, 0xcf, 0xd0, 0xd0, 0xd1, 0xd2, 0xd2, 0xd3, 0xd3, 0xd4, 0xd4, 0xd5, 0xd6, 0xd6, 0xd7,	//	a
			0xd7, 0xd8, 0xd8, 0xd9, 0xd9, 0xda, 0xdb, 0xdb, 0xdc, 0xdc, 0xdd, 0xdd, 0xde, 0xde, 0xdf, 0xdf,	//	b
			0xe0, 0xe0, 0xe1, 0xe1, 0xe2, 0xe3, 0xe3, 0xe4, 0xe4, 0xe5, 0xe5, 0xe6, 0xe6, 0xe7, 0xe7, 0xe8, //	c
			0xe8, 0xe9, 0xe9, 0xea, 0xea, 0xeb, 0xeb, 0xec, 0xec, 0xed, 0xed, 0xee, 0xee, 0xef, 0xef, 0xf0,	//	d
			0xf0, 0xf1, 0xf1, 0xf2, 0xf2, 0xf3, 0xf3, 0xf4, 0xf4, 0xf4, 0xf5, 0xf5, 0xf6, 0xf6, 0xf7, 0xf7, //	e
			0xf8, 0xf8, 0xf9, 0xf9, 0xfa, 0xfa, 0xfb, 0xfb, 0xfc, 0xfc, 0xfc, 0xfd, 0xfd, 0xfe, 0xfe, 0xff,	//	f
};

enum animationDirection: byte {
	// previous _mirror is replaced by _lateral/_medial
	_static, _proximal, _distal, _lateral, _medial,
};

//const byte NEOPixelChannelOrder[4];		//	0bWWRRGGBB  = NEO_GRBW


class baselinebyte {
public:
uint32_t baselineColour;
byte* const blc_pt = (byte*)&baselineColour;


};


class Numb {
public:
	Numb() {}
	Numb(int x) : Val{byte(x)} {}

	Numb& operator +=(const Numb& right) {
		this->Val = min(0xff, ((int)this->Val + right.Val));
		return *this;
	}
	Numb& operator -=(const Numb& right) {
		this->Val = max(0x00, (this->Val - right.Val));
		return *this;
	}
	Numb& operator ++() {		// prefix
			Val<0xff?++Val:Val=0xff;
			return *this;
	}
	Numb operator ++(int unused) {		// postfix
			Numb result = *this;
			Val<0xff?++Val:Val=0xff;
			return result;
	}
	Numb& operator --() {		// prefix
			Val>0x00?--Val:Val=0x00;
			return *this;
	}
	Numb operator --(int unused) {		// postfix
			Numb result = *this;
			Val>0x00?--Val:Val=0x00;
			return result;
	}
	bool operator ==(const Numb& right)	{
		return (this->Val == right.Val);
	}
	bool operator ==(const auto& right)	{
		return (this->Val == right);
	}
	bool operator !=(const Numb& right)	{
		return (this->Val != right.Val);
	}
	bool operator !=(const auto& right)	{
		return (this->Val != right);
	}
	bool operator >(const auto& right)	{
		return (int(this->Val) > int(right));
	}
	bool operator <(const auto& right)	{
		return (int(this->Val) < int(right));
	}
	bool operator >=(const auto& right)	{
		return (int(this->Val) >= int(right));
	}
	bool operator <=(const auto& right)	{
		return (int(this->Val) <= int(right));
	}
	byte Lin() {
		return l_table[Val];
	}
	byte Lin(int lbl, bool Higher) {
		return g_table[(byte(int(l_table[Val] + (Higher?-lbl:lbl))))];
	}
	byte Gam() {
		return g_table[Val];
	}
	byte pnt() {
		return Val;
	}
private:
	byte Val;
};


enum illumination: uint8_t 
{
	 	//		toggle bits
	_start=0, decay,
		//		exclusive bits
	 radiate, sparkle, cometTail, larsonSeq, twinkle, festoon, LiveCursor,
		//		unmapped functionss
	_stop=0x80,	
};

struct timeout_t {
	uint32_t time{0};
	void Extend(int _ms) {
		time += _ms;	}
	void Next(int _ms) {
		time = millis() + _ms;	}
	bool Valid() {
		return (time > millis());	}
	bool CheckPoint(int _ms) {
		if (Valid()==false) {
			this->Next(_ms);
			return true;
		}
		return false;	}
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
	const bool theTruePressState;
	Button_t(uint8 assignedPin, bool button_is_wired_as_PULLUP) : pin{assignedPin}, theTruePressState{!button_is_wired_as_PULLUP} {};
	bool     lastReading{0};
	uint32_t lastDebounceTime{0};
	uint16_t _state{0};
	bool pressed()			{ return _state == 1; }
	bool released()			{ return _state == 0xffff; }
	bool held(uint16_t _ms = 0) {
		//	use: has button been held longer _ms?  default: held irregardless of time
		//	after ~40s might encounter rollover issues.
		return (_state<0xffff && (_state*DEBOUNCE_DELAY) > (_ms+DEBOUNCE_DELAY)); 
	}
	void read() {
		bool thisRead = digitalRead(pin);
		if (thisRead != lastReading) {
			// no match, reset timer
			lastDebounceTime = millis();
		}
		if ( millis() - lastDebounceTime > DEBOUNCE_DELAY) {
			//	is that read-pin a pullup, Vcc; or pulldown, GND?
			bool pressed = thisRead==theTruePressState;		
			if (pressed) {
				if (_state  < 0xfffe) _state++;
				else if (_state == 0xfffe) _state = 2;
			} else if (_state) _state = (_state == 0xffff) ? 0 : 0xffff;		//	released
		}
		lastReading = thisRead;
	}
};



struct decayChan8_t {
	byte LBLpFrame[3]{0};
	void calcLinearBrightnessLevels(int _decayTime, const int fps) {
		float nFrames = max((float)1.0, (float)fps * (float)_decayTime / 1000);
		float LpFr = 256 / nFrames;
		LBLpFrame[0] = byte((LpFr * 1) +0.5);
		LBLpFrame[1] = byte((LpFr * 2) +0.5) -LBLpFrame[0];
		LBLpFrame[2] = byte((LpFr * 3) +0.5) -LBLpFrame[0] -LBLpFrame[1];
		// if (DEBUG) Serial.printf("%i, %i, %i, \n", LBLpFrame[0],LBLpFrame[1],LBLpFrame[2]);
	}
};

class decayColour32_t {
	public:
		bool Tick() ;
		void setFPS(int) ;
		void updateChannels() ;
		void setDecaytime(int, uint8_t) ;
		uint32_t Decay(uint32_t , uint32_t ) ;
		void Begin(int, int ) ;

	private:
		decayChan8_t dChannel[4];
		int _indvlChannelDecayTime[4]{400};
		int _fps{40};
		uint32_t _time{0};
		int _tickTimeSteps[3];
		byte f{0};
		void _updateChannel(int ) ;

};		//		END class decay_colour32_t

	void decayColour32_t::Begin(int fps, int decayTime) {
		setFPS(fps);
		setDecaytime(decayTime, 5);
	}

/*		This tick is different than timeout_t.  Since we're ticking based off
	fps/1000, rather than X ms., we will never have accurate integral incremental steps. So
	a needless way to improve this is to calculate 2 additional ms tick steps. Tick()
	now compares millis() - tickTimeStep(f) against the last stored millis(). After the 3rd 
	step (f==2) a fresh millis() is stored in _time. This prevents using floating point and 
	rounding errors, improving tick accuracy.  This is notably observed when using high 
	fps values (60+) and high decay times (>2000ms). This correction measurably improves
	tick accuracy by 2ms / 100ms (2%) without the need for constant floating point use.
*/
	bool decayColour32_t::Tick(){
		if ((_fps > 5) && (millis() - _tickTimeSteps[f]) >= _time) {
			if (++f > 2) {	_time=millis();	f=0;	}
			return true;
			}
		return false;
	}

	void decayColour32_t::setFPS(int newFPS) {
		_fps = constrain(newFPS, 10, 120);
		f=0; _time=millis();
		// re-calc tick time steps
		for (int n=1; n<4; n++) {	
			_tickTimeSteps[n-1] = ((n*1000)/_fps) +0.5;	}
		updateChannels();
	}

	void decayColour32_t::updateChannels() {
		for (int ch = 0; ch<4; ch++){
			_updateChannel(ch);
		}
	}

	void decayColour32_t::setDecaytime(int _time, uint8_t channel) {
		/* Sets decay time for channel	:	0/ 1/ 2/ 3/ 	4 / 	 5
																aka	:	B/ G/ R/ W/ BGR	/ BGRW 			*/
		int ch=0;
		if (channel == 4) channel = 2;
		else if (channel > 4) channel = 3;
		else ch = channel;
		for (; ch <= channel; ch++){
			_indvlChannelDecayTime[ch] = _time;
			_updateChannel(ch);
		}
	}

	void decayColour32_t::_updateChannel(int Channel)	{
		dChannel[Channel].calcLinearBrightnessLevels(_indvlChannelDecayTime[Channel], _fps);
	}

	uint32_t decayColour32_t::Decay(uint32_t Pixel, uint32_t baseline) {
		byte* const blbyte = (byte*)&baseline;
		byte* const Pixbyte = (byte*)&Pixel;
		for (int ch=0; ch<4; ch++) {
			int T;
			bool H = Pixbyte[ch] > blbyte[ch];
			bool L = Pixbyte[ch] < blbyte[ch];
			if (H) {
				// needed a correction here with max(0,...) because rollover was still occuring. 
				T = max(0x00, l_table[Pixbyte[ch]] -dChannel[ch].LBLpFrame[f]);
				if (false && ch==3) {
					Serial.print("\t> ");
					Serial.print(Pixbyte[ch],HEX);
					Serial.print(", ");
					Serial.print(blbyte[ch],HEX);
					Serial.print(", ");
					Serial.print(dChannel[ch].LBLpFrame[f],HEX);
					Serial.print(", ");
					Serial.println(T,HEX);
				}
				T = max(g_table[T], blbyte[ch]);
				// random here is needed to push past the 1 -> 0 issue when dChannel.LBLpFrame 
				// values are too small. It works out to be rather dithering. 
				if (f==2 && T <=2) T=random(0,3);
			}
			else if (L) {
				T = min(0xff, l_table[Pixbyte[ch]] +dChannel[ch].LBLpFrame[f]);
				if (false && ch==3) {
					Serial.print("\t> ");
					Serial.print(Pixbyte[ch],HEX);
					Serial.print(", ");
					Serial.print(blbyte[ch],HEX);
					Serial.print(", ");
					Serial.print(dChannel[ch].LBLpFrame[f],HEX);
					Serial.print(", ");
					Serial.println(T,HEX);
				}
				T = min(g_table[T], blbyte[ch]);
			}
			else T = blbyte[ch];
			Pixbyte[ch] = (byte)T;
		}
		if (++f >2) f=0;
		return Pixel;
	}



struct ddd_t {
	// i think everything returns +/- deg°
	int _orientation[3] {0};
	
	bool operator==(const ddd_t& R) const {
		return (	(R._orientation[0] == ddd_t::_orientation[0]) &&
							(R._orientation[1] == ddd_t::_orientation[1]) &&
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
	int nearby(ddd_t other) {
		return sqrt(sq(this->p() - other.p()) + sq(this->y() - other.y()));
	}
	int compareRoll(ddd_t other) {
		return abs(this->r() - other.r());
	}
};

struct commsMode_t { 
	uint8_t bitMap8{};
	enum mod: byte {
		WiFi=0, STA, AP, mDNS, WS, RemoteXY, BT, leBT,  	//	bits  0b00101011
		offline=0xf0, none=0xff,													//	functions
	};
	bool set(mod M) {
		switch (M) {
			case offline	:	_clr(WiFi); 			//						break;
			default 			:	return false; 								break;
			case STA			:	_clr(AP);			_set(WiFi);			break;
			case AP 			:	_clr(STA);		_set(WiFi);			break;
			case mDNS			:	_set(WiFi);										break;
			case RemoteXY	:	_clr(WS);											break;
			case WS				:	_clr(RemoteXY);								break;
			case BT 			:	_clr(leBT);		_clr(WiFi);			break;
			case leBT			:	_clr(BT);			_clr(WiFi);			break;
			case WiFi			:					 											break;
		}
		_set(M);		//	<-- 'fall-through' default switch action.
		return get(M);
	}
	bool get(mod M) {
		return bitRead(bitMap8,int(M));
	}
	bool get(mod M, mod N) {
		return get(M) && get(N);
	}
	
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
	uint16_t bitMap16{3};		//	0000 0011 : start, baseline-decay
	animationDirection Dir;
	void set(illumination Ac) {
		switch (Ac) {
			case decay  		:	_set(Ac);	_clr(radiate);	break;
			case radiate		:	_set(Ac);	_clr(decay);		break;
			case _start			:	_set(Ac);									break;
			case _stop			:	_clr(_start);							break;
			case sparkle 		:	_toggle(Ac);							break;
			default 				:	_clearExclusives(Ac);			break;
		}
	}
	bool directionIs(animationDirection Anim) {
		return (Dir==Anim);
	}
	animationDirection direction() {
		return Dir;
	}
	void newDirection(animationDirection Anim) {
		Dir = Anim;
	}
	void flipDirection() {
		switch (Dir) {
			case  _proximal	: Dir = _distal; 		break;
			case  _distal		: Dir = _proximal;	break;
			case  _lateral	: Dir = _medial;		break;
			case  _medial		: Dir = _lateral;		break;
			default: 		break;
		}
	}
	bool get(illumination Ac) {
		bool result{false};
		switch (Ac) {
			case _stop			:	result = !_get(Ac);	break;
			case _start			:	;		//	intentional fall through
			default 				:	result = _get(Ac);	break;
		}
		return result;
	}
	
	bool _get(illumination B) {
		return bitRead(bitMap16, int(B&0x0f));
	}
	void _set(illumination B) {
		bitSet(bitMap16, int(B&0x0f));
	}
	void _clr(illumination B) {
		bitClear(bitMap16, int(B&0x0f));
	}
	bool _toggle(illumination Ac) {
		bitWrite(bitMap16, int(Ac&0x0f), !_get(Ac));
		return _get(Ac);
	}
	void _clearExclusives(illumination bitParameter=_stop) {	
		const uint16_t exclusivesBitmap = 0x03;		//	0's are the exclusives
		if (bitParameter > _start && bitParameter < _stop) {		//	0's are the exclusives
			bitMap16 &= exclusivesBitmap;
			bitSet(bitMap16, int(bitParameter));
		}
	}
};


int _reverseLookupLoop(byte lookup, int j) {
	while (lookup < g_table[j]) { j--; }
	return j;		//	returns index of value in array closest to lookup
}

byte _findLinearIndex(byte lookup) {
	/* quick determine which third lookup value can be found, now we only need to 
		compare up to 85 values, rather than 255. round 1, is lookup in the early third?
		round 2, is lookup in the 2nd third? round 3, late third? search everything from 
		the last value down. round 4 is an error condition, returns 0.  */
	int _thirdPlace = 85;
	int LI = g_table[_thirdPlace]; 
	while (_thirdPlace < 256 && lookup >= LI) {
		_thirdPlace+=85;
		if (_thirdPlace > 255) {
			LI = 0; return LI; break;	}
		LI = g_table[_thirdPlace];
	}		//	END loop
	LI = _reverseLookupLoop(lookup, _thirdPlace);
	LI = (byte)min(255,max(0,LI));		//	ensure LI stays within byte range.
	return LI;
}

byte gamma8(byte linear) {
	/*
			if using : const uint8_t PROGMEM gamma8[]  = {...};
			use: return pgm_read_byte(&gamma8[linear]);
	*/
	return g_table[linear];
}

// attempts to estimate what the linear value was, rather than use the lookup table
byte deGamma(byte gammaValue) {
	int Lin=0;		//		index of g_table[], and our estimated original linear value
	int n=0;			//		number of times (if any) Val appears in the table
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






class k_colour {
		/* Kelvin Temperature to RGB | Credit to Tanner Helland for the base algorithm
		Port to C++ for Unreal Engine by Jorge Valle Hurtado - byValle */
	// Temperature input in Kelvin valid in the range 1000 K to 40000 K. White light = 6500K
	int Temperature_K;

	/* 	Estimate, or lookup, the colour temp of the White led component of your RGBW leds
				2700k		Warm
				3000k		Warm White
				4000k		Cool White
				5000k		Daylight
				6000k		Cool Daylight
				7000k		Cold White 			*/
	int NeoPixel_whiteChannel_temp = 3500;
	
	// RGB components
	float Red_f;
	float Green_f;
	float Blue_f;

	// Temperature divided by 100 for calculations
	float temp_k; // (Temperature_K / 100);
	
	// Final result as an RGBW NeoPixel style Color	ie: 0xwwrrggbb 
	uint32_t NEOPIX_col32;
	uint8_t _chanByte[4];	
public:
	uint32_t Calculate(int Temperature_K = 6500) {
		temp_k = ((Temperature_K)+(5000-NeoPixel_whiteChannel_temp)) / 100;
		// WHITE
		_chanByte[3] = 55;

		// RED
		if (temp_k <= 66)	Red_f = 255;
		else Red_f = 329.698727446 * pow(temp_k - 60, -0.1332047592);
		_chanByte[2] = constrain(Red_f,0,255);

		// GREEN
			if (temp_k <= 66)	Green_f = 99.4708025861 * log(temp_k) - 161.1195681661;
			else	Green_f = 288.1221695283 * pow(temp_k - 60, -0.0755148492);
			_chanByte[1] = constrain(Green_f,0,255);

		// BLUE
			if (temp_k >= 66)	Blue_f = 255;
			else	{
				if (temp_k <= 19)	Blue_f = 0;
				else	Blue_f = 138.5177312231 * log(temp_k - 10) - 305.0447927307;
			}
			_chanByte[0] = constrain(Blue_f,0,255);

		// FINAL RESULT AS ....
			byte* neo_ptr = (byte*)&NEOPIX_col32;
			neo_ptr[0] = _chanByte[0];		//	blue
			neo_ptr[1] = _chanByte[1];		//	green
			neo_ptr[2] = _chanByte[2];		//	red
			neo_ptr[3] = _chanByte[3];		//	white

			Serial.printf("%iK\tg.%2x r.%2x b.%2x w.%2x - NEO_PIX memory format: %#010x\n", Temperature_K, _chanByte[1], _chanByte[2], _chanByte[0], _chanByte[3], NEOPIX_col32);
	return NEOPIX_col32;
	}
};	//	END class k_colour



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






#endif 		//		__STRUCTS_H__
