/*
 * LRParser.cpp
 *
 *  Created on: 21 okt. 2015
 *      Author: AnthonyHermans
 */

#include "LRParser.h"
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
#include <string>
#include <stack>
#include <stdexcept>

std::ostream& operator<<(std::ostream& out, const LRParser& self) {
	out << "The grammar is:" << std::endl;
	for (int i = 0; i<self.grammarDefault.size(); i++){
		out << self.grammarDefault.at(i) << " -> " << self.grammarDefault.at(i+1) << std::endl;
		i++;
	}
//	out << "Input:" << self.inputDefault << std::endl;
//	out << "Terminals:";
//	for (int i=0;i<self.terminals.size();i++){
//		out << " " << self.terminals.at(i);
//	}
//	out << std::endl << "Nonterminals:";
//	for (int i=0;i<self.nonterminals.size();i++){
//		out << " " << self.nonterminals.at(i);
//	}
//	out << std::endl;

//	//=======[Item Set Output]=======
//	for (int i = 0; i<self.itemSet.size();i++){
//		out <<"Item set " <<self.itemSet.at(i).name << ":\n";
//		for (int j=0;j<self.itemSet.at(i).rules.size();j+=2){
//			out << self.itemSet.at(i).rules.at(j) << "->" << self.itemSet.at(i).rules.at(j+1) << std::endl;
//		}
//	}
//	out << "The transitionTable: \n";
//	out << "Item set|";
//	for (int i=0;i<self.terminals.size();i++){
//		out << " " << self.terminals.at(i) << " |";
//	}
//	for (int i=0;i<self.nonterminals.size();i++){
//		out << " " << self.nonterminals.at(i) << " |";
//	}
//	out << std::endl;
//	for (int i=0;i<self.itemSet.size();i++){
//			out << "   " << i << "    | ";
//			for(int j=0;j<self.terminals.size();j++){
//				if(self.includedInItemset(self.terminals.at(j),i)){
//					out << self.findState(i,self.terminals.at(j)) << " | ";
//				}else{
//					out << "  | ";
//				}
//			}
//			for(int j=0;j<self.nonterminals.size();j++){
//				if (self.includedInItemset(self.nonterminals.at(j),i)){
//					out << self.findState(i,self.nonterminals.at(j)) << " | ";
//				}else{
//					out << "  | ";
//				}
//			}
//			out << std::endl;
//	}
//	out << std::endl;
	out << "====[Parse Table Output]====\n";
		for (int k =0;k<self.actionTable.size();k++){
			for (int i=0;i<self.actionTable.at(k).size();i++){
				out << "" << self.actionTable.at(k).at(i) << "|";
			}
			out << std::endl;
		}

//	std::cout << "====[Stack Output]====\n";
//	std::stack<int> copy = self.theStack;
//	for (int i=0;i< self.theStack.size();i++){
//		std::cout << copy.top() << std::endl;
//		copy.pop();
//	}
    return out;
}

int LRParser::dotIndex(std::string str) const{
	//A private function that will return the index of the string where the '.' is
	int returnIndex = -1;
	char c = '.';
	for (int i=0; i<str.length(); i++){
		if (str[i] == c){
			returnIndex = i;
		}
	}
	return returnIndex;
}

bool LRParser::completedDot(){
	char dot = '.';
	for (int i=1;i<grammar.size();i+=2){
		if (grammar.at(i).back() == dot){
			continue;
		}else{
			return false;
		}
	}
	return true;
}

bool LRParser::includedInItemset(std::string str, int set,bool withDot) const{
	if(withDot){
		for (int i=1;i<itemSet.at(set).rules.size();i+=2){
				int index = dotIndex(itemSet.at(set).rules.at(i)) + 1;
				if(itemSet.at(set).rules.at(i)[index] == str[0]){
					return true;
				}
			}
			return false;
	}else{
		for (int i=1;i<itemSet.at(set).rules.size();i+=2){
			if(itemSet.at(set).rules.at(i) == str){
					return true;
			}
		}
		return false;
	}
}

bool LRParser::isAcceptstate(int set,std::string rule){
	for (int i=1;i<itemSet.at(set).rules.size();i+=2){
		if(itemSet.at(set).rules.at(i) == rule){
			return true;
		}
	}
	return false;
}

