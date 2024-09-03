#ifndef FOURCHANNELTRANSITION_H
#define FOURCHANNELTRANSITION_H

#include <Arduino.h>

/*! Friday 21 June 2024
    @brief  Class that plots a 3D vector from two given points in 3 plane cartesian.  Accomodates for an additional parameter of White LED intensity change, seperately.
	Imagine R, G, & B are x, y, z axis in space; with optional W being an intensity magnitude attribute rather than a 4th axis. Given two RGB colour codes, this class will help plot a linear step between these colours, creating a smooth-ish colour transition. 
	Points exist within axis of R/G/B and take format 0xwwrrggbb, however these calculations only care that the white component be at the end (HsB), the other three are simply triangle sides, the order will be maintained. Therefore wwrrggbb, wwggrrbb, or 00rrggbb will be compatible. 
	Once the vector is defined calls can be made to step through this vector, obtaining new LED colour information, which could then be transmitted to the LED.
*/

class fourChannelTransition	{

private:
	const uint32_t 			pointA, pointB;
	int									_idx{0};												//	internal index
	int 								dr_AB[3];												//	delta ratio vector A->B
	uint16_t 						_VectorDistance{0} ;						//	calculated hypotenuse from RGB ch
	const uint8_t* 			_cartA = (uint8_t*)&pointA;			//	pointer to cartesian of pointA
	const uint8_t* 			_cartB = (uint8_t*)&pointB;			//	
	uint8_t* 						cartWRGB=(uint8_t*)&anyWRGBValue;	//	pointer to cartesian
	float 							_calcLambdaRatio(int);					//	
	uint8_t							_calc_vAB(uint8_t, float);			//	
	uint32_t 						_get_vAB_Lambda_allCh(int);			//	calc lambda on vector A->B

public:
	uint32_t 						anyWRGBValue{0};								//	pointX along A->B
	uint8_t 						DeltaWhite{0} ;									//	change in ww ch only
	uint16_t 						getDistance() ;									//	
	uint32_t						stepFwd(bool=false);
	uint32_t						stepRev(bool=false);
	uint32_t						stepRecall();
	

	// Constructor: start point, end point, step length calc type (longest triangle side(true) or linear length[hypotenuse](false))
	fourChannelTransition(uint32_t FromColour, uint32_t ToColour) :
		pointA{FromColour},
		pointB{ToColour}
		{
			uint8_t WC=0;		//	the White Channel is the first byte. Big-Endian.
			uint32_t _temp{0};
			for (uint8_t ch=0; ch<4; ch++) {
				_temp += pow(abs(_cartB[ch] - _cartA[ch]), 2);
				if (ch==WC) {
					DeltaWhite = _cartB[ch] - _cartA[ch];
					continue;
				}
				dr_AB[ch-1] = (_cartB[ch] - _cartA[ch]);
			}
			//_VectorDistance = min((int)sqrt(_temp)+0.5, 422);	//	√(3×255²) = 422
			_VectorDistance = (int)sqrt(_temp)+0.5;		//	√(3×255²) = max 422
			
		};		//	constructor
};		//	END class

#endif		//	__FOURCHANNELTRANSITION_H__
