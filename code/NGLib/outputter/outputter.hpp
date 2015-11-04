
#ifndef _OUTPUTTER_H
#define _OUTPUTTER_H

#include <string>
#include <fstream>

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
};


class FileOutPutter: public Outputter {
public:
	FileOutPutter(std::string _filename);
	
	void init();
	
	void output(std::string s);
	
	void close();
	
private:
	std::string filename;
	std::ofstream file;
};



#endif
