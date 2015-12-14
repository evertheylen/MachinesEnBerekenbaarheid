#pragma once

/* [bake me]

dependencies["headers"] = [
	"libs/tinyxml>>headers",
	"MBLib/new_CFG>>headers",
	"NGLib/outputter>>headers",
	"NGLib/replacor>>headers",
	"NGLib/exceptions>>headers",
]

[stop baking] */

#include "NGLib/replacor/context_replacor.hpp"
#include "NGLib/tree/tree.hpp"

class Teacher {
public:
	Teacher() {}
	
	Teacher(ContextReplacor _repl):
		repl(_repl) {}


	// if max_repl == -1 ==> Infinite
	void generate(std::string start, int max_repl = -1) {
		Tree<ContextReplacor::Rule_Type::NumT> tree;
// 		out->init();
		rec_generate(start, tree, max_repl);
// 		out->close();
	}
	
	
	
private:
	void rec_generate(std::string s, Tree<ContextReplacor::Rule_Type::NumT>& tree, int max_repl) {
		for (int i=0; i<context.size(); i++) std::cout << "  ";
		if ((max_repl != -1) and (context.size() == max_repl)) {
// 			out->output(s);
			std::cout << "[" << s << "]\n";
		} else if (repl->replaceable(s)) {
			std::cout << s << ": \n";
			auto num = repl->replace(s, context);
			context.push_back(num);
			for (auto sub_s: repl->get_body(num)) {
				rec_generate(sub_s, context, max_repl);
			}
			context.pop_back(); // reference!
		} else {
// 			out->output(s);
			std::cout << "[" << s << "]\n";
		}
	}
	
	// templated
	ContextReplacor repl;
};