int LRParser::isPresentInOtherState(std::string str){
	for (int i=0;i<states.size();i++){
		if (states.at(i).following == str){
			return i;
		}
	}
	return -1;
}

int LRParser::findState(int setNumber,std::string str) const{
	for (int i=0;i<states.size();i++){
		if (states.at(i).setNumber == setNumber and states.at(i).following == str){
			return states.at(i).stateNumber;
		}
	}
	for (int i=0;i<states.size();i++){
		if (states.at(i).following == str){
			return states.at(i).stateNumber;
		}
	}
	return 0;
}

std::string LRParser::findCorrectAction(int state,std::string symbol)const{
	//std::cout << "find action for state " << state << " with symbol "<< symbol<< std::endl;
	for (int i=0;i<actionTable.size();i++){
		//std::cout << actionTable.at(i).at(0) << std::endl;
		if(actionTable.at(i).at(0) == symbol){
			//std::cout << "Action : "<<"|" <<actionTable.at(i).at(state+1) << "|"<<std::endl;
			return actionTable.at(i).at(state+1);
		}
		continue;
	}
	return "";
}

std::string LRParser::findReductionRule(int ruleNumber)const{
	for(int i=0;i<grammarDefault.size();i++){
		//std::cout << i << " =? " << ruleNumber << std::endl;
		if ((i+1)/2==ruleNumber){
			//std::cout << "Rule=" << grammarDefault.at(i-1) << "->" << grammarDefault.at(i)<< std::endl;
			return grammarDefault.at(i-1);
		}
	}
	return "";
}

void LRParser::findRemainingRules(std::vector <std::string>& input, char followingToken){
	for (int i=0;i<grammar.size();i+=2){
		if (followingToken == grammar.at(i)[0]){
			std::string newrightsiderule = grammar.at(i+1);
			int index = dotIndex(newrightsiderule);
			newrightsiderule.erase(newrightsiderule.begin()+index);
			newrightsiderule = "." + newrightsiderule;
			input.push_back(grammar.at(i));
			input.push_back(newrightsiderule);
		}
	}
};

