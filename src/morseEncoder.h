#ifndef MORSEENCFLASH_H
#define MORSEENCFLASH_H
/*! Saturday 03 August 2024 - Morse Encoder:Flash
    @brief  Class which provides a bool for blinking a light based 
	on the morse code for a few supplied letters.
*/
#include <Arduino.h>

struct MCchar_t {
	unsigned char encoded;
	int len() {
		return (int)(encoded & 0b00000111);				//	MC len encoded in bits(0:2)
	}
	char getElement(int i) {
		char res = '\0';
		if (i<len()) {
			res = (bitRead(encoded,3+i)?'-':'.');		//	MC element bits begin at bit(3)
		}
		return res;
	}
};

class morseEncoder	{
private:
	uint32_t _timeout;
	bool _ON;
	int _dUnit;					//	element timing in ms 
	int _fUnit;					//	Farnsworth gap timing in ms
	int _l{0};
	int _e{0};
	unsigned char toEncode(char); 
	void _doTiming(int _p, bool _state);
	void _dot();
	void _dash();
	void _elementSep();
	void _characterSep();
	void _wordSep();
	bool _getState(MCchar_t*);
	int _calculateTiming_ms(int); 

public:
	// char elementbuffer[6];
	void set_dWPM(int);		//	set new wpm value using PARIS method
	void set_fWPM(int);		//	set custom Farnsworth wpm value.
	bool iterate(const char* array);

	~morseEncoder(void);
	morseEncoder();
	morseEncoder(int wpm);
	morseEncoder(int wpm, int Farnsworth);


};		//	END class

#endif		//	__MORSEENCFLASH_H__
;
