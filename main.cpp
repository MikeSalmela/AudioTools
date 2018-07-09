#include "Wav/Wav.h"
#include "SignalProcessing/Filter/FIR.h"
#include <fstream>
#include <iostream>

int main(){


	Wav test = Wav("elvis.wav");
	FIR fir = FIR(100, 0.1, HIGHPASS);

	//test.changeDATA(fir.filter(test.get_RAW_data()));

	test.writeWAV("test.wav");

	return 0;
}
