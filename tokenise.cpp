#include "functions.h"

//this function reads the text from the given file line by line
//and stores them in one string(side by side)
std::string make_str (std::ifstream &filename)
{
	std::string str;
        while(!filename.eof()) {
                std::string temp;
		std::getline(filename, temp);
		if(filename.eof()) {
			break;
		}
		for(int i = 0; i < temp.length(); i++) {
			if(temp[i] == ';') {
                        	break;
                	}
			str += temp[i];
        	}
	}
	return str;
}

//this function checks if the parentheses are balanced in the given string
bool balance(std::string expr)
{ 
	int count = 0;
	std::stack<char> temp;
        for(int i=0;i<expr.length();i++)
        {
            if(expr[i] == '(')
            {
		count++;
                temp.push(expr[i]);
            }
            else if(!temp.empty() && temp.top()=='('&& expr[i]==')')
            {
                temp.pop();
            }
	    if(expr[i] == ')')
	    {
		    count--;
	    }
        }
        if(temp.empty() && count == 0) 
        {
            return true;
        }
        return false;
}

//this function returns the string(inside quotes) from the given string
//starting from the i index
std::string text(const std::string &str,int &i)
{
        std::string t = "";
        t += str[i];
        i++;
        while(i + 1 < str.length() && !(str[i] == '"' && str[i - 1] != '\\')) {
                t += str[i];
                i++;
        }
        if(str[i] == '"') {
                t += str[i];
                i++;
        }
        return t;
}

//this function addes spaces before and after all the parentheses in the string
void add_space(std::string &str) 
{
	for(int i = 0; i < str.length(); i++) {
		if(str[i] == '(' || str[i] == ')') {
			str.insert(i+1, " ", 1);
		}
	}
	for(int i = str.length() - 1; i >= 0; i--) {
                if(str[i] == ')' || str[i] == '(') {
                        str.insert(i, " ", 1);
                }
        }
}	

//this function splites the given string by spaces and returns the vector of strings
std::vector<std::string> split(std::string &line)
{
	std::vector<std::string> tokens;
	int i = 0;
	bool b;
	std::string temp;
	while(i < line.length()) {
		b = false;
		temp = "";
		while(line[i] != ' ') {
			b = true;
			if(line[i] == '"') {
				temp = text(line, i);
			}
			else {
				temp += line[i];
				i++;
			}
		}
		if (temp != "") {
			tokens.push_back(temp);
		}
		if(!b) {
			i++;
		}
	}
	return tokens;

}

//this function receives a vector of strings and 
//an index of an opening bracket, and returns the index of
//the corresponding closing bracket
int find_closing(std::vector<std::string> items, int i) 
{
	int count = 0;
	for(int j = i + 1; j < items.size(); j++) {
		if(items[j] == "(") {
			count++;
		} else if(items[j] == ")") {
			count--;
		}
		if(count == 0) {
			return j;
		}
	}
	return -1;
}

//this function checks if there are any nested parentheses inside 
//the given vector of strings 
bool is_nested(std::vector<std::string> items)
{
	for(int i = 0; i < items.size(); i++) {
		if(items[i] == "(") {
			return true;
		}
	}
	return false;
}
