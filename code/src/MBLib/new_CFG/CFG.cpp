
#include "CFG.hpp"

SimpleRule::SimpleRule(TiXmlElement* root) {
// 		try {
		std::string num_s = std::string(root->Attribute("ID"));
		if (num_s == "") throw SyntacticError("No ID given");
		head = std::string(root->Attribute("LHS"));
		if (head == "") throw SyntacticError("No head given");
		std::string body_s = std::string(root->Attribute("RHS"));
		if (body_s == "") throw SyntacticError("No body given");
		
		body = split(body_s);
		num = std::stoi(num_s);
// 		} catch (std::exception& e) {
// 			throw syntacticError();
// 		}
}

SimpleRule::SimpleRule(const std::string& _head, const std::vector<std::string>& _body, unsigned int _num):
	head(_head), body(_body), num(_num) {}

const std::string& SimpleRule::get_head() const {
	return head;
}

const std::vector<std::string>& SimpleRule::get_body() const {
	return body;
}

unsigned int SimpleRule::get_num() const {
	return num;
}

TiXmlElement* SimpleRule::to_xml() {
	TiXmlElement* rule_el = new TiXmlElement("Rule");
	rule_el->SetAttribute("LHS", head);
	rule_el->SetAttribute("RHS", to_string(body));
	rule_el->SetAttribute("ID", to_string(num));
	return rule_el;
}

