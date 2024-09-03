
#ifndef __SIGMOID_FLY__
#define __SIGMOID_FLY__

#include <Arduino.h>



class sigmoid_fly {
	private:
		bool _live{false};
		int* array{0};
		void _reset() ;
		float _softArg_to_curveAdjust(int ) ;
		void _topandbottom(const int* , const int* , const float* , int* , int* ) ;
		
	public:
		sigmoid_fly(){}	;
		~sigmoid_fly(){
			_reset();		};
		float sigmoid(float ) ;
		bool build(int , int , int ) ;
		bool build() ;
		int get(int ) ;

};		//	END class sigmoid_fly

void sigmoid_fly::_reset() {
	delete array;
	array = nullptr;
	_live=false;
}

float sigmoid_fly::sigmoid(float x){
	return (1.0 / (1.0 + exp(x)));
}

float sigmoid_fly::_softArg_to_curveAdjust(int soft) {
	switch (soft) {
		default : return 0.2;
		break; case 1 : return 0.17;
		break; case 2 : return 0.13;
		break; case 3 : return 0.1;
	}
}

void sigmoid_fly::_topandbottom(const int* minValue, const int* maxValue, const float* adj, int* subtract, int* multiply) {
	if (*minValue < *maxValue) {
		*subtract = -*minValue / *adj;
		*multiply = *maxValue/ *adj - *minValue / *adj;
	}
	else {
		*subtract = *maxValue / *adj;
		*multiply = *minValue / *adj - *maxValue / *adj;
	}
}

/* The method used by this function to scale, and make scalable, 
		relies on building the array from the middle to the beginning, 
		and the end, respectively.	If you fiddle around with Sigmoid 
		curves for a bit you will understand why. 		*/
bool sigmoid_fly::build(int minValue, int maxValue, int Soft) {
	const float Incline = _softArg_to_curveAdjust(Soft);
	int subtract, multiply, i;
	float _adjust = sigmoid(Incline * -32);
	_topandbottom(&minValue, &maxValue, &_adjust, &subtract, &multiply);

	if (_live) _reset();
	array = new int[64];
	_live=true;
	
	float x = 0;		// first pass, early array
	for (i=32; i>=0; i--) {
		array[i] = ((sigmoid(x) * multiply) - subtract) ;
		x -= Incline;
	}
	x = 0;					// second pass, late array
	for (i=32; i<64; i++) {
		array[i] = ((sigmoid(x) * multiply) - subtract) ;
		x += Incline;
	}
	return build();
}

bool sigmoid_fly::build() {
	return _live;
}

int sigmoid_fly::get(int idx) {
	idx = constrain(idx, 0, 63);
	if (_live) return array[idx];
	else return 0;
}





#endif


