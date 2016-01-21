#include <utility>
#include <iostream>
#include <set>

#include "NGLib/tree/tree.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/replacor/replacor.hpp"
#include "teacher.hpp"

Teacher::Teacher(Replacor* _repl):
	repl(_repl) {}

Teacher::Teacher3* generate(std::string start, int max_repl) {
	Teacher3* tree = new Teacher3({start, 0});
	Teacher3::PathT path;
	rec_generate(start, *tree, path, max_repl);
	return tree;
}
	
void Teacher::output(Teacher3& tree, Outputter* out) {
	if (tree.children.empty()) {
		out->output(tree.data.first);
	} else {
		for (Teacher3* subtree: tree.children) {
			output(*subtree, out);
		}
	}
}
	
void Teacher::score(Teacher3& tree, double score_amount) {
	repl->score(tree, score_amount);
}
	
void Teacher::rec_generate(std::string s, Teacher3& tree, typename Teacher3::PathT& path, int max_repl) {
	//std::cout << "assert " << s << " == " << tree.get_tree_by_path(path)->data.first << "\n";
	assert(s == tree.get_tree_by_path(path)->data.first);
	// if we haven't reached maximum replacements and s is replacable (probably variable)
	if ((not ((max_repl != -1) and int(path.size()) >= max_repl)) and repl->replaceable(s)) {
		// replacable (probably variable)
		
		// build up context
		std::list<unsigned int> context;
		//std::cout << "path is " << path << "\n";
		for (auto p: tree.get_list_by_path(path)) context.push_back(p.second);
		context.pop_back();
		
		auto rule = repl->replace(s, context);
		//std::cout << "replacing by " << rule << "\n";
		Teacher3* subtree = tree.get_tree_by_path(path);
		// set rule by which children are generated
		subtree->data.second = rule;
		int i = 0;
		for (std::string sub_s: repl->get_body(rule)) {
			// std::cout << "sub_s == " << sub_s << "\n";
			// set child
			subtree->children.push_back(new Teacher3({sub_s, 0}));
			// make new path
			auto new_path = path;
			new_path.push_back(i);
			rec_generate(sub_s, tree, new_path, max_repl);
			i++;
		}
	}
	// other cases (max repl reached, terminal): do nothing
}
	
