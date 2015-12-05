
#include "outputter.hpp"


FileOutputter::FileOutputter(std::string _filename):
		filename(_filename) {}
	
void FileOutputter::init() {
	file.open(filename);
}

void FileOutputter::output(std::string s) {
	file << s;
}

void FileOutputter::close() {
	file.close();
}

TiXmlElement* FileOutputter::to_xml() {
	TiXmlElement* elem = new TiXmlElement("FILEOUTPUTTER");
	return elem;
}
