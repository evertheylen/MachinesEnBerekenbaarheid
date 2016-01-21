
#include "s_rule.hpp"

StochasticRule::StochasticRule(TiXmlElement* root): SimpleRule(root) {
		const char* chance_attr = root->Attribute("chance");
		if (chance_attr == nullptr) throw SyntacticError("No chance attr");
		chance = std::stod(std::string(chance_attr));
	}
	
StochasticRule::StochasticRule(const std::string& _head, const std::vector<std::string>& _body, unsigned int _num, double _chance):
	SimpleRule(_head, _body, _num), chance(_chance) {}

TiXmlElement* StochasticRule::to_xml() {
	TiXmlElement* root = SimpleRule::to_xml();
	// more precision for chance
	std::stringstream morePrecision;
	morePrecision.precision(20);
	morePrecision << chance;
	//std::cout << morePrecision.str() << std::endl;
	root->SetAttribute("chance", morePrecision.str());
	return root;
}

double StochasticRule::get_chance() const {
	return chance;
}
