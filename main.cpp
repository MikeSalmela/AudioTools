#include "Wav/Wav.h"
#include <fstream>
#include <iostream>

int main(){
	Wav test = Wav("diamonds.wav");
	test.writeWAV("new.wav");	

	return 0;
}
