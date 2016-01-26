#include "stochastic_replacor.hpp"

unsigned int StochasticReplacor::replace(const std::string& var, std::list<unsigned int>& context) {
	std::uniform_real_distribution<double> dist(0, 1);
	double picked_rule = dist(mt);
	double prev_chance = 0;
	for (auto& it: cfg.rules(var)) {
		double chance = cfg.get_rule(it.second).get_chance();
		if (picked_rule - prev_chance < chance) {
			return it.second;
		} else {
			prev_chance += chance;
		}
	}
	
	return {};
}
	
TiXmlElement* StochasticReplacor::to_xml() {
	TiXmlElement* elem = new TiXmlElement("STOCHASTIC_REPLACOR");
	TiXmlElement* cfg_elem = cfg.to_xml();
	elem->LinkEndChild(cfg_elem);
	return elem;
}
