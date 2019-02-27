//vulnerability with ID in parameter.h, only takes single id, doesn't account for strings or anything longer then one char

#include "DatalogParser.h"
#include "Predicate.h"
#include "Parameter.h"
#include "Rule.h"
#include <iostream>
#include <algorithm>
#include <sstream>
using namespace std;

vector<Predicate*> SchemePredicates;
vector<Predicate*> FactPredicates;
vector<Rule*> RulePredicates;
vector<Predicate*> QueriePredicates;
vector<string> domain_holder;
bool creating_expression = false;
stringstream the_expression;
int recursion_level = 0;

DatalogParser::DatalogParser(vector<Token*> token_vector_in)
{
	token_vector = token_vector_in;
	token_number = 0;

}
DatalogParser::~DatalogParser()
{
	for (unsigned int i = 0; i < token_vector.size(); i++)
	{
		delete token_vector.at(i);
	}
	for (unsigned int i = 0; i < FactPredicates.size(); i++)
	{
		delete FactPredicates.at(i);
	}
	for (unsigned int i = 0; i < RulePredicates.size(); i++)
	{
		delete RulePredicates.at(i);
	}
	for (unsigned int i = 0; i < SchemePredicates.size(); i++)
	{
		//		delete SchemePredicates.at(i);
	}
	for (unsigned int i = 0; i < QueriePredicates.size(); i++)
	{
		delete QueriePredicates.at(i);
	}
}



void DatalogParser::match(string item)
{
	if (token_vector.at(token_number)->get_type() != item)
	{
		throw string("break");
	}
	token_number++;
}
void DatalogParser::operator_thing(Parameter* myParameter)
{
	if (token_vector.at(token_number)->get_type() == "ADD")
	{
		if (creating_expression != true)myParameter->addString(token_vector.at(token_number)->get_output());
		else the_expression << token_vector.at(token_number)->get_output();
		match("ADD");
	}
	else
	{
		if (creating_expression != true)myParameter->addString(token_vector.at(token_number)->get_output());
		else the_expression << token_vector.at(token_number)->get_output();
		match("MULTIPLY");
	}
}
void DatalogParser::expression(Parameter* myParameter)
{
	if (recursion_level == 0)
	{
		myParameter->addString("");
	}
	recursion_level++;
	creating_expression = true;
	the_expression << "(";
	match("LEFT_PAREN");
	parameter(myParameter);
	operator_thing(myParameter);
	parameter(myParameter);
	the_expression << ")";
	match("RIGHT_PAREN");
	//creating_expression = false;
	recursion_level--;
	if (recursion_level == 0)
	{
		creating_expression = false;
		myParameter->updateExpression(the_expression.str());
	}
	else
	{
		myParameter->updateExpression(the_expression.str());
	}
	the_expression.str("");
	the_expression.clear();
}
void DatalogParser::parameter(Parameter* myParameter)
{
	if (token_vector.at(token_number)->get_type() == "STRING")
	{
		if (creating_expression != true)myParameter->addString(token_vector.at(token_number)->get_output());
		else
		{
			the_expression << token_vector.at(token_number)->get_output();
		}
		match("STRING");
	}
	else if (token_vector.at(token_number)->get_type() == "ID")
	{
		if (creating_expression != true)myParameter->addString(token_vector.at(token_number)->get_output());
		else
		{
			the_expression << token_vector.at(token_number)->get_output();
		}
		match("ID");
	}
	else
	{
		expression(myParameter);
	}
}
void DatalogParser::idList(Parameter* myParameter)
{
	if (token_vector.at(token_number)->get_type() == "RIGHT_PAREN")
	{
		//escape idList
	}
	else
	{
		match("COMMA");
		myParameter->addString(token_vector.at(token_number)->get_output());
		match("ID");
		idList(myParameter);
	}
}
void DatalogParser::stringList(Parameter* myParameter)
{
	if (token_vector.at(token_number)->get_type() == "RIGHT_PAREN")
	{
		//escape stringList
	}
	else
	{
		match("COMMA");
		myParameter->addString(token_vector.at(token_number)->get_output());
		domain_holder.push_back(token_vector.at(token_number)->get_output());
		match("STRING");
		stringList(myParameter);
	}
}
void DatalogParser::parameterList(Parameter* myParameter)
{
	if (token_vector.at(token_number)->get_type() == "RIGHT_PAREN")
	{
		//escape parameterList
	}
	else
	{
		match("COMMA");
		parameter(myParameter);
		parameterList(myParameter);
	}
}
void DatalogParser::predicateList(Rule* myRule)
{
	if (token_vector.at(token_number)->get_type() == "PERIOD")
	{
		//escape predicateList
	}
	else
	{
		match("COMMA");
		predicate(myRule);
		predicateList(myRule);
	}
}

void DatalogParser::queryPredicate()
{
	Predicate* myPredicate = new Predicate(token_vector.at(token_number)->get_output());
	match("ID");
	match("LEFT_PAREN");
	Parameter* myParameter = new Parameter(token_vector.at(token_number)->get_output());
	myPredicate->addParameter(myParameter);
	parameter(myParameter);
	parameterList(myParameter);
	match("RIGHT_PAREN");
	myParameter->removeFirstString();
	QueriePredicates.push_back(myPredicate);
}

