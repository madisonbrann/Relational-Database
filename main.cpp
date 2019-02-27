#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <locale>   
#include <vector>

#include "Tokenizer.h"
#include "Token.h"
#include "DatalogParser.h"
#include "Predicate.h"
#include "Parameter.h"
#include "Rule.h"

using namespace std;

int main(int argc, char *argv[])
{
	vector<Token*> token_vector;
	if (argc < 2)
	{
		cerr << "Please provide name of input and output files" << endl;
		return 0;
	}

	ifstream in(argv[1]);
	Tokenizer* myTokenizer = new Tokenizer();
	token_vector = myTokenizer->herewego(in);
	DatalogParser* myDatalogParser = new DatalogParser(token_vector);
	myDatalogParser->parse_tokens();
	delete myTokenizer;
	// delete myPredicate;
	delete myDatalogParser;
	 system("pause");

	return 0;
}