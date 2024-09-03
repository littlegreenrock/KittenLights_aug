
#ifndef __TIMEOUT_H__
#define __TIMEOUT_H__

#include <Arduino.h>


struct timeout_t {
	uint32_t _time{0};
	uint32_t runningTotal{0};
	uint32_t* _splitArray{nullptr};
	byte laps{0};

	~timeout_t(){
		Reset();
	}
/*	future, timer, heartbeat, countdown, functions:
			runningTotal == no. beats; 
			_time == millis() comparison time	*/
	void Extend(int _ms) {
		_time += _ms;	}
	void Next(int _ms) {
		_time = millis() + _ms;	
		runningTotal++;	}
	bool Valid() {
		return (_time > millis());	}
	bool CheckPoint(int _ms) {
		if (Valid()==false) {
			this->Next(_ms);
			return true;
		}
		return false;	
	}
/*	past, duration timing, stopwatch features:
			runningTotal == instantaneous ms;
			_time == 'zero' time;	*/
	uint32_t Start() {
		// yes; it's just an alias to toggle Start/Stop
		return Stop();
	}
	uint32_t Stop() {	
		if (_time > 0) {			//	stop
			runningTotal += (millis() - _time);
			_time = 0;
		}	else {							//	start
			_time = millis();		//	'zero' the time. 
		}
		return runningTotal;	//	nb: never reset from Start/Stop()
	}
	void Reset() {	
		//	zero everything, discard any lap memory
		delete _splitArray;
		_splitArray = nullptr;
		laps = 0;
		_time = 0;
		runningTotal = 0;
	}
	uint32_t _splitTime(bool recordLap) { 
		/*	behaves as the Split/Lap button would.  
				Split gives you the instantaneous time without stopping the count.
				Lap does the same but also records the split time. 		*/
		if (_time==0) Reset();
		else {
			uint32_t split = millis() - _time;
			// runningTotal += (millis() - _time);
			if (recordLap) {
				uint32_t* newSplit = new uint32_t[++laps] {split};
				for (int i = 1; i< laps; i++) {
					newSplit[i] = _splitArray[i-1];		}
				delete _splitArray;
				_splitArray = &newSplit[0];
				if (laps>1) return split - _splitArray[1];
			}
			return split;
		}
		return 0;
	}
	uint32_t Split() {
		return _splitTime(false);
	}

	uint32_t Lap() {
		// records the split, and returns the lap data
		return _splitTime(true);
	}
	/*	interpreted as a request for lap-time(idx).  
			If 'running' and Lap data : returns the delta between laps. 	
						-and no lap data: interpreted as if Lap()
						ie: Lap(0) is no different to Lap()
						and Lap(1) is no different to Lap()
			If 'stopped' and Lap data : returns the delta between laps.
						-and no lap data: interpreted as if Lap() --> Split(true) --> Reset()
						ie: Lap(0) returns data for lap 0
						and Lap(1) returns data for lap 1
			*/
	uint32_t Lap(int idx) {
		if (_time==0) {		//	'stopped' state
			idx = constrain((laps-1-idx), 0, laps-1);
			if (idx==laps-1)	return _splitArray[idx] - 0;	//	because array is in reverse order.  We handle that quietly here and don't tell the human.
			else 							return _splitArray[idx] - _splitArray[idx+1];
		}
		return Lap();
	}
};

#endif

