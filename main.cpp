#include "Wav/Wav.h"
#include "SignalProcessing/Filter/FIR.h"
#include "SignalProcessing/Audio/Audio.h"
#include <fstream>
#include <iostream>


int main(){


	//Wav test = Wav("seiska.wav");
	makeSine(5000, 20000, 100000);
	Wav test2(makeSine(5000, 50000, 2000), 50000);
	test2.writeWAV("test2.wav");
	//test.changeDATA(fir.filter(test.get_RAW_data()));

	//test.writeWAV("test.wav");

	return 0;
}
