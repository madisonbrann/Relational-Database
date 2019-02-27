#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <locale>   
#include <vector>

#include "Tokenizer.h"

Tokenizer::Tokenizer()
{

}

Tokenizer::~Tokenizer()
{

}


using namespace std;

vector<Token*> token_vector;
bool creating_string = false;
bool creating_single_comment = false;
bool creating_multi_comment = false;
bool after_hash = false;
bool second_stick_thing = false;
bool apostrophe_present = false;
bool first_quote = false;
bool not_multi_comment = false;
int starter_line = 0;
int total_tokens = 0;
int increase_line_num;
int start_line_num = 0;
string ID_Output = "";
string string_output = "";
string comment_output = "";
stringstream ss;


void Tokenizer::printToken(string type, string output, int line_num)
{
	//	cout << "(" << type << ",\"" << output << "\"," << line_num << ")" << endl;
	total_tokens++;
	if (type != "COMMENT")
	{
		Token* token = new Token(type, output, line_num);
		token_vector.push_back(token);
	}
}


bool Tokenizer::checkForToken(char output, int& line_num, char next_char, unsigned int& i, string& item_holder, string& line_holder, ifstream& in)
{
	bool token_present = false;
	bool found_quote_two = false;
	string type;
	switch (output)
	{
	case '?': type = "Q_MARK";
		token_present = true;
		break;
	case '.': type = "PERIOD";
		token_present = true;
		break;
	case ',': type = "COMMA";
		token_present = true;
		break;
	case '(': type = "LEFT_PAREN";
		token_present = true;
		break;
	case ')': type = "RIGHT_PAREN";
		token_present = true;
		break;
	case ':': if (next_char == '-' && creating_multi_comment != true && creating_single_comment != true && creating_string != true)
	{
		type = "COLON_DASH";
		i++; //increment in read data if use next_char
		token_present = true;
		break;
	}
			  type = "COLON";
			  token_present = true;
			  break;

	case '*': type = "MULTIPLY";
		token_present = true;
		break;
	case '+': type = "ADD";
		token_present = true;
		break;
	case '\'': if (creating_multi_comment || creating_single_comment == true) break;

		if (creating_string == true)
		{
			if (apostrophe_present == true)
			{
				apostrophe_present = false;
				break;
			}

			if (next_char == '\'') //in case of '' or apostrophe
			{
				apostrophe_present = true;
				break;
			}

			found_quote_two = true;
			token_present = true;
			creating_string = false;
			string_output += output;
			break;
		}
		else
		{
			starter_line = line_num;
			creating_string = true;
			start_line_num = line_num;
			token_present = true;
			break;
		}

	case '#': if (creating_string == true || creating_multi_comment == true) break;

		if (creating_single_comment == true && next_char == '\n')
		{
			printToken("COMMENT", comment_output, line_num);
			not_multi_comment = false;
			comment_output = "";
			creating_single_comment = false;
			creating_multi_comment = false;
		}

		if (next_char != '|')
		{
			not_multi_comment = true;
		}
		creating_single_comment = true;
		start_line_num = line_num;
		after_hash = true;
		token_present = true;
		break;
	case '|': if (not_multi_comment == true) break;
		if (creating_multi_comment == true && next_char == '#')
		{
			second_stick_thing = true;

			i++;
		}
		if (second_stick_thing == true)
		{
			creating_single_comment = false;
			creating_multi_comment = false;
			second_stick_thing = false;
			comment_output += output;
			comment_output += '#';
			printToken("COMMENT", comment_output, start_line_num);
			not_multi_comment = false;
			start_line_num = 0;
			comment_output = "";
			token_present = true;
			return true;
		}
		if (after_hash == true) //watch out if symbol before ISN'T '#'
		{
			creating_multi_comment = true;
			creating_single_comment = false;
			start_line_num = line_num;
		}

		after_hash = false;
		break;
	}

	if (creating_single_comment == true || creating_multi_comment == true)
	{
		comment_output += output;
		return true;
	}


	if (creating_string == true)
	{
		string_output += output;
		after_hash = false;
		return true;
	}
	if (found_quote_two == true)
	{
		printToken("STRING", string_output, starter_line);
		creating_string = false;
		string_output = "";
		start_line_num = 0;
		after_hash = false;
		return true;
	}
	if (ID_Output != "" && token_present == true)//if anything is in ID_Output after iterating through item
	{
		if (ID_Output == "Facts")
		{
			printToken("FACTS", ID_Output, line_num);
			ID_Output = "";
		}
		else if (ID_Output == "Schemes")
		{
			printToken("SCHEMES", ID_Output, line_num);
			ID_Output = "";
		}
		else if (ID_Output == "Queries")
		{
			printToken("QUERIES", ID_Output, line_num);
			ID_Output = "";
		}
		else if (ID_Output == "Rules")
		{
			printToken("RULES", ID_Output, line_num);
			ID_Output = "";
		}
		else
		{
			printToken("ID", ID_Output, line_num);
			ID_Output = "";
		}
	}
	if (token_present == true)
	{
		if (type == "COLON_DASH")
		{
			printToken(type, ":-", line_num);
		}

		else //whenever there is a token present that isn't a weird case?
		{
			string s = "";
			printToken(type, s += output, line_num);
		}
		after_hash = false;
		return true;
	}

	else
	{
		after_hash = false;
		return false;
	}
}

