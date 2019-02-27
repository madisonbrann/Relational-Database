#ifndef PREDICATE
#define PREDICATE

#include <string>
#include <iostream>
#include "Parameter.h"

using namespace std;


class Predicate
{
private:
	string name;
	vector<Parameter*> content;

public:
	Predicate(string name_in)
	{
		name = name_in;
	}
	virtual ~Predicate()
	{
		for (unsigned int i = 0; i < content.size(); i++)
		{
			delete content.at(i);
		}
	}

	void addParameter(Parameter* myParameter)
	{
		content.push_back(myParameter);
	}
	string to_string()
	{
		stringstream ss;
		ss << name;
		for (unsigned int i = 0; i < content.size(); i++)
		{
			ss << content.at(i)->to_string();
		}
		return ss.str();
	}

	string rule_to_string()
	{
		stringstream ss;
		ss << name;
		for (unsigned int i = 0; i < content.size(); i++)
		{
			ss << content.at(i)->to_string() << ",";
		}
		return ss.str();
	}
};

#endif