void LRParser::makeItemsets(){
	//Initial adding of the dots
		grammar.insert(grammar.begin(),grammar.at(0));
		grammar.insert(grammar.begin(),"S'");
		grammar.at(1) = "." + grammar.at(1);
		for (int i = 3;i<grammar.size();i+=2){
			grammar.at(i) ="."+grammar.at(i);
		}
		Set emptySet;
		Set newItemset;
		newItemset.name = 0;
		newItemset.rules = grammar;
		itemSet.push_back(newItemset);

		newItemset = emptySet;

		//Second dot adding
		//std::cout << completedDot() << std::endl;
			//Make the item sets for the terminals
			for (int i=2;i<terminals.size();i++){
				newItemset.name= itemSet.size();
				for (int j=1;j<grammar.size();j+=2){
					if (grammar.at(j).find(terminals.at(i)) != std::string::npos and dotIndex(grammar.at(j)) != grammar.at(j).size()-1){
						//Terminals occurs in the rule and the dot isn't the last character of the rule
						int index = dotIndex(grammar.at(j));
						//std::cout << "Found the right terminal:" << grammar.at(j) << " : " << terminals.at(i) <<"\n";
						//std::cout << grammar.at(j) << std::endl;
						grammar.at(j).insert(index+2,".");
						//std::cout << grammar.at(j) << std::endl;
						grammar.at(j).erase(grammar.at(j).begin()+index);
						//std::cout << "Found the right terminal:" << grammar.at(j) << " : " << terminals.at(i) <<"\n";
						newItemset.rules.push_back(grammar.at(j-1));
						newItemset.rules.push_back(grammar.at(j));
					}
				}
				if (newItemset.rules.size() !=0){
					itemSet.push_back(newItemset);
					newItemset = emptySet;
				}
			}
			//Make the item sets for the nonterminals
			for (int i=0;i<nonterminals.size();i++){
				newItemset.name= itemSet.size();
				for (int j=1;j<grammar.size();j+=2){
					if (grammar.at(j).find(nonterminals.at(i)) != std::string::npos and dotIndex(grammar.at(j)) != grammar.at(j).size()-1){
						//Nonterminals occurs in the rule and the dot isn't the last character of the rule
						int index = dotIndex(grammar.at(j));
						if(grammar.at(j)[index+1] == nonterminals.at(i)[0]){
							//std::cout << "Found the right terminal:" << grammar.at(j) << " : " << nonterminals.at(i) <<"\n";
							//std::cout << grammar.at(j) << std::endl;
							grammar.at(j).insert(index+2,".");
							//std::cout << grammar.at(j) << std::endl;
							grammar.at(j).erase(grammar.at(j).begin()+index);
							//std::cout << "Found the right terminal:" << grammar.at(j) << " : " << nonterminals.at(i) <<"\n";
							newItemset.rules.push_back(grammar.at(j-1));
							newItemset.rules.push_back(grammar.at(j));
						}
					}
				}
				if (newItemset.rules.size() !=0){
					itemSet.push_back(newItemset);
					newItemset = emptySet;
				}
			}
			while (!completedDot()){
				for (int i=1;i<grammar.size();i+=2){
					int index = dotIndex(grammar.at(i));
					newItemset = emptySet;
					if ((grammar.at(i)[index+1] == '+' or grammar.at(i)[index+1] == '*') and index != grammar.at(i).size()-1){
						newItemset.name = itemSet.size();
						//std::cout << "Before: " << grammar.at(i) << "\n";
						grammar.at(i).insert(index+2,".");
						grammar.at(i).erase(grammar.at(i).begin()+index);
						//std::cout << "After: "<<grammar.at(i) << "\n";
						std::vector <std::string> newVec;
						newVec.push_back(grammar.at(i-1));
						newVec.push_back(grammar.at(i));
						findRemainingRules(newVec, grammar.at(i)[index+2]);
						newItemset.rules = newVec;
						itemSet.push_back(newItemset);
						continue;
					}else{
						if(index != grammar.at(i).size()-1){
							//std::cout << grammar.at(i) << std::endl;
							newItemset.name = itemSet.size();
							grammar.at(i).insert(index+2,".");
							grammar.at(i).erase(grammar.at(i).begin()+index);
							newItemset.rules.push_back(grammar.at(i-1));
							newItemset.rules.push_back(grammar.at(i));
							itemSet.push_back(newItemset);
							continue;
						}
					}
				}
			}
			for (int i=0;i<itemSet.size();i++){
					for(int j=0;j<terminals.size();j++){
						if (includedInItemset(terminals.at(j),i)){
							if(isPresentInOtherState(terminals.at(j))!=-1){
								itemSet.at(i).state = states.at(isPresentInOtherState(terminals.at(j))).stateNumber;
								//std::cout << "Itemset: " << i << " ==> "<< terminals.at(j) << "->" << itemSet.at(i).state << "\n";
							}else{
								State newState;
								newState.stateNumber = states.size()+1;
								newState.following = terminals.at(j);
								newState.setNumber = i;
								states.push_back(newState);
								itemSet.at(i).state = newState.stateNumber;
								//std::cout << "Itemset: " << i << " ==> "<< terminals.at(j) << "->" << itemSet.at(i).state << "\n";
							}
						}
					}
					for(int j=0;j<nonterminals.size();j++){
						if (includedInItemset(nonterminals.at(j),i)){
							State newState;
							newState.stateNumber = states.size()+1;
							newState.following = nonterminals.at(j);
							newState.setNumber = i;
							states.push_back(newState);
							itemSet.at(i).state = newState.stateNumber;
				 			//std::cout << "Itemset: " << i << " ==> "<< nonterminals.at(j) << "->" << itemSet.at(i).state << "\n";
						}
					}
			}
}

