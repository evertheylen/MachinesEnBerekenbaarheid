#include <iostream>
#include <cmath>
#include <type_traits>
using namespace std;

// My question: can I rewrite algo so that it can be written somewhat like the following?

#define same_type(S, T) (std::is_same<S, T>::value)

template <typename T>
T algo (T input) {
	input -= 2;
	input += 5;
	
	if (same_type(T, int)) {
		input <<= 2;
	} else if (same_type(T, int)) {
		input = floor(input);
	}
	
	input += 7;
	input -= 12;
	return input;
}


template <>
int algo<int>(int input) {
	// common stuff for double and int
	input -= 2;
	input += 5;
	
	// specifically for int
	input <<= 2;
	
	// more common stuff for double and int
	input += 7;
	input -= 12;
	return input;
}

template<>
double algo<double>(double input) {
	// common stuff for double and int
	input -= 2;
	input += 5;
	
	// specifically for double
	input = floor(input);
	
	// more common stuff for double and int
	input += 7;
	input -= 12;
	return input;
}



int main() {
	cout << algo(59.2123) << endl;
	cout << algo(400) << endl;
}
