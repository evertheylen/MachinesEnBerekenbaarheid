#pragma once

#include <utility>
#include <set>

#include "NGLib/tree/tree.hpp"
#include "NGLib/outputter/outputter.hpp"
#include "NGLib/replacor/replacor.hpp"

class Teacher {
public:
	using Element3 = std::pair<std::string, unsigned int>;
	// second doesn't matter if it has no children in the tree
	using Teacher3 = Tree<Element3>;
	
	Teacher() = default;
	
	Teacher(Replacor* _repl);

	// if max_repl == -1 ==> Infinite
	Teacher3* generate(std::string start, int max_repl);
	
	void output(Teacher3& tree, Outputter* out);
	
	void score(Teacher3& tree, double score_amount);
	
private:
	void rec_generate(std::string s, Teacher3& tree, typename Teacher3::PathT& path, int max_repl);
	
	Replacor* repl;
};
