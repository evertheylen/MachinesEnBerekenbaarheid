#include "normal_replacor.hpp"


unsigned int NormalReplacor::replace(std::string var, std::list<unsigned int>& context) {
	std::uniform_int_distribution<int> dist(0, 99);
	int picked_rule = dist(mt);
	double prev_chance = 0;
	int total_possibilities = cfg.rules(var).size();
	for (auto& it: cfg.rules(var)) {
		double chance = prev_chance + (100/total_possibilities);
		if (picked_rule - prev_chance < chance) {
			return it.second;
		} else {
			prev_chance += chance;
		}
	}
	return {};
}

bool NormalReplacor::replaceable(std::string symb) {
	return cfg.has_rules(symb);
}

TiXmlElement* NormalReplacor::to_xml() {
	TiXmlElement* elem = new TiXmlElement("NORMAL_REPLACOR");
	TiXmlElement* cfg_elem = cfg.to_xml();
	elem->LinkEndChild(cfg_elem);
	return elem;
}

