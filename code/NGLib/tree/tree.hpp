
/* [bake me]

[stop baking] */

#pragma once

#include <vector>
#include <memory>
#include <iostream>

template <typename T>
class Tree {
public:
	Tree() = default;
	
	Tree(T _data): data(_data) {}
	
	Tree(T _data, std::vector<Tree<T>*> _children):
			data(_data), children(_children) {}
	
	Tree(T _data, std::vector<Tree<T>> _children):
			data(_data) {
		for (Tree<T>& t: _children) {
			children.push_back(t.clone());
		}
	}
	
	Tree<T>* clone() {
		Tree<T>* t = new Tree<T>(data);
		for (Tree<T>* c: children) {
			t->children.push_back(c->clone());
		}
		return t;
	}
	
	friend std::ostream& operator<<(std::ostream& out, const Tree& t) {
		t.print(out);
		return out;
	}
	
	void print(std::ostream& out, int indent=0) const {
		for (int i=0; i<indent; i++) out << "  ";
		out << "- " << data << "\n";
		for (Tree<T>* c: children) {
			c->print(out, indent+1);
		}
	}
	
	~Tree<T>() {
		for (Tree<T>* c: children) {
			delete c;
		}
	}
	
	T data;
	std::vector<Tree<T>*> children;
};


