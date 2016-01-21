
#include "PDA_CFG.hpp"

// SetChoice

SetChoice::SetChoice(unsigned int _max, int length): 
		choice(length), max(_max) {}

bool SetChoice::next_choice() {
	for (int i=0; i<choice.size(); i++) {
		if (choice.at(i) < max-1) {
			// we can change this!
			choice[i]++;
			for (int j=i-1; j >= 0; j--) {
				choice[j] = 0;
			}
			return true;
		}
	}
	// if we get here, reset all
	for (int i=0; i<choice.size(); i++) choice[i] = 0;
	return false;
}

unsigned int SetChoice::size() {
	return choice.size();
}

unsigned int SetChoice::at(unsigned int i) {
	return choice.at(i);
}

