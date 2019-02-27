#ifndef RULE
#define RULE

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "Predicate.h"

using namespace std;

class Rule
{
private:
	vector<Predicate*> predicate_list;

public:
	Rule()
	{

	}
	virtual ~Rule()
	{
		for (unsigned int i = 0; i < predicate_list.size(); i++)
		{
			delete predicate_list.at(i);
		}
	}

	void addPredicate(Predicate* myPredicate)
	{
		predicate_list.push_back(myPredicate);
	}

	string to_string()
	{
		stringstream ss;
		string temp;
		temp = predicate_list.at(0)->rule_to_string();
		temp.erase(temp.begin() + temp.size() - 1);
		ss << temp << " :- ";
		for (unsigned int i = 1; i < predicate_list.size(); i++)
		{
			if (i != predicate_list.size() - 1)
			{
				ss << predicate_list.at(i)->rule_to_string();
			}
			else
			{
				temp = predicate_list.at(i)->rule_to_string();
				temp.at(temp.size() - 1) = '.';
				ss << temp;
			}
		}
		return ss.str();;
	}
};

#endif
