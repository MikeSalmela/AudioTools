#include "Wav/Wav.h"
#include "SignalProcessing/Filter/FIR.h"
#include <fstream>
#include <iostream>

int main(){
	Wav test = Wav("seiska.wav");
//	FIR fir = FIR(1000, 0.1, HIGHPASS);

	// test.changeDATA(fir.filter(test.get_RAW_data()));
	test.writeWAV("new.wav");

	return 0;
}
