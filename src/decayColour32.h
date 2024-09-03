
#ifndef __DECAYCOLOUR32__
#define __DECAYCOLOUR32__

#include <Arduino.h>

extern const uint8_t g_Table[];
extern const uint8_t l_Table[];

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

class decayColour32 {
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
		int _fps{48};
		uint32_t _time{0};
		int _tickTimeSteps[3];
		byte LiteralTimeFrame{0};
		void _updateChannel(int ) ;
};		//		END class decay_colour32_t
	void decayColour32::Begin(int fps, int decayTime) {
		setFPS(fps);
		setDecaytime(decayTime, 5);
	}
/*		This tick is different than timeout_t.  Since we're ticking based off
	fps/1000, rather than X ms., we will never have accurate integral incremental steps. So
	a needless way to improve this is to calculate 2 additional ms tick steps. Tick()
	now compares millis() - tickTimeStep(frame) against the last stored millis(). After the 3rd 
	step (frame==2) a fresh millis() is stored in _time. This prevents using floating point 
	and rounding errors, improving tick accuracy.  This is notably observed when using high 
	fps values (60+) and high decay times (>2000ms). This correction measurably improves
	tick accuracy by 2ms / 100ms (2%) without the need for constant floating point use.
*/
	bool decayColour32::Tick() {
		if ((_fps > 5) && (millis() - _tickTimeSteps[LiteralTimeFrame]) >= _time) {
			if (++LiteralTimeFrame > 2) {	_time=millis();	LiteralTimeFrame=0;	}
			return true;
		}
		return false;
	}
	void decayColour32::setFPS(int newFPS) {
		_fps = constrain(newFPS, 10, 120);
		LiteralTimeFrame=0; _time=millis();
		// re-calc tick time steps
		for (int n=1; n<4; n++) {	
			_tickTimeSteps[n-1] = ((n*1000)/_fps) +0.5;	}
		updateChannels();
	}
	void decayColour32::updateChannels() {
		for (int ch = 0; ch<4; ch++){
			_updateChannel(ch);
		}
	}
	void decayColour32::setDecaytime(int _time, uint8_t channel) {
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
	void decayColour32::_updateChannel(int Channel)	{
		dChannel[Channel].calcLinearBrightnessLevels(_indvlChannelDecayTime[Channel], _fps);
	}
	uint32_t decayColour32::Decay(uint32_t Pixel, uint32_t baseline) {
		byte* const blbyte = (byte*)&baseline;
		byte* const Pixbyte = (byte*)&Pixel;
		for (int ch=0; ch<4; ch++) {
			int T;
			bool H = Pixbyte[ch] > blbyte[ch];
			bool L = Pixbyte[ch] < blbyte[ch];
			if (H) {
				// needed a correction here with max(0,...) because rollover was still occuring. 
				T = max(0x00, l_Table[Pixbyte[ch]] -dChannel[ch].LBLpFrame[LiteralTimeFrame]);
				if (false && ch==3) {
					Serial.print("\t> ");
					Serial.print(Pixbyte[ch],HEX);
					Serial.print(", ");
					Serial.print(blbyte[ch],HEX);
					Serial.print(", ");
					Serial.print(dChannel[ch].LBLpFrame[LiteralTimeFrame],HEX);
					Serial.print(", ");
					Serial.println(T,HEX);
				}
				T = max(g_Table[T], blbyte[ch]);
				// random here is needed to push past the 1 -> 0 issue when dChannel.LBLpFrame 
				// values are too small. It works out to be rather dithering. 
				if (LiteralTimeFrame==2 && T <=2) T=random(0,3);
			}
			else if (L) {
				T = min(0xff, l_Table[Pixbyte[ch]] +dChannel[ch].LBLpFrame[LiteralTimeFrame]);
				if (false && ch==3) {
					Serial.print("\t> ");
					Serial.print(Pixbyte[ch],HEX);
					Serial.print(", ");
					Serial.print(blbyte[ch],HEX);
					Serial.print(", ");
					Serial.print(dChannel[ch].LBLpFrame[LiteralTimeFrame],HEX);
					Serial.print(", ");
					Serial.println(T,HEX);
				}
				T = min(g_Table[T], blbyte[ch]);
			}
			else T = blbyte[ch];
			Pixbyte[ch] = (byte)T;
		}
		if (++LiteralTimeFrame >2) LiteralTimeFrame=0;
		return Pixel;
	}

#endif
