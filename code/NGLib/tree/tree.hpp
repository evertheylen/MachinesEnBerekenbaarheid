
/* [bake me]

[stop baking] */

#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <list>

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& p) {
	out << "PAIR(" << p.first << ", " << p.second << ")";
	return out;
}

#define container_ostream(Container) \
template <typename T> \
std::ostream& operator<<(std::ostream& out, const Container<T>& c) { \
	out << "{"; \
	for (auto el: c) { \
		out << el << ", "; \
	} \
	out << "}"; \
	return out; \
} \

container_ostream(std::list)



template <typename T>
class Tree {
public:
	
	using PathT = std::vector<unsigned int>;
	
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
	
	Tree<T>* get_tree_by_path(const PathT& path) {
		Tree<T>* current_tree = this;
		for (unsigned int i: path) {
			current_tree = current_tree->children.at(i);
		}
		return current_tree;
	}
	
	std::list<T> get_list_by_path(const PathT& path) {
		std::list<T> l = {data};
		Tree<T>* current_tree = this;
		for (unsigned int i: path) {
			current_tree = current_tree->children.at(i);
			l.push_back(current_tree->data);
		}
		return l;
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
	
	std::vector<Tree<T>*> all_children() const {
		std::vector<Tree<T>*> result;
		_all_children(result);
		return result;
	}
	
	void _all_children(std::vector<Tree<T>*>& child_vec) const {
		for (Tree<T>* child: children) {
			if (!child->children.empty()) {
				child->_all_children(child_vec);
			}
			child_vec.push_back(child);
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


