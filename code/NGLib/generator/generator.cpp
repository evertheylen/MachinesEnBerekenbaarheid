
#include "generator.hpp"

#include "NGLib/outputter/outputter.hpp"
#include "NGLib/replacor/replacor.hpp"

Generator::Generator(Outputter* _out, Replacor* _repl):
	out(_out), repl(_repl) {}


// if max_repl == -1 ==> Infinite (default)
void Generator::generate(std::vector<std::string> start, int max_repl) {
	std::list<std::string> context = {};
	out->init();
	for (auto s: start) {
		rec_generate(s, context, max_repl);
	}
	out->close();
}


void Generator::rec_generate(std::string s, std::list<std::string>& context, int max_repl) {
	if (max_repl != -1 && context.size() >= max_repl) {
		// we have reached the maximum depth, output this symbol (variable or not)
		out->output(s);
	} else if (repl->replaceable(s)) {
		// replace and continue with recursive stuff
		context.push_back(s);
		std::vector<std::string> body = repl->replace(s, context);
		for (auto sub_s: body) {
			rec_generate(sub_s, context, max_repl);
		}
		context.pop_back(); // reference!
	} else {
		// not replaceable, output
		out->output(s);
	}
}

