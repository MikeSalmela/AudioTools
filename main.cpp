#include "Wav/Wav.h"
#include <fstream>
#include <iostream>

int main(){
	Wav test = Wav("pro.wav");
	vector<char> data = test.get_RAW_data();
	ofstream of("off.raw");
	for(char c : data){
		of << c;
	}

	return 0;
}
