
#ifndef H_SETS
#define H_SETS

#include <iostream>
#include <map>
#include <string>
#include <cassert>
#include <algorithm>

// Helper class for returning a search result from get_name
template <typename T>
struct SearchResult {	
	SearchResult(bool _found, T _value):
			found(_found), value(_value) {}
	
	bool found;
	T value;
};

// base class, also virtual
template <typename ID_T, typename T>
class SetBase {
public:
	typedef SearchResult<T> Result;
	
	SetBase(T _default_name, ID_T size):
			default_name(_default_name) {
		for (ID_T i=0; i<size; i++) {
			add(i);
		}
	}
	
	virtual SearchResult<T> get_name(ID_T i) = 0;
	//virtual SearchResult<ID_T> get_ID(T i) = 0;
	virtual void set_name(ID_T i, T name) = 0;
	
	ID_T get_size() {
		return set.size();
	}
	
	void add(ID_T i) {
		set.insert(i);
		counter = std::max(i, counter)+1;
	}
	
	ID_T add() {
		set.insert(counter);
		counter++;
		return counter-1;
	}
	
	virtual void remove(ID_T i) {
		if (contains(i)) {
			set.erase(i);
		}
	}
	
	bool contains(ID_T i) {
		return set.find(i) != set.end();
	}

protected:
	T default_name;
	std::set<ID_T> set;
	// counter is used when creating a new element, so we know an ID not yet used
	// if you create gaps between ID's, the counter will not consider them, so be careful.
	ID_T counter = 0;
};

// has a map to store names
template <typename ID_T=unsigned int, typename T=std::string>
class NamedSet: public SetBase<ID_T, T> {
public:
	typedef SearchResult<T> Result;
	
	NamedSet(T _default_name, std::vector<T> _names, ID_T _size=0):
			SetBase<ID_T, T>(_default_name, _size) {
		this->size = (_size > _names.size() ? _size : _names.size());
		int i = 0;
		for (T& iter: _names) {
			names[i] = iter;
			i++;
		}
	}
	
	NamedSet(T _default_name, ID_T _size=0):
			SetBase<ID_T, T>(_default_name, _size) {}
	
	Result get_name(ID_T i) {
		assert(this->contains(i));
		
		auto m = names.find(i);
		if (m != names.end()) {
			return Result(true, m->second);
		} else {
			return Result(false, this->default_name);
		}
	}
	
	void set_name(ID_T i, T name) {
		assert(this->contains(i));
		names[i] = name;
	}
	
	void remove(ID_T i) {
		((SetBase<ID_T, T>*) this)->remove(i);
		auto p = names.find(i);
		if (p != names.end()) {
			names.erase(p);
		}
	}

private:
	std::map<ID_T, T> names;
};

// does not store names
template <typename ID_T=unsigned int, typename T=std::string>
class UnnamedSet: public SetBase<ID_T, T> {
public:
	typedef SearchResult<T> Result;
	
	UnnamedSet(T _default_name, ID_T _size):
			SetBase<ID_T, T>(_default_name, _size) {}
	
	Result get_name(ID_T i) {
		assert(this->containts(i));
		return Result(false, this->default_name);
	}
	
	void set_name(ID_T i, T name) {
		// do nothing
		assert(this->containts(i));
	}
};

#endif
