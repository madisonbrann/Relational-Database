#ifndef TOKEN
#define TOKEN

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Token
{
private:
	string type;
	string output;
	int line_num;

public:
	Token(string type_in, string output_in, int line_num_in)
	{
		type = type_in;
		output = output_in;
		line_num = line_num_in;
	}
	virtual ~Token()
	{

	}

	string get_type()
	{
		return type;
	}

	string get_output()
	{
		return output;
	}

	int get_line()
	{
		return line_num;
	}

	string to_string()
	{
		stringstream ss;
		ss << "(" << type << ",\"" << output << "\"," << line_num << ")" << endl;
		return ss.str();
	}
};

#endif