/// Method for parsing the input CFG xml file
void LRParser::parseXML(std::string inputfile, std::string inputString){
	TiXmlDocument cfg;
	cfg.LoadFile(inputfile);
	TiXmlElement* root = cfg.FirstChildElement();
	if (root == NULL or root->FirstChildElement() == NULL){
			std::string error = "The file " + std::string(inputfile) + " is not valid: there is no root element or there are no children.\n";
			std::cout << error << std::endl;
	} else {
		//std::cout << "Parsing the file: " << inputfile <<"\n";
		//std::cout << root->Value() << std::endl;
		TiXmlElement* variables = root->FirstChildElement();
		//std::cout << "Variables = "<< variables->GetText() << std::endl;
		std::string nonterminals_str = variables->GetText();
		//TODO Change:startSymbol is not used right now, (startsymbol is expressed first in xml)
		TiXmlElement* startSymbol = variables->FirstChildElement();
		std::string startSymbolTxt = startSymbol->GetText();
		//std::cout << "Start = "<< startSymbolTxt << std::endl;
		nonterminals.push_back(startSymbolTxt);
		for (int i=0;i<nonterminals_str.size();i++){
			char k = nonterminals_str.at(i);
			std::string str = "";
			str+= k;
			if(nonterminals_str.at(i)!=' ' and nonterminals_str.at(i)!=startSymbolTxt.at(0)){//String can't be empty or the startsymbol (it is already added)
				nonterminals.push_back(str);
			}
		}
		TiXmlElement* nonvariables = variables->NextSiblingElement();
		std::string terminals_str = nonvariables->GetText();
		//std::cout << "Terminals = "<< nonvariables->GetText() << std::endl;
		for (int i=0;i<terminals_str.size();i++){
			char k = terminals_str.at(i);
			std::string str = "";
			str+= k;
			terminals.push_back(str);
		}
		terminals.push_back("$");
		TiXmlElement* productions = nonvariables->NextSiblingElement();
		//std::cout << "Productions = " << std::endl;
		for (TiXmlElement* rule= productions->FirstChildElement(); rule != NULL ; rule = rule->NextSiblingElement()) {
			std::string leftSide = rule->Attribute("LHS");
			std::string rightSide = rule->Attribute("RHS");
			std::string newRule="";
			for (int i=0;i<rightSide.size();i++){
				if(rightSide[i]!= '|'){
					newRule+=rightSide.at(i);
					if(i+1==rightSide.size()){
						//std::cout << leftSide << " -> " <<newRule << std::endl;
						grammar.push_back(leftSide);
						grammar.push_back(newRule);
						newRule = "";
					}
				}else{
					//std::cout << leftSide << " -> " <<newRule << std::endl;
					grammar.push_back(leftSide);
					grammar.push_back(newRule);
					newRule = "";
				}
			}
		}
		input = inputString;
		inputDefault = input;
		input+="$";
		//std::cout << input << std::endl;
		grammarDefault = grammar;
	}
}

void LRParser::checkStringConsistency()const{
	for (int i=0;i<inputDefault.size();i++){
		bool isTerminal = false;
		for (int j=0;j<terminals.size();j++){
			if (inputDefault.at(i) == terminals.at(j)[0]){
				isTerminal = true;
				break;
			}
		}
		if(isTerminal==false){
			throw std::runtime_error("String contains non known terminals!!!\n");
		}
	}
}

void LRParser::makeParseTable(){
	//std::cout << "1)Making the parsetable\n";
	makeItemsets();
	makeGotoTable();
	for(int j=0;j<terminals.size();j++){
		std::vector<std::string> newVec;
		newVec.push_back(terminals.at(j));
		actionTable.push_back(newVec);
	}
	for (int i=0;i<itemSet.size();i++){
			for(int j=0;j<terminals.size();j++){
				if (includedInItemset(terminals.at(j),i)){
					actionTable.at(j).push_back("  s"+std::to_string(findState(i,terminals.at(j))));
				}else{
					if(terminals.at(j) == "$"){
						std::string startSymbol = grammar.at(2) + "."; //In order to find acceptstate check if itemset contains the S->E. rule
						if(isAcceptstate(i,startSymbol)){
							actionTable.at(j).push_back(" acc");
						}else{
							//Reduce actions
							int position = 1;
							bool isFilled = false;
							for(int k=0;k<grammarDefault.size()/2;k++){
								//std::cout << j << position << std::endl;
								//std::cout << grammarDefault.at(position) << " " << i << "\n";
								if(includedInItemset(grammarDefault.at(position)+".",i,false)){
									actionTable.at(j).push_back("  r" + std::to_string(k+1));
									position+=2;
									isFilled =true;
									continue;
								}
								position+=2;
							}
							if(isFilled == false){
								actionTable.at(j).push_back("    ");
							}
						}
					}else{
						//Reduce actions
						int position = 1;
						bool isFilled = false;
						for(int k=0;k<grammarDefault.size()/2;k++){
							//std::cout << j << position << std::endl;
							//std::cout << grammarDefault.at(position) << " " << i << "\n";
							if(includedInItemset(grammarDefault.at(position)+".",i,false)){
								actionTable.at(j).push_back("  r" + std::to_string(k+1));
								position+=2;
								isFilled =true;
								continue;
							}
							position+=2;
						}
						if(isFilled == false){
							actionTable.at(j).push_back("    ");
						}
					}
				}
			}
	}
	for (int i=0;i<gotoTable.size();i++){
		actionTable.push_back(gotoTable.at(i));
	}
}