void DatalogParser::predicate(Rule* myRule)
{
	Predicate* myPredicate = new Predicate(token_vector.at(token_number)->get_output());
	match("ID");
	match("LEFT_PAREN");
	Parameter* myParameter = new Parameter(token_vector.at(token_number)->get_output());
	myPredicate->addParameter(myParameter);
	parameter(myParameter);
	parameterList(myParameter);
	match("RIGHT_PAREN");
	myParameter->removeFirstString();
	myRule->addPredicate(myPredicate);
}
void DatalogParser::headPredicate(Rule* myRule)
{
	Predicate* myPredicate = new Predicate(token_vector.at(token_number)->get_output());
	//QueriePredicates.push_back(myPredicate);
	match("ID");
	match("LEFT_PAREN");
	Parameter* myParameter = new Parameter(token_vector.at(token_number)->get_output());
	myPredicate->addParameter(myParameter);
	match("ID");
	idList(myParameter);
	myRule->addPredicate(myPredicate);
	match("RIGHT_PAREN");
}
void DatalogParser::query()
{
	queryPredicate();
	match("Q_MARK");
}
void DatalogParser::rule()
{
	Rule* myRule = new Rule();
	headPredicate(myRule);
	match("COLON_DASH");
	predicate(myRule);
	predicateList(myRule);
	match("PERIOD");
	RulePredicates.push_back(myRule);
}
void DatalogParser::fact()
{
	Predicate* myFactPredicate = new Predicate(token_vector.at(token_number)->get_output());
	match("ID");
	match("LEFT_PAREN");
	Parameter* myParameter = new Parameter(token_vector.at(token_number)->get_output());
	myFactPredicate->addParameter(myParameter);
	domain_holder.push_back(token_vector.at(token_number)->get_output());
	match("STRING");
	stringList(myParameter);
	match("RIGHT_PAREN");
	match("PERIOD");
	FactPredicates.push_back(myFactPredicate);
}
void DatalogParser::scheme()
{
	Predicate* mySchemePredicate = new Predicate(token_vector.at(token_number)->get_output());
	match("ID");
	match("LEFT_PAREN");
	Parameter* myParameter = new Parameter(token_vector.at(token_number)->get_output());
	mySchemePredicate->addParameter(myParameter);
	match("ID");
	idList(myParameter);
	match("RIGHT_PAREN");
	SchemePredicates.push_back(mySchemePredicate);
}
void DatalogParser::queryList()
{
	if (token_vector.at(token_number)->get_type() == "EOF")
	{
		//escape queryList
	}
	else
	{
		query();
		queryList();
	}
}
void DatalogParser::ruleList()
{
	if (token_vector.at(token_number)->get_type() == "QUERIES")
	{
		//escape schemeList
	}
	else
	{
		rule();
		ruleList();
	}
}
void DatalogParser::factList()
{
	if (token_vector.at(token_number)->get_type() == "RULES")
	{
		//escape factList
	}
	else
	{
		fact();
		factList();
	}
}
void DatalogParser::schemeList()
{
	if (token_vector.at(token_number)->get_type() == "FACTS")
	{
		//escape schemeList
	}
	else
	{
		scheme();
		schemeList();
	}
}
void DatalogParser::datalogProgram()
{
	match("SCHEMES");
	match("COLON");
	scheme();
	schemeList();
	match("FACTS");
	match("COLON");
	factList();
	match("RULES");
	match("COLON");
	ruleList();
	match("QUERIES");
	match("COLON");
	query();
	queryList();
}

//normal functions
bool DatalogParser::parse_tokens()
{
	try
	{
		datalogProgram();
		cout << "Success!" << endl;
	}
	catch (string e)
	{
		cout << "Failure!\n  (" << token_vector.at(token_number)->get_type() << ",\"" << token_vector.at(token_number)->get_output() << "\"," << token_vector.at(token_number)->get_line() << ")" << endl;
		return false;
	}
	return true;
}

void DatalogParser::sortDomain()
{
	sort(domain_holder.begin(), domain_holder.end());
	domain_holder.erase(unique(domain_holder.begin(), domain_holder.end()), domain_holder.end());
}

string DatalogParser::to_string()
{
	stringstream ss;

	ss << "Schemes(" << SchemePredicates.size() << "):" << endl;
	for (unsigned int i = 0; i < SchemePredicates.size(); i++)
	{
		ss << "  " << SchemePredicates.at(i)->to_string() << endl;
	}

	ss << "Facts(" << FactPredicates.size() << "):" << endl;
	for (unsigned int i = 0; i < FactPredicates.size(); i++)
	{
		ss << "  " << FactPredicates.at(i)->to_string() << "." << endl;
	}

	ss << "Rules(" << RulePredicates.size() << "):" << endl;
	for (unsigned int i = 0; i < RulePredicates.size(); i++)
	{
		ss << "  " << RulePredicates.at(i)->to_string() << endl;
	}

	ss << "Queries(" << QueriePredicates.size() << "):" << endl;
	for (unsigned int i = 0; i < QueriePredicates.size(); i++)
	{
		ss << "  " << QueriePredicates.at(i)->to_string() << "?" << endl;
	}

	sortDomain();
	ss << "Domain(" << domain_holder.size() << "):" << endl;
	for (unsigned int i = 0; i < domain_holder.size(); i++)
	{
		ss << "  " << domain_holder.at(i) << endl;
	}

	return ss.str();
}