vector<string> Tokenizer::createSpaceVector(string line)
{
	vector<string> space_vector;
	stringstream ss;
	string output;
	bool past_initial_space = false;
	bool start_space = false;
	for (unsigned int i = 0; i < line.size(); i++)
	{
		if (line.at(i) != ' ' && line.at(i) != '\t')
		{
			past_initial_space = true;
		}
		if (past_initial_space == true)
		{
			if (line.at(i) == ' ')
			{
				start_space = true;
				output += " ";
			}
			if (line.at(i) == '\t')
			{
				start_space = true;
				output += '\t';
			}
			if (line.at(i) != ' ')
			{

				if (start_space == true)
				{
					space_vector.push_back(output);
					output = "";
				}
				start_space = false;
			}
		}
	}
	/*
	for (unsigned int i = 0; i < space_vector.size(); i++)
	{
	cout << space_vector.at(i).size() << endl;;
	}
	cout << endl;
	*/
	return space_vector;
}

void Tokenizer::readData(ifstream& in)
{
	locale loc;
	string line_holder;
	string item_holder;
	vector<string> space_vector;
	int line_num = 1;
	vector<char> vector1;
	bool ID_started = false;
	char next_char;
	unsigned int space_location_id;

	while (getline(in, line_holder)) // goes through each line
	{
		space_vector = createSpaceVector(line_holder);
		ss << line_holder;
		space_location_id = 0;
		while (ss >> item_holder) // goes through each item in line
		{
			for (unsigned int i = 0; i < item_holder.size(); i++) //go through each char in item
			{
				if (i + 1 < item_holder.size())
				{
					next_char = item_holder.at(i + 1); //access the next character
				}
				else
				{
					next_char = '\0';
				}
				if (checkForToken(item_holder.at(i), line_num, next_char, i, item_holder, line_holder, in) != true)
				{
					if (item_holder.size() != 0) //in case of string function deleting entire item
					{
						if (isalpha(item_holder.at(i))) //will start ID ouput if letter
						{
							ID_Output += item_holder.at(i);
							ID_started = true;
						}
						else if (isdigit(item_holder.at(i), loc) == true && ID_started == true) //Add numbers to ID output if ID is already started
						{
							ID_Output += item_holder.at(i);
						}
						else //outputs undefined characters here
						{
							printToken("UNDEFINED", string(1, item_holder.at(i)), line_num);
						}
					}
				}

			}

			if (ID_Output != "") //if anything is in ID_Output after iterating through item
			{
				if (ID_Output == "Facts")
				{
					printToken("FACTS", ID_Output, line_num);
					ID_Output = "";
				}
				else if (ID_Output == "Schemes")
				{
					printToken("SCHEMES", ID_Output, line_num);
					ID_Output = "";
				}
				else if (ID_Output == "Queries")
				{
					printToken("QUERIES", ID_Output, line_num);
					ID_Output = "";
				}
				else if (ID_Output == "Rules")
				{
					printToken("RULES", ID_Output, line_num);
					ID_Output = "";
				}
				else
				{
					printToken("ID", ID_Output, line_num);
					ID_Output = "";
				}
			}
			if (creating_string == true)
			{
				if (space_location_id < space_vector.size())
					string_output += space_vector.at(space_location_id);
			}
			if (creating_multi_comment == true || creating_single_comment == true)
			{
				if (space_location_id < space_vector.size())
					comment_output += space_vector.at(space_location_id);
			}
			space_location_id++;
		}
		ss.clear();
		line_num++;
		if (creating_single_comment == true)
		{
			creating_single_comment = false;
			if (comment_output.at(comment_output.size() - 1) == ' ') comment_output.pop_back();
			printToken("COMMENT", comment_output, start_line_num);
			not_multi_comment = false;
			creating_single_comment = false;
			creating_multi_comment = false;
			comment_output = "";
			start_line_num = 0;
		}
		if (creating_multi_comment == true)
		{
			comment_output += '\n';
		}
		if (creating_string == true)
		{
			string_output += '\n';
		}
	}
	if (creating_string == true)
	{
		printToken("UNDEFINED", string_output, starter_line);
	}
	if ((creating_multi_comment) == true)
	{
		printToken("UNDEFINED", comment_output, start_line_num);
	}
	printToken("EOF", "", line_num);
	//	cout << "Total Tokens = " << total_tokens << endl;
}

vector<Token*> Tokenizer::herewego(ifstream& in)
{
	readData(in);
	return token_vector;
}