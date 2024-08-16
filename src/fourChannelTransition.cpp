/*
		fourChannelTransition
*/
#include "fourChannelTransition.h"

float fourChannelTransition::_calcLambdaRatio(int L) {
	return float(L) / float(getDistance());
	// int a = float(float(L*100) / (float)getDistance()) +0.5;
	// return float(a/100);		// force ".2f"
}

uint8_t fourChannelTransition::_calc_vAB(uint8_t Ch, float LR) {
	if (Ch==0) return uint8_t(LR * DeltaWhite + _cartA[Ch]);
	else return uint8_t(LR * dr_AB[Ch-1] + _cartA[Ch]);
}

uint32_t fourChannelTransition::_get_vAB_Lambda_allCh(int L) {
	float LR = _calcLambdaRatio(L);
	if (LR>=1.0) return pointB;
	else if(LR<0.01) return pointA;
	else {
		for (uint8_t Ch=0; Ch<4; Ch++) {
			cartWRGB[Ch] = _calc_vAB(Ch, LR);
		}
		return anyWRGBValue;
	}
}

uint16_t fourChannelTransition::getDistance() {
	return _VectorDistance;
}

uint32_t fourChannelTransition::stepFwd(bool toEnd) {
	_idx++;
	if (toEnd || _idx > getDistance()) _idx = getDistance();
	return _get_vAB_Lambda_allCh(_idx);
}

uint32_t fourChannelTransition::stepRev(bool toEnd) {
	_idx--;
	if (toEnd || _idx < 0) _idx = 0;
	return _get_vAB_Lambda_allCh(_idx);
}

uint32_t fourChannelTransition::stepRecall() {
	return _get_vAB_Lambda_allCh(_idx);
}
