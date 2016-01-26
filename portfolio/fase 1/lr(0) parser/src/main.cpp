/*
 * main.cpp
 *
 *  Created on: 21 okt. 2015
 *      Author: AnthonyHermans
 */

#include "LRParser.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]){
	try {
		if (argc < 2){
			std::cout << "Not enough arguments given\a\n";
			return 0;
		}else{
			LRParser lr;
			// argv[1] is the input xml file
			std::string parse_string = "0+1+0+1";
			lr.parseXML(argv[1],parse_string);
			lr.checkStringConsistency();
			lr.parseLR();
			return 0;
		}
	}
	catch(std::exception& except){
		std::cout << except.what() << std::endl;
	}
	catch(...){
		std::cout << "An error has occured\n";
	}
}


