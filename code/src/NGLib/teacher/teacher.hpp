#pragma once

/* [bake me]

dependencies["headers"] = [
	"libs/tinyxml>>headers",
	"MBLib/new_CFG>>headers",
	"NGLib/outputter>>headers",
	"NGLib/replacor>>headers",
	"NGLib/exceptions>>headers",
	"NGLib/tree>>headers",
]

[stop baking] */

#include <utility>
#include <iostream>

#include "NGLib/replacor/context_replacor.hpp"
#include "NGLib/tree/tree.hpp"
#include "NGLib/outputter/outputter.hpp"
#include <set>

class Teacher {
public:
	using Element3 = std::pair<std::string, ContextReplacor::Rule_Type::NumT>;
	// second doesn't matter if it has no children in the tree
	using Teacher3 = Tree<Element3>;
	
	Teacher() = default;
	
	Teacher(ContextReplacor* _repl):
		repl(_repl) {}


	// if max_repl == -1 ==> Infinite
	Teacher3 generate(std::string start, int max_repl) {
		Teacher3 tree({start, 0});
		Teacher3::PathT path;
		rec_generate(start, tree, path, max_repl);
		return tree;
	}
	
	void output(Teacher3& tree, Outputter* out) {  // TODO use Outputter
		if (tree.children.empty()) {
			out->output(tree.data.first);
		} else {
			for (Teacher3* subtree: tree.children) {
				output(*subtree, out);
			}
		}
	}
	
	void score(Teacher3& tree, double score_amount) {
		std::set<std::pair<ContextReplacor::Rule_Type::NumT, ContextReplacor::Rule_Type::NumT>> already_updated;
		score_helper(tree, score_amount, already_updated);
	}
	
	void score_helper(Teacher3& tree, double score_amount, std::set<std::pair<ContextReplacor::Rule_Type::NumT, ContextReplacor::Rule_Type::NumT>>& already_updated) {
		// modify scores for all children of tree.
		
		for (Teacher3* child: tree.all_children()) {
			if (child->data.second == 0) continue;
			if (already_updated.find({tree.data.second, child->data.second}) != already_updated.end()) continue; 
			double result = repl->cfg.get_rule(tree.data.second).table[child->data.second] * score_amount;
			repl->cfg.get_rule(tree.data.second).table[child->data.second] = result;
			already_updated.insert({tree.data.second, child->data.second});
		}
		
		// Call score recursively for all children
		for (Teacher3* child: tree.children) {
			score_helper(*child, score_amount, already_updated);
		}
	}
	
private:
	void rec_generate(std::string s, Teacher3& tree, typename Teacher3::PathT& path, int max_repl) {
		//std::cout << "assert " << s << " == " << tree.get_tree_by_path(path)->data.first << "\n";
		assert(s == tree.get_tree_by_path(path)->data.first);
		// if we haven't reached maximum replacements and s is replacable (probably variable)
		if ((not ((max_repl != -1) and int(path.size()) >= max_repl)) and repl->replaceable(s)) {
			// replacable (probably variable)
			
			// build up context
			std::list<typename ContextReplacor::Rule_Type::NumT> context;
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
	
	// templated
	ContextReplacor* repl;
};
