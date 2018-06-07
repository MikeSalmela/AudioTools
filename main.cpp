#include "Wav/Wav.h"
#include "SignalProcessing/Filter/FIR.h"
#include <fstream>
#include <iostream>

int main(){
	Wav test = Wav("elvis.wav");
	FIR fir = FIR();
	vector<uint8_t> newData = fir.filter(test.get_RAW_data());
	test.changeDATA(newData);
	test.writeWAV("new.wav");

	return 0;
}
