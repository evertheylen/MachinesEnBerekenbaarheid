
#ifndef H_GENERATOR
#define H_GENERATOR

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>



class Outputter {
public:
	// Open file for example
	// TODO turn this into constructor?
	virtual void init() = 0;
	
	// Output a terminal
	virtual void output(const std::string& s) = 0;
	
	// close a file for example
	// TODO turn this into destructor?
	virtual void close() = 0;
};


class FileOutPutter: public Outputter {
public:
	FileOutPutter(const std::string& _filename):
			filename(_filename) {}
	
	void init() {
		file.open(filename);
	}
	
	void output(const std::string& s) {
		file << s;
	}
	
	void close() {
		file.close();
	}
	
private:
	std::string filename;
	std::ofstream file;
};




class Replacor {
public:
	// TODO constructor with CFG?
	
	// a list so popping and pushing is easy
	// TODO: string refs look hackerish and error-prone
	virtual std::vector<std::string*> replace(const std::string& var, const std::list<const std::string&>& context) = 0;
	
	virtual bool replaceable(const std::string& symb) = 0;
	// in a CFG bounded class, this would be `return is_var(var)`
	// this function decides whether a symbol will be replaced or outputted
	
};


class DumbReplacor: public Replacor {
public:
	std::string open = "(";
	std::string close = ")";
	
	std::vector<std::string*> replace(const std::string& var, const std::list<const std::string&>& context) {
		return {&open, &var, &close};
	}
	
	bool replaceable(const std::string& symb) {
		if (symb == open or symb == close) {
			return false;
		}
		return true;
	}
};

class Generator {
public:
	Generator(Outputter* _out, Replacor* _repl):
			out(_out), repl(_repl) {}
	
	// if max_repl == -1 ==> Infinite
	void generate(const std::vector<std::string>& start, int max_repl=-1) {
		std::string the_var = "A";
		std::list<std::string*> context = {};
		out->init();
		for (auto& s: start) {
			rec_generate(s, context, max_repl);
		}
		out->close();
	}
	
private:
	void rec_generate(const std::string& s, std::list<std::string*> context, int max_repl) {
		if (max_repl != -1 && context.size() >= max_repl) {
			// we have reached the maximum depth, output this symbol (variable or not)
			out->output(s);
		} else if (repl->replaceable(s)) {
			// replace and continue with recursive stuff
			context.push_back(s);
			std::vector<const std::string&> body = repl->replace(s, context);
			for (auto& sub_s: body) {
				rec_generate(sub_s, context, max_repl);
			}
		} else {
			// not replaceable, output
			out->output(s);
		}
	}
		
	Outputter* out;
	Replacor* repl;
};

#endif

int main() {
	Generator g(new FileOutPutter("test"), new DumbReplacor);
	g.generate({"A"}, 10);
}

