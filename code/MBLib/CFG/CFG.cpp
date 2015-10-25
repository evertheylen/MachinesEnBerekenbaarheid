
#include "CFG.hpp"

#include <memory>
#include <string>



// s_CFG

s_CFG::s_CFG() {}

s_CFG::s_CFG(std::set<std::string> _V, std::set<std::string> _T, std::string _S):
		CFG<std::string>(_V, _T, _S) {}

s_CFG::s_CFG(TiXmlDocument& doc) {
	TiXmlElement* root = doc.RootElement();
	assert(root != nullptr);
	
	auto vars_el = root->FirstChildElement("Variables");
	assert(vars_el != nullptr);
	for (char s: std::string(vars_el->GetText())) {
		this->V.insert(std::string(1, s));
	}
	this->S = std::string(vars_el->FirstChildElement("StartSymbol")->GetText());
	
	auto term_el = root->FirstChildElement("Terminals");
	assert(term_el != nullptr);
	for (char s: std::string(term_el->GetText())) {
		this->T.insert(std::string(1,s));
	}
	
	auto prod_el = root->FirstChildElement("Productions");
	assert(prod_el != nullptr);
	for (TiXmlElement* rule_el = prod_el->FirstChildElement("Rule");
			rule_el != nullptr;
			rule_el = rule_el->NextSiblingElement("Rule")) {
		std::string head = std::string(rule_el->Attribute("LHS"));
		std::string bodies = std::string(rule_el->Attribute("RHS"));
		std::vector<std::string> buf;
		for (char c: bodies) {
			if (c == '|') {
				this->P[head].insert(buf);
				buf.clear();
			} else {
				buf.push_back(std::string(1,c));
			}
		}
		this->P[head].insert(buf);
	}
}

std::string s_CFG::create_variable(const std::string& name) {
	assert(!this->is_variable(name));
	this->V.insert(name);
	return name;
}

void s_CFG::to_xml(std::string filename) {
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	TiXmlElement* root = new TiXmlElement("CFG");
	
	TiXmlElement* vars = new TiXmlElement("Variables");
	std::string varstring;
	for (std::string s: this->V) varstring += s;
	TiXmlText* vartext = new TiXmlText(varstring);
	vars->LinkEndChild(vartext);
	
	TiXmlElement* startsymbol = new TiXmlElement("StartSymbol");
	TiXmlText* symboltext = new TiXmlText(this->S);
	startsymbol->LinkEndChild(symboltext);
	vars->LinkEndChild(startsymbol);
	
	root->LinkEndChild(vars);
	
	TiXmlElement* terminals = new TiXmlElement("Terminals");
	std::string termstring;
	for (std::string s: this->T) termstring += s;
	TiXmlText* termtext = new TiXmlText(termstring);
	terminals->LinkEndChild(termtext);
	
	root->LinkEndChild(terminals);
	
	TiXmlElement* productions = new TiXmlElement("Productions");
	for (auto iter: this->P) {
		std::string head = iter.first;
		for (auto body: iter.second) {
			TiXmlElement* rule = new TiXmlElement("Rule");
			rule->SetAttribute("LHS", head);
			rule->SetAttribute("RHS", to_string(body));
			productions->LinkEndChild(rule);
		}
	}
	
	root->LinkEndChild(productions);
	
	doc.LinkEndChild(decl);
	doc.LinkEndChild(root);
	
	doc.SaveFile(filename);
}
