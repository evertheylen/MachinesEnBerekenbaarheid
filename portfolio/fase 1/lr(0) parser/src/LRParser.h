/*
 * LRParser.h
 *
 *  Created on: 21 okt. 2015
 *      Author: AnthonyHermans
 */

#ifndef LRPARSER_H_
#define LRPARSER_H_

#include <string>
#include <iostream>
#include <stack>
#include <vector>

struct Set{
	int name;
	int state;//Mss verwijderen?
	std::vector < std::string > rules;
};

struct State{
	int stateNumber = 0;
	int setNumber = 0;
	std::string following;
};

class LRParser {
public:
	//Constructor and destructor
	LRParser() = default;
	~LRParser() = default;

	//Overloaded cout function
	friend std::ostream& operator<<(std::ostream& out, const LRParser& i);

	//The LR parse algorithm, with no lookahead symbols (default is k=0)
	void parseLR(int k=0);

	//The parse XML algorithm
	void parseXML(std::string inputfile,std::string inputString);
	
	// check if the parsed string consist of known terminals
	void checkStringConsistency()const;
private:
	// string you wan't to parse
	std::string input; // ends with an '$' symbol
	std::string inputDefault;
	
	// grammar. terminals and nonterminals
	std::vector <std::string> grammar;	// grammar that will be manipulated through the algorithm
	std::vector <std::string> grammarDefault; 	// default grammar from xml
	std::vector <std::string> terminals;
	std::vector <std::string> nonterminals;
	
	std::stack<int> theStack; // stack with state numbers
	
	// action and gototable (actiontable contains the goto table)
	std::vector<std::vector <std::string > > actionTable;
	std::vector<std::vector <std::string > > gotoTable;
	
	// items sets and the states of the DFA
	std::vector <Set> itemSet;
	std::vector <State> states;

	// construction of parseTable
	void makeParseTable();
	
	// construction of gotoTable
	void makeGotoTable();
	
	// construction of itemsets
	void makeItemsets();
	
	// algorithm to parse the string based on shift, reduce and accept
	void parseAlgorithm();
	
	// find the index of the dot in the string
	int dotIndex(std::string) const;
	
	// find if all the rules are added to all itemsets (check if all dots in the rules are at the end)
	bool completedDot();
	
	// find the remaining rules that start with the char argument
	void findRemainingRules(std::vector <std::string>&,char);
	
	// check if setNumber and the rule lead to an accept state
	bool isAcceptstate(int setNumber, std::string rule);
	
	// check if a string(rule) is included in itemset i according if the rule contains dot or not
	bool includedInItemset(std::string,int i,bool withDot=true) const;
	
	// check if rule is present in the following state
	int isPresentInOtherState(std::string);
	
	// find the state based on setNumber and string
	int findState(int setNumber, std::string str)const;
	
	// find the reduction rule for a specific rulenumber
	std::string findReductionRule(int ruleNumber)const;
	
	// find the correct action for a state and symbol in the actiontable
	std::string findCorrectAction(int state, std::string symbol)const;
	
	// convert a string(that contains non integer characters) to integer
	std::string stringToInteger(const std::string& input_string)const;
	
	// check if the rule is either r(reduce) or s(shift)
	std::string checkRule(const std::string& input_string)const;
	
};

#endif /* LRPARSER_H_ */
