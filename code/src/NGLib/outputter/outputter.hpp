
#ifndef _OUTPUTTER_H
#define _OUTPUTTER_H

#include <string>
#include <fstream>
#include "tinyxml/tinyxml.h"

class Outputter {
public:
	// Open file for example
	// TODO turn this into constructor?
	virtual void init() = 0;
	
	// Output a terminal
	virtual void output(std::string s) = 0;
	
	// close a file for example
	// TODO turn this into destructor?
	virtual void close() = 0;
	
	virtual TiXmlElement* to_xml() = 0;
	
	virtual ~Outputter() {};
};


class FileOutputter: public Outputter {
public:
	FileOutputter(std::string _filename="test");
	
	FileOutputter(TiXmlElement* root);
	
	void init();
	
	void output(std::string s);
	
	void close();
	
	TiXmlElement* to_xml();
	
private:
	std::string filename;
	std::ofstream file;
};



#endif
