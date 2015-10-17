
#include "CFG.hpp"

CFG::CFG() {}

CFG::CFG(TiXmlDocument& doc) {
	TiXmlElement* root = doc.RootElement();
	assert(root != nullptr);
	
	auto vars_el = root->FirstChildElement("Variables");
	assert(vars_el != nullptr);
	for (char s: std::string(vars_el->GetText())) {
		V.insert(s);
	}
	
	S = *(vars_el->FirstChildElement("StartSymbol")->GetText());  // Char pointer
	
	auto term_el = root->FirstChildElement("Terminals");
	assert(term_el != nullptr);
	for (char s: std::string(term_el->GetText())) {
		T.insert(s);
	}
	
	auto prod_el = root->FirstChildElement("Productions");
	assert(prod_el != nullptr);
	for (TiXmlElement* rule_el = prod_el->FirstChildElement("Rule");
			rule_el != nullptr;
			rule_el = rule_el->NextSiblingElement("Rule")) {
		char head = *(rule_el->Attribute("LHS"));
		std::string bodies = std::string(rule_el->Attribute("RHS"));
		std::string buf("");
		for (char c: bodies) {
			if (c == '|') {
				P[head].insert(buf);
				buf.clear();
			} else {
				buf += c;
			}
		}
		P[head].insert(buf);
	}
}

std::ostream& operator<< (std::ostream& out, CFG& G) {
	out << "Variables = {";
	for (char var: G.V) {
		out << var << ", ";
	}
	
	out << "}\nTerminals = {";
	for (char term: G.T) {
		out << term << ", ";
	}
	
	out << "}\nProductions = {\n";
	for (auto iter: G.P) {
		for (auto rule: iter.second) {
			out << "  " << iter.first << " --> " << rule << "\n";
		}
	}
	
	out << "}\nStart symbol = " << G.S << "\n";
	return out;
}

void CFG::to_xml(std::string filename) {
	TiXmlDocument doc;
	TiXmlDeclaration decl("1.0", "", "");
	TiXmlElement root("CFG");
	
	doc.LinkEndChild(&decl);
	doc.LinkEndChild(&root);
	
	doc.SaveFile(filename);
}
