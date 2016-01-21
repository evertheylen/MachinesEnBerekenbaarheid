
#include "CYK.hpp"

void pad(const std::string& s, std::ostream& out, unsigned int length) {
	out << s;
	for (int i=0; i<(length-s.size()); i++) {
		out << " ";
	}
}