void LRParser::makeGotoTable(){
	for(int j=0;j<nonterminals.size();j++){
		std::vector<std::string> newVec;
		newVec.push_back(nonterminals.at(j));
		gotoTable.push_back(newVec);
	}
	for (int i=0;i<itemSet.size();i++){
			for(int j=0;j<nonterminals.size();j++){
				if (includedInItemset(nonterminals.at(j),i)){
					gotoTable.at(j).push_back("   "+std::to_string(findState(i,nonterminals.at(j))));
				}else{
					gotoTable.at(j).push_back("    ");
				}
			}
	}
}

void LRParser::parseAlgorithm(){
	std::cout << "Parsing the string " << input << std::endl;
	theStack.push(0);
	bool isParsed = false;
	std::string inputSymbol = "";
	std::string action = "";
	std::string rightmostDerivation = "";
	while(!isParsed){
		inputSymbol = "";
		inputSymbol += input.at(0);
		action = findCorrectAction(theStack.top(),inputSymbol);
		//====[Accept action]====//
		if (findCorrectAction(theStack.top(), inputSymbol) == " acc"){
			isParsed = true;
			break;
		}
		//===[Shift action]===//
		if(checkRule(action) == "s"){
			//std::cout << action << std::endl;
			input.erase(input.begin());
			int newState = std::stoi(stringToInteger(action)); //Converts the char to the integer to become the state
			theStack.push(newState);
			continue;
		}
		//===[Reduce action]===//
		if(checkRule(action) == "r"){
			int replaceRule = std::stoi(stringToInteger(action)); //Converts the char to the integer to become the new replacement rule
			theStack.pop();
			std::string reductionRule = findReductionRule(replaceRule);
			rightmostDerivation += std::to_string(replaceRule);
			action = findCorrectAction(theStack.top(),reductionRule);
			if(action == "    "){
				theStack.pop();
				continue;
			}
			// if the string doesn't contain a number just pop state from stack
			if (stringToInteger(action) == ""){
				theStack.pop();
				continue;
			}
			int newState = std::stoi(stringToInteger(action));
			theStack.push(newState);
			continue;
		}
		if(action == "    "){
			break;
		}
	}

	//====[Parsing is done]====//
	if(isParsed == true){
		std::cout << "The string " << inputDefault << " is ACCEPTED by the CFG.\n";
		//std::cout << "Righmost derivation: " << rightmostDerivation << std::endl;
	}else{
		std::cout << "The string " << inputDefault << " is NOT ACCEPTED by the CFG.\n";
	}
}

// Convert the string with the rule to an integer
std::string LRParser::stringToInteger(const std::string& input_string)const{
	std::string returnVal = "";
	for (int i=0;i<input_string.size();i++){
		// if the character is non integer don't add it to returnVal
		if (input_string.at(i) == 's' or input_string.at(i) == 'r') continue;
		returnVal+=input_string.at(i);
	}
	return returnVal;
}

// Returns s if input_string is a shiftrule and else it returns an r
std::string LRParser::checkRule(const std::string& input_string)const{
	std::string returnVal = "";
	for (int i=0;i<input_string.size();i++){
		// if the character is non integer don't add it to returnVal
		if (input_string.at(i) == 's' or input_string.at(i) == 'r'){
			returnVal+=input_string.at(i);
			break;
		}
	}
	return returnVal;
}

void LRParser::parseLR(int k){
	std::cout << "Start of the LR parse algorithm\n";
	makeParseTable();
	//std::cout << *this << std::endl;
	parseAlgorithm();
	//std::cout << *this << std::endl;

}
