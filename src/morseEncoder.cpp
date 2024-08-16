/*
		morseEncoder. 2024 August 03.  lgr.
*/
#include "morseEncoder.h"
#include <Arduino.h>

morseEncoder::~morseEncoder(void) {
	//	Object is being deleted
}
morseEncoder::morseEncoder() {
  set_dWPM(9);
	set_fWPM(18);
}
morseEncoder::morseEncoder(int wpm) {
	set_dWPM(wpm);
	set_fWPM(18);
}
morseEncoder::morseEncoder(int wpm, int Farnsworth) {
  set_dWPM(wpm);
	set_fWPM(Farnsworth);
}

unsigned char morseEncoder::toEncode(char C) {
	/* The Encoding Method of Morse to 8-bit Binary:
		0bdddddlll 	: 0b = binary byte notation
					: ddddd = places for /./ and /-/ represented as 0/1 
					: lll = 3-bit Dec-Bin number representing the length of the code in elements.
					: the binary is read from bit 0 to bit 7.
			examples: J	.---	4 elements length = 0b100
					: /./ are 0
					: /-/ are 1
					: .--- = 0111 however we write into binary from right to left; 1110
					: the last bit (bit 7) is unused and unread due to length being 4
					: combined, 	J	.---	0b01110100
			Now a 1-byte latin character can be represented as a 1-byte MC character.
		*/
	if (isUpperCase(C)) C+=0x20;
	switch (C) {
		case 'a':	return	0b00010010;		//	.-
		case 'b':	return	0b00001100;		//	-...
		case 'c':	return	0b00101100;		//	-.-.
		case 'd':	return	0b00001011;		//	-..
		case 'e':	return	0b00000001;		//	.
		case 'f':	return	0b00100100;		//	..-.
		case 'g':	return	0b00011011;		//	--.
		case 'h':	return	0b00000100;		//	....
		case 'i':	return	0b00000010;		//	..
		case 'j':	return	0b01110100;		//	.---
		case 'k':	return	0b00101011;		//	-.-
		case 'l':	return	0b00010100;		//	.-..
		case 'm':	return	0b00011010;		//	--
		case 'n':	return	0b00001010;		//	-.
		case 'o':	return	0b00111011;		//	---
		case 'p':	return	0b00110100;		//	.--.
		case 'q':	return	0b01011100;		//	--.-
		case 'r':	return	0b00010011;		//	.-.
		case 's':	return	0b00000011;		//	...
		case 't':	return	0b00001001;		//	-
		case 'u':	return	0b00100011;		//	..-
		case 'v':	return	0b01000100;		//	...-
		case 'w':	return	0b00110011;		//	.--
		case 'x':	return	0b01001100;		//	-..-
		case 'y':	return	0b01101100;		//	-.--
		case 'z':	return	0b00011100;		//	--..
		case '0':	return	0b11111101;		//	-----
		case '1':	return	0b11110101;		//	.----
		case '2':	return	0b11100101;		//	..---
		case '3':	return	0b11000101;		//	...--
		case '4':	return	0b10000101;		//	....-
		case '5':	return	0b00000101;		//	.....
		case '6':	return	0b00001101;		//	-....
		case '7':	return	0b00011101;		//	--...
		case '8':	return	0b00111101;		//	---..
		case '9':	return	0b01111101;		//	----.
		case ' ':	return	0b11111000;		//	' '
		case '#':	return	0b01010100;		//	.-.-	*notes
		case '@':	return	0b01100100;		//	..--	*notes
		default:	return	0x0;
	/* What I believe is a better system for numbers:
		* • have a code for "numbers begin", "#"
		* • and a code for "numbers end" or "resume alpha", "@"
		* • say, [.-.-] R+T or Roman-Type, written as '#' for numbers
		* • and [..--] U+T or Universal Type, written as '@' for resuming regular latin morse, if it's even necessary.
		* • these two codes are unused for std. intl. MC
		* • Then; digits 1-0 are tapped out like roman numerals, ie:
		* 	• 0 = X   	= /--/			= M
		* 	• 1 = I   	= /./				= E
		* 	• 2 = II  	= /../			= I
		* 	• 3 = III 	= /.../			= S
		* 	• 4 = IV  	= /.-/			= A
		* 	• 5 = V   	= /-/				= T
		* 	• 6 = VI  	= /-./			= N
		* 	• 7 = VII 	= /-../			= D
		* 	• 8 = VIII	= /-.../		= B
		* 	• 9 = IX  	= /.--/			= W
		* 	• 0 being represented by roman numeral X, for 10, as the romans never had a symbol to represent 0
		*		and it's simple and effective, and close to existing '0'
		*		-- . .. ... .- - -. -.. -... .-- --	meisatndbw	0123456789
		*	•	Memetic: "ME IS AT eND BoW" (0-9) or "Elle IS A TeeNageD Blonde Woman" (1-0)
		*	• Now an operator has 2 new 4-element codes to learn, but can give up 10 5-element codes.
		*	• Numbers become faster to enter and receive. Translatting letters into numbers is simpler for the transcriing operator
		*	• The current pips-counting method is now as convenient as counting with the fingers on one hand.
		*		--Jade
	   */
		
	}
}

