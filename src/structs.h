/*	enums and structs 	------------------------------------------------	*/
#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <Arduino.h>
#include "timeout.h"

//	0xWWRRGGBB  = NEO_GRBW

const uint8_t DEBOUNCE_DELAY = 20;		//	milliseconds
byte gamma8(byte);
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
const uint8_t l_Table[256] = {			//	a2.2(hex)
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



const uint8_t Dawn[][3] = {
	// idx : 0 - 70.  Order = r g b.  Linear bright levels.
		{ 0x51,	0x26,	0x24, },
		{ 0x30,	0x7E,	0x9A, },
		{ 0x00,	0x05,	0x25, },
		{ 0x27,	0x26,	0x66, },
		{ 0x20,	0x25,	0x6A, },
		{ 0x3F,	0x32,	0x80, },
		{ 0x65,	0x52,	0x8D, },
		{ 0xC3,	0x76,	0xA9, },
		{ 0xFA,	0xD7,	0x3C, },	//
		{ 0xFA,	0xD0,	0x31, },
		{ 0xFC,	0xC9,	0x36, },
		{ 0xFC,	0xC4,	0x34, },
		{ 0xFC,	0xBC,	0x30, },
		{ 0xF9,	0xA4,	0x20, },
		{ 0xFB,	0x53,	0x1D, },	//
		{ 0xFB,	0x69,	0x17, },
		{ 0xFB,	0x6A,	0x1A, },
		{ 0xFC,	0x61,	0x0C, },
		{ 0xF7,	0x20,	0x05, },
		{ 0xCD,	0x1B,	0x15, },
		{ 0xD6,	0x1B,	0x16, },
		{ 0xD6,	0x1B,	0x16, },
		{ 0xE4,	0x1C,	0x0E, },
		{ 0xF8,	0x26,	0x08, },
		{ 0xF2,	0x21,	0x08, },
		{ 0xEA,	0x1D,	0x07, },
		{ 0xE8,	0x1F,	0x06, },
		{ 0xF8,	0x27,	0x0F, },
		{ 0xE8,	0x1B,	0x06, },
		{ 0xAD,	0x26,	0x17, },
		{ 0xD3,	0x24,	0x0F, },
		{ 0xEA,	0x2D,	0x08, },
		{ 0xFB,	0x38,	0x03, },
		{ 0xFD,	0x67,	0x01, },	//
		{ 0xFD,	0x8C,	0x00, },
		{ 0xFD,	0x7D,	0x00, },
		{ 0xFE,	0x97,	0x22, },
		{ 0xF8,	0x6B,	0x24, },
		{ 0xFC,	0x7C,	0x2C, },
		{ 0xFC,	0x7D,	0x2F, },
		{ 0xFD,	0xA2,	0x4C, },
		{ 0xF8,	0x88,	0x50, },
		{ 0xFB,	0xA6,	0x74, },
		{ 0xFB,	0xA9,	0x7E, },
		{ 0xD7,	0x9D,	0x8C, },
		{ 0xD4,	0xA4,	0x9B, },
		{ 0xD4,	0xAA,	0xA4, },
		{ 0xC2,	0xA6,	0xA2, },
		{ 0xB2,	0xA2,	0xA3, },
		{ 0xC3,	0xAC,	0xAE, },
		{ 0xD9,	0xB3,	0xB1, },
		{ 0xC0,	0xAB,	0xAC, },
		{ 0xBE,	0xA9,	0xAB, },
		{ 0xA8,	0xA3,	0xAA, },
		{ 0xA5,	0xA3,	0xAE, },
		{ 0x99,	0x9C,	0xAA, },
		{ 0x92,	0x9D,	0xAC, },
		{ 0x91,	0x9B,	0xAB, },
		{ 0x8C,	0x99,	0xA8, },
		{ 0x84,	0x97,	0xA9, },
		{ 0x70,	0x92,	0xA9, },
		{ 0x73,	0x93,	0xAA, },
		{ 0x67,	0x8E,	0xA7, },
		{ 0x5D,	0x8A,	0xA3, },
		{ 0x5A,	0x89,	0xA1, },
		{ 0x4F,	0x85,	0xA3, },
		{ 0x52,	0x84,	0xA2, },
		{ 0x4A,	0x83,	0xA1, },
		{ 0x39,	0x7C,	0x9B, },
		{ 0x32,	0x7C,	0x9C, },
		{ 0x2D,	0x7A,	0x9A, },
};



class baselinebyte {
public:
	uint32_t color32;
	byte* const colB = (byte*)&color32;
	void warmer(){
		// degamma
		// determine K
		// inc K
		// re-calc lin
		// gamma
	}
	void cooler(){}
	void lessTent(){
		// use NEO_PIXEL brightness() function, or, 
		// de-gamma
		// dec lin
		// gamma
	}
	void moreTent(){}

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
		return l_Table[Val];
	}
	byte Lin(int lbl, bool Higher) {
		return g_Table[(byte(int(l_Table[Val] + (Higher?-lbl:lbl))))];
	}
	byte Gam() {
		return g_Table[Val];
	}
	byte pnt() {
		return Val;
	}
private:
	byte Val;
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



struct q_button_t {
	//	The Physical Button 		---------------------------------------
	q_button_t(uint8 assignedPin, bool button_is_wired_as_PULLUP) : pin{assignedPin}, _digitalRead_PRESSED{!button_is_wired_as_PULLUP} {};
	
	const uint8_t pin;
	uint16_t _state;
	const bool _digitalRead_PRESSED;
	
	void read() {
		_state<<=1;
		bitWrite(_state, 0, digitalRead(pin));
	}

	bool pressed() {
		return ((_digitalRead_PRESSED==HIGH && _state == 0x7fff) || 
			(_digitalRead_PRESSED==LOW  && _state == 0x8000));
	}
	
	bool released() {
		return ((_digitalRead_PRESSED==HIGH && _state == 0xfffe) || 
				(_digitalRead_PRESSED==LOW  && _state == 0x0001));
	}
	
	bool held() {
		return ((_digitalRead_PRESSED==HIGH && _state == 0xffff) || 
			(_digitalRead_PRESSED==LOW  && _state == 0x0000));
	}

	bool idle() {
		return ((_digitalRead_PRESSED==HIGH && _state == 0x0000) || 
			(_digitalRead_PRESSED==LOW  && _state == 0xffff));
	}
};		//	END q_button_t



struct Button_t {
	// Definition of the Physical Button 		---------------------------------------
	Button_t(uint8 assignedPin, bool button_is_wired_as_PULLUP) : pin{assignedPin}, theTruePressState{!button_is_wired_as_PULLUP} {};
	
	const uint8_t  pin;
	
	bool     lastReading{};
	uint32_t lastDebounceTime{};
	uint16_t _state{};
	const bool theTruePressState;
	
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



enum class ANIM: byte {
	// previous _mirror is replaced by _lateral/_medial
	_static, _proximal, _distal, _lateral, _medial,
};
enum class DISP: uint8_t {
	_start=0, decay,	//	toggle bits
	 radiate, sparkle, cometTail, larsonSeq, twinkle, festoon, LiveCursor,	//	exclusive bits
	_stop=0x80,				//	unmapped functions
};
enum class COM: byte {
		WiFi=0, STA, AP, mDNS, WS, RemoteXY, BT, leBT,  	//	bits  0b00101011
		offline=0xf0, none=0xff,													//	functions
	};



struct ddd_t {
private:
	int _orientation[3] {0};
public:
	enum axis : byte {
		// just an alias for P/Pitch to be used as array pointer.
		P=0, R=1, Y=2, Pitch=0, Roll=1, Yaw=2,
	};
	
	// i think everything returns +/- deg°
	bool operator==(const ddd_t& R) const {
		return (	(R._orientation[0] == ddd_t::_orientation[0]) &&
							(R._orientation[1] == ddd_t::_orientation[1]) &&
							(R._orientation[2] == ddd_t::_orientation[2]) )
							;}
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
private:
	uint8_t _bitMap8 {} ;
	bool _toggle(COM ) ;
	void _set(COM ) ;
	void _clr(COM ) ;

public:
	bool set(COM ) ;
	bool get(COM ) ;
	bool get(COM , COM ) ;

};		//	END struct commsMode_t

	bool commsMode_t::set(COM M) {
		switch (M) {
			case COM::offline		:	_clr(COM::WiFi); 			//						break;
			default 			:	return false; 							break;
			case COM::STA				:	_clr(COM::AP);				_set(COM::WiFi);	break;
			case COM::AP 				:	_clr(COM::STA);				_set(COM::WiFi);	break;
			case COM::mDNS			:	_set(COM::WiFi);			break;
			case COM::RemoteXY	:	_clr(COM::WS);				break;
			case COM::WS				:	_clr(COM::RemoteXY);	break;
			case COM::BT 				:	_clr(COM::leBT);			_clr(COM::WiFi);	break;
			case COM::leBT			:	_clr(COM::BT);				_clr(COM::WiFi);	break;
			case COM::WiFi			:					 							break;
		}
		_set(M);		//	<-- 'fall-through' default switch action.
		return get(M);
	}
	bool commsMode_t::get(COM M) {
		return bitRead(_bitMap8,int(M));
	}
	bool commsMode_t::get(COM M, COM N) {
		return get(M) && get(N);
	}
	bool commsMode_t::_toggle(COM B) {
		bitWrite(_bitMap8, int(B), !(bitRead(_bitMap8, int(B))));
		return bitRead(_bitMap8, int(B));
	}
	void commsMode_t::_set(COM B) {
		bitSet(_bitMap8, int(B));
	}
	void commsMode_t::_clr(COM B) {
		bitClear(_bitMap8, int(B));
	}



struct displayMode_t {
public:
	void set(DISP ) ;
	bool directionIs(ANIM ) ;
	ANIM directionIs() ;
	void set(ANIM ) ;
	void flipDirection() ;
	bool get(DISP ) ;

private:
	uint16_t _bitMap16 {3} ;		//	0000 0011 : start, baseline-decay
	ANIM _Dir ;
	bool _get(DISP ) ;
	void _set(DISP ) ;
	void _clr(DISP ) ;
	bool _toggle(DISP ) ;
	void _clearExclusives(DISP ) ;	


};		//		END struct displayMode_t

	void displayMode_t::set(DISP Ac) {
		switch (Ac) {
			case DISP::decay  	:	_set(Ac);	_clr(DISP::radiate);	break;
			case DISP::radiate	:	_set(Ac);	_clr(DISP::decay);		break;
			case DISP::_start		:	_set(Ac);												break;
			case DISP::_stop		:	_clr(DISP::_start);							break;
			case DISP::sparkle	:	_toggle(Ac);										break;
			default 						:	_clearExclusives(Ac);						break;
		}
	}
	bool displayMode_t::directionIs(ANIM Anim) {
		return (_Dir==Anim);
	}
	ANIM displayMode_t::directionIs() {
		return _Dir;
	}
	void displayMode_t::set(ANIM Anim) {
		_Dir = Anim;
	}
	void displayMode_t::flipDirection() {
		switch (_Dir) {
			case  ANIM::_proximal	: _Dir = ANIM::_distal; 		break;
			case  ANIM::_distal		: _Dir = ANIM::_proximal;	break;
			case  ANIM::_lateral	: _Dir = ANIM::_medial;		break;
			case  ANIM::_medial		: _Dir = ANIM::_lateral;		break;
			default: 		break;
		}
	}
	bool displayMode_t::get(DISP Ac) {
		bool result{false};
		switch (Ac) {
			case DISP::_stop			:	result = !_get(Ac);	break;
			case DISP::_start			:	;		//	intentional fall through
			default 				:	result = _get(Ac);	break;
		}
		return result;
	}
	bool displayMode_t::_get(DISP B) {
		return bitRead(_bitMap16, int(B)&(0x0f));
	}
	void displayMode_t::_set(DISP B) {
		bitSet(_bitMap16, int(B)&(0x0f));
	}
	void displayMode_t::_clr(DISP B) {
		bitClear(_bitMap16, int(B)&(0x0f));
	}
	bool displayMode_t::_toggle(DISP Ac) {
		bitWrite(_bitMap16, int(Ac)&(0x0f), !_get(Ac));
		return _get(Ac);
	}
	void displayMode_t::_clearExclusives(DISP bitParameter=DISP::_stop) {	
		const uint16_t exclusivesBitmap = 0x03;		//	0's are the exclusives
		if (bitParameter > DISP::_start && bitParameter < DISP::_stop) {		//	0's are the exclusives
			_bitMap16 &= exclusivesBitmap;
			bitSet(_bitMap16, int(bitParameter));
		}
	}



int _reverseLookupLoop(byte lookup, int j) {
	while (lookup < g_Table[j]) { j--; }
	return j;		//	returns index of value in array closest to lookup
}
byte _findLinearIndex(byte lookup) {
	/* quick determine which third lookup value can be found, now we only need to 
		compare up to 85 values, rather than 255. round 1, is lookup in the early third?
		round 2, is lookup in the 2nd third? round 3, late third? search everything from 
		the last value down. round 4 is an error condition, returns 0.  */
	int _thirdPlace = 85;
	int LI = g_Table[_thirdPlace]; 
	while (_thirdPlace < 256 && lookup >= LI) {
		_thirdPlace+=85;
		if (_thirdPlace > 255) {
			LI = 0; return LI; break;	}
		LI = g_Table[_thirdPlace];
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
	return g_Table[linear];
}
// attempts to estimate what the linear value was, rather than use the lookup table
byte deGamma(byte gammaValue) {
	int Lin=0;		//		index of g_Table[], and our estimated original linear value
	int n=0;			//		number of times (if any) Val appears in the table
	if (gammaValue >= 255) return g_Table[255];
	else {			
		while (gammaValue <= g_Table[Lin]) {
			if (g_Table[Lin++] == gammaValue) n++;
		}
	}
	if (n<3) return Lin-1;
	else return (byte)((Lin-n)+(n/2));
}
//	Generate an LED gamma-correction table for Arduino sketches, given any gamma factor.
//	2.8 is a common normal.  I personally like 2.2.
uint8_t gamma8Calc(const uint8_t* ColourByte, const float* G_value) {
	float PercentIntensity = (float)*ColourByte / (float)255.0;
	float Result = pow(PercentIntensity, *G_value) * 255.0 + 0.5;
	return (uint8_t)Result; }
void generateGammaByteTable(float gamma=2.8, bool outputInHexPlease=0) {
	char C;
	Serial.printf("const uint8_t gamma[] = {\t\t\\\\\tgamma factor %1.1f(%s)", gamma, outputInHexPlease?"hex":"dec");
	for(uint8_t i = 0; i <= 0xff; i++) {
		if(i > 0) Serial.print(", ");
		if((i & 0x0f) == 0) {
			C = (i>>4);
			C>0x9?C+=0x61:C+=0x30;
			Serial.printf("\t//\t%c\n", C);
		}
		Serial.printf(outputInHexPlease?"%#04x":"%3d", gamma8Calc(&i, &gamma));
	}
	Serial.println(" };");
}



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
