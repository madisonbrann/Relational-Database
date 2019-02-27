#ifndef DATALOGPARSER
#define DATALOGPARSER

#include <string>
#include <vector>
#include <sstream>
#include "Token.h"
#include "Parameter.h"
#include "Rule.h"

using namespace std;

class DatalogParser
{
private:
	vector<Token*> token_vector;
	int token_number;
public:
	DatalogParser(vector<Token*> token_vector_in);
	virtual ~DatalogParser();
	// grammar functions
	void match(string item);
	void operator_thing(Parameter* myParameter);
	void expression(Parameter* myParameter);
	void parameter(Parameter* myParameter);
	void idList(Parameter* myParameter);
	void stringList(Parameter* myParameter);
	void parameterList(Parameter* myParameter);
	void predicateList(Rule* myRule);
	void queryPredicate();
	void predicate(Rule* myRule);
	void headPredicate(Rule* myRule);
	void query();
	void rule();
	void fact();
	void scheme();
	void queryList();
	void ruleList();
	void factList();
	void schemeList();
	void datalogProgram();

	//normal functions
	bool parse_tokens();
	void sortDomain();
	string to_string();
};

#endif

