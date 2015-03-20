/*
* read.cpp
*
*  Created on: 2015Äê3ÔÂ16ÈÕ
*      Author: Administrator
*/
#include "readFromTxt.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define LINES_TXT 6  // tax file lines

using namespace std;
void testByLine() {
	char buffer[256];
	fstream outFile;
	static string str[LINES_TXT];
	outFile.open("Config.txt", ios::in); //readFile
	if (outFile.fail()) {
		cout << "error file not exists" << endl;
		outFile.close();
	}
	else {
		cout << "Config.txt" << "--- all file is as follows:---" << endl;
		int i = 0;
		while (!outFile.eof()) {

			outFile.getline(buffer, 256, '\n');
			str[i] = buffer;
			cout << str[i] << endl;
		}
		outFile.close();
	}

}

/*
int main() {


	cout << endl << "read by Line £ºtestByLine()" << endl;
	testByLine();


}*/