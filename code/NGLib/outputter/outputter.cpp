
#include "outputter.hpp"


FileOutPutter::FileOutPutter(std::string _filename):
		filename(_filename) {}
	
void FileOutPutter::init() {
	file.open(filename);
}

void FileOutPutter::output(std::string s) {
	file << s;
}

void FileOutPutter::close() {
	file.close();
}
