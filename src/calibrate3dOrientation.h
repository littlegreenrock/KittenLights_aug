

#ifndef CALIBRATE3DORIENTATION_H
#define CALIBRATE3DORIENTATION_H

// #include <Arduino.h>
/*! Friday 11 August 2024
    @brief  Class to handle calibrate3dOrientation
*/
class calibrate3dOrientation	{

	private:
		const int rMx=25;
		int _thisReading[3]{} ;
		int Rec[25][3]={0,0,0} ;
		int _r{0};
		int _Wiggle; 
		int errCone[3]={75, 75, 75};		//	+/- 7.5° to any given angle
		bool _lap{false};
		bool _closeEnough(int , int , int ) ;
	public:
		calibrate3dOrientation(){
			reset();
		};
		void reset() ;
		void newReading(float, float, float) ;
		bool logPRY(float, float, float) ;
		bool logPRY() ;
		int aim(float, float, float) ;
		int getRelativePitch(int , float ) ;
		int testStability(float , float , float ) ;

};				//	END class
	// - - - - - - -- - - - - - - -- -
	void calibrate3dOrientation::newReading(float P, float R, float Y) {
		// cnverts float(PI) to int(degrees*10^1) -1800 - 0000 - 1800°
		_thisReading[0] = P*573;
		_thisReading[1] = R*573;
		_thisReading[2] = Y*573;
	}
	void calibrate3dOrientation::reset() { 
		_r=0;
		_lap=false;
		_Wiggle=0;
		int i{0};
		while (i<3) {
			Rec[i][0]=0;
		}
	}
	bool calibrate3dOrientation::logPRY(float P, float R, float Y) {
		newReading(P,R,Y);
		return logPRY();
	}
	bool calibrate3dOrientation::logPRY() {
		if (_r<rMx)	{
			Rec[_r][0] = _thisReading[0];
			Rec[_r][1] = _thisReading[1];
			Rec[_r][2] = _thisReading[2];
			_r++;
		}		 
		return (_r<rMx);
	}
	
	bool calibrate3dOrientation::_closeEnough(int value, int range, int comparison) {
		//	is Value within Range of Comparison_value ?
		//	is 30 within 5 of 33 ? true, 28<30<38.
		return ((value-comparison+range)*(value-comparison-range) <= 0);
	}

	int calibrate3dOrientation::aim(float P, float R, float Y){
		newReading(P,R,Y);
		int pos=-1;
		int pos2=-2;
		for (int i=0; i<_r; i++) {
			if ((_closeEnough(_thisReading[2], errCone[2], Rec[i][2])) 
			&& (_closeEnough(_thisReading[0], errCone[0]*5, Rec[i][0])) )
			{
				pos2 = pos;
				pos = i;
			}
		}
		if (pos== -1) return pos;
		if (pos2== -1) return pos;
		if (pos2+1 == pos) return pos;
		return -1;
	}

	int calibrate3dOrientation::getRelativePitch(int R_val, float P) {
		return int(P*573) - Rec[R_val][0];
	}		
	int calibrate3dOrientation::testStability(float P, float R, float Y) {
		// Serial.print(P);
		// Serial.print(", ");
		// Serial.print(R);
		// Serial.print(", ");
		// Serial.println(Y);
		newReading(P, R, Y);

		_lap = logPRY()==false;
		if (_lap) {
			_r=0;			//	continuous loop
				int32_t Sum{0};
				int sd, Mean, SumSq{0};
				for (int i = 0; i< rMx; i++)	{	Sum += Rec[i][2];	}
				Mean = Sum / rMx;
				for (int i = 0; i< rMx; i++)	{	SumSq += pow(Rec[i][2] - Mean, 2);	}
				sd = (SumSq/(rMx-1)+0.6);
				_Wiggle=sd;
		}
	return _Wiggle;

	}

#endif		//	__CALIBRATE3DORIENTATION_H__