void morseEncoder::_doTiming(int _p, bool _state){
	/*	Farnsworth timing is about sending the characters at a pre-determined
	and faster speed than the actual wpm speed. The character of dits and dahs
	is sent like we were transmitting at 20 wpm, but the gaps between each 
	character, and the gaps between each word, are lengthened to achieve the 
	desired wpm rate. 
			Why do this?:
	It standardizes each character, while transmitting at slower speeds.
	*/
	//  test for a between-char/word gap
	if (_state==false && _p>2)	{
		// use given wpm (slower, or longer delay)
		_timeout = millis() + _p * _dUnit;
	}
	else {
		// use Farnsworth timing (faster, standardised character sending)
		_timeout = millis() + _p * _fUnit;
	}
	_ON = _state;
}
void morseEncoder::_dot() {
	_doTiming(1, true);
}
void morseEncoder::_dash() {
	_doTiming(3, true);
}
void morseEncoder::_elementSep() {
	_doTiming(1, false);
}
void morseEncoder::_characterSep() {
	_doTiming(3, false);
	_l++; _e=0;
}
void morseEncoder::_wordSep() {
	_doTiming(7, false);
	_l++; _e=0;
}

	
bool morseEncoder::_getState(MCchar_t* mChar) {
	/* Takes mChar_t pointer 
	 */
	if (millis() > _timeout) {
		if (_ON) {
			if (++_e < mChar->len()) _elementSep();		//	only time element counter is advanced
			else {		//	( _e == mChar.len() )
				_characterSep();
			}
		}		//	END if(_ON)
		else {	//	( _ON==false )
			if (_e < mChar->len()) {
				//	action the next element
				if (mChar->getElement(_e)=='.') _dot();
				else if (mChar->getElement(_e)=='-') _dash();
				else {
					//	This could only mean that the toEncode binary for this character is corrupt/wrong.
					Serial.println("ERR: _getState(), element length wrong for encoded.char: %s");
					_wordSep();		//	limp by recovery
				}
			}	//	END if ( j<M.len() )
			else if (_e > mChar->len()) {
				//	TODO: if multi character word, the next character would start now
				_wordSep();
			}
			else if (_e == mChar->len()) {
				// 0xf8 = the code for a 'space' character.
				if (mChar->encoded != 0xf8) 
					Serial.println("ERR: _getState(), _e==mChar.len() fall through.");
				_wordSep();
			}
		}
	}
	return _ON;		//	<-- this is what the state of the LED should be. 
}

int morseEncoder::_calculateTiming_ms(int wpm) {
	return 1200/(max(1,min(18,wpm)));
}
void  morseEncoder::set_dWPM(int wpm) {
	// the fUnit must remain less than or equal to the dUnit
	_dUnit = _calculateTiming_ms(wpm);
	if (_dUnit < _fUnit) set_fWPM(wpm);
}
void  morseEncoder::set_fWPM(int wpm) {
	// the fUnit cannot be made larger than the dUnit
	_fUnit = min(_calculateTiming_ms(wpm),_dUnit);
	// _fUnit = _calculateTiming_ms(wpm);
}

bool morseEncoder::iterate(const char* array) {
	MCchar_t MC;
	MC.encoded = toEncode(array[_l]);
	bool B = _getState(&MC);
	if (array[_l] == '\0') {
		_wordSep();
		_l=0;
	}
	return B;
}

;		//	eof