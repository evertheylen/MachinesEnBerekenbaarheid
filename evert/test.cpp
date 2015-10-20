
#include <iostream>
#include <set>
#include <string>
#include <type_traits>
#include <map>

class PDA {
public:
	int Q_size;  // Q = [0, Q_size[
};

class NamedPDA: public PDA {
public:
	std::map<int, std::string> Q_names; // other direction is expensive (but bimaps were not a great success)
	
	std::string get_name(int q) {
		auto m = Q_names.find(q);
		if (m != Q_names.end()) {
			return m->second;
		} else {
			return std::string("unnamed:" + std::to_string(q));;
		}
	}
	
	void set_name(int q, std::string& s) {
		Q_names[q] = s;
	}
	
	int get_state(std::string s) {
		// -1 means not found
		for (auto iter: Q_names) {
			if (iter.second == s) {
				return iter.first;
			}
		}
		return -1;
	}
	
};

namespace static_if_detail {
	struct identity {
		template<typename T>
		T operator()(T&& x) const {
			return std::forward<T>(x);
		}
	};

	template<bool Cond>
	struct statement {
		template<typename F>
		void then(const F& f){
			f(identity());
		}

		template<typename F>
		void else_(const F&){}
	};

	template<>
	struct statement<false> {
		template<typename F>
		void then(const F&){}

		template<typename F>
		void else_(const F& f){
			f(identity());
		}
	};
} //end of namespace static_if_detail


template<bool Cond, typename F>
static_if_detail::statement<Cond> static_if(F const& f){
    static_if_detail::statement<Cond> if_;
    if_.then(f);
    return if_;
}


// Introducing The Algorithm Of The Year 2015:
//    ... an algorithm creating an extra state!

template<typename T>
T extra_state(T P) {
	P.Q_size++;
	
	// Allright, now set names
	static_if<std::is_same<NamedPDA, T>::value>([&](auto f){
        f
    }).else_([&](auto f){
        --f(value);
    });
}


int main() {
	PDA P;
	P.Q_size = 5;
	std::cout << extra_state(P).Q_size << std::endl;
	
	NamedPDA NP;
	NP.Q_size = 7;
	std::cout << extra_state(NP).get_name(7) << std::endl;
	
	return 0;
}
