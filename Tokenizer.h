#ifndef TOKENIZER
#define TOKENIZER

#include <string>
#include <vector>
#include "Token.h"

using namespace std;

class Tokenizer
{
private:

public:
	Tokenizer();
	virtual ~Tokenizer();

	string to_string();
	void printToken(string type, string output, int line_num);
	bool checkForToken(char output, int& line_num, char next_char, unsigned int& i, string& item_holder, string& line_holder, ifstream& in);
	vector<string> createSpaceVector(string line);
	void readData(ifstream& in);
	vector<Token*> herewego(ifstream& in);
};


#endif

