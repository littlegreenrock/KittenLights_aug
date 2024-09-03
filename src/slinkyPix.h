
#ifndef __SLINKYPIX__
#define __SLINKYPIX__

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel* LEDS;

enum _limits : int8_t {
	none=0, fwdBounce=1, revBounce=-1, onlyUp=2, onlyDown=-2,
};		//	end enum _limits

/*
	strip order 				(1,2,3,4)		(r,g,b,w)
	uint32_t						0x04010203	0xwwrrggbb
	getPixelColor(0)		0x04010203	0xwwrrggbb
	neo_array_ptr				2134				grbw
	(byte*)&uint32_t[i]	3214				bgrw  (rev. 4123)
			•	ergo,		(B*)32[i] -> _ptr[0..3]		= 1,2,0,3
			•	and,		_ptr[i] 	-> (B*)32[0..3]	= 2,1,3,4
	*/


class slinkyPix {
	private:
		byte 			_ledNo;						//	g r b w 
		byte 			_limitHigh;
		byte 			_limitLow;

	public:
		slinkyPix() ;
		slinkyPix(int ) ;
		slinkyPix(int , unsigned int , unsigned int ) ;
		~slinkyPix() ;
		uint32_t 	colorCode;				//	0xwwrrggbb
		_limits 	setting{none};
		void set(uint32_t ) ;
		void set(_limits ) ;
		void set(uint32_t , _limits ) ;
		void setLimit(uint8_t , uint8_t ) ;
		void r32wPtr(uint32_t&, uint8_t** ) ;
		void rPtrw32(uint32_t&, uint8_t** ) ;
		void pos(byte ) ;
		byte where() ;
		void poke() ;
		void pick() ;
		void pick(byte ) ;
		void move(int8_t ) ;
		void moveUp(uint8_t ) ;
		void moveDown(uint8_t ) ;

};		//	END struct slinkyPix

slinkyPix::slinkyPix(){};
slinkyPix::slinkyPix(int ledPixel) : _ledNo{ledPixel} {
	setLimit(0x0, 0xff);	//	if you don't tell me, I won't know. 
};

slinkyPix::slinkyPix(int ledPixel, unsigned int lower_pixel_limit, unsigned int upper_pixel_limit) :
	_ledNo{(byte)ledPixel}, _limitLow{(byte)lower_pixel_limit}, _limitHigh{(byte)upper_pixel_limit} {};

slinkyPix::~slinkyPix(){};

void slinkyPix::set(uint32_t Colour32) {
	colorCode = Colour32;
}

void slinkyPix::r32wPtr(uint32_t &col32, uint8_t** neo_array_ptr) {
	int r32[]{1,2,0,3};
	int wPt{0};
	uint8_t* col8 = (uint8_t*)col32;
	for (;wPt<4;wPt++) {
		*neo_array_ptr[wPt] = col8[r32[wPt]];		}
}
void slinkyPix::rPtrw32(uint32_t &col32, uint8_t** neo_array_ptr) {
	int rPt[]{2,1,3,4};
	int w32{0};
	uint8_t* col8 = (uint8_t*)col32;
	for (;w32<4;w32++) {
		col8[w32] = *neo_array_ptr[rPt[w32]];		}
}


byte slinkyPix::where() {
	return _ledNo;	}
void slinkyPix::pos(byte led) {
	_ledNo = led;
	// neo_array_ptr = LEDS->getPixels() + (led * 4);
}
void slinkyPix::poke() {
	LEDS->setPixelColor(this->_ledNo, this->colorCode);
	// alt.
	// for (int i=0; i<4;i++) {
	// 	neo_array_ptr[i] = (byte)(colorCode<<(i*4)&0xff);		}
}
void slinkyPix::pick(byte led) {
	colorCode = LEDS->getPixelColor(led);
	// uint8_t* _array_ptr = LEDS->getPixels() + (led * 4);
	// uint8_t* cCode = (uint8_t*) &colorCode;
	// for (int i=0; i<4;i++) {
	// 	cCode[i] = _array_ptr[i];		}
}
void slinkyPix::pick() {
	pick(this->_ledNo);
	// alt.
	// uint8_t* cCode = (uint8_t*) &colorCode;
	// for (int i=0; i<4;i++) {
	// 	cCode[i] = this->neo_array_ptr[i];		}
}
void slinkyPix::move(int8_t x=1) {
	if (x==0) return;
	switch (setting) {
		case none : {
			x<0? moveDown(x):moveUp(x);		break; }
		case fwdBounce : 
		case onlyUp :	moveUp(x); break;
		case onlyDown :
		case revBounce : moveDown(x); break;
		default : return;
	};		//	END switch()
}
void slinkyPix::moveUp(uint8_t x=1) {
	int _calc = (int)_ledNo + x;
	if (setting==none) _ledNo = min(_calc, (int)_limitHigh);
	else if (setting==fwdBounce && _calc >= _limitHigh) {
		_ledNo = _limitHigh-1;
		setting = revBounce;
	}
	else if (setting==onlyUp && _calc >= _limitHigh)	_ledNo = _limitLow;
	else _ledNo = _calc;
}
void slinkyPix::moveDown(uint8_t x=1) {
	int _calc = (int)_ledNo - x;
	if (setting==none) _ledNo = max(_calc, (int)_limitLow);
	else if (setting==revBounce && _calc < _limitLow) {
		_ledNo = _limitLow;
		setting = fwdBounce;
	}
	else if (setting==onlyDown && _calc < _limitLow)	_ledNo = _limitHigh-1;
	else _ledNo = _calc;
	}
void slinkyPix::set(_limits Auto) {
	setting = Auto;		}
void slinkyPix::set(uint32_t Colour32, _limits Auto) {
	set(Colour32);
	set(Auto);
}
void slinkyPix::setLimit(uint8_t low, uint8_t high) {
	// as you should expect, limitLow is inclusive, but limitHigh is exclusive.
	_limitLow = low;
	_limitHigh = high;
}


#endif		//	__SLINKYPIX__

