#include "functions.h"

//this function gets a string(value) as an argument and determines
//the type of the object
type determine_type(std::string str)
{
	type t;
	if(str == "(") {
		 t = list;
	} 
	else if((str[0] <= '9' && str[0] >= '0') || str[0] == '\"' || str[0] == '\'' || str[0] <= '-')  {
		t = literal;
	} else {
		t = identifier;
	}
	return t;
}

//this function gets two iterators as parameters
//(positions of the opening and closing parentheses)
//and finds the position of closing bracket, corresponding to the given opening bracket
std::vector<std::string>::iterator find_closing(std::vector<std::string>::iterator beg, std::vector<std::string>::iterator end)
{
        int count = 0;
	std::vector<std::string>::iterator start = beg;
        while(start != end) {
                if(*start == "(") {
                        count++;
                } else if(*start == ")") {
                        count--;
                }
                if(count == 0) {
                        return start;
                }
		++start;
        }
	return end;
}

//constructor of an empty object
token::token()
{
	value = " ";
	args = {};
	index_of_type = list;
}

//constructor of a non_list object
token::token(std::string str)
{   	
	value = str;
	args = {};
	index_of_type = determine_type(str); 
}

//constructor of a list object, whiches objects are non_list
token::token(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end) {
	while(start != end) {
		if(*start == "(") {
			start++;
			continue;
		}
		token a(*start);
		args.push_back(a);
		start++;
	}
	index_of_type = list;
	value = "";
}

//this method returns the vector of arguments of the object
std::vector<token>& token::get_args() {
	return args;
}

void token::set_value(std::string val)
{
	value = val;
}

//this function returns the value of the object
std::string token::get_value() {
	return value;
}

//this function sets the type of the object to the given type
void token::set_type(type t) {
	index_of_type = t;
}

//this function pushes the given object to the vector of the arguments 
//of the object 
void token::push_arg(token obj) {
	args.push_back(obj);
}

//this function returns the type of the object
type token::get_type()
{
	return index_of_type;
}
//this function sets the value of the i-th argument
void token::set_arg_value(std::string str, int i)
{
	args[i].value = str;
}




//this function receives the positions of opening and closing parentheses and 
//checks if there is a nested object in it
bool is_nested(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end) {
	start++;
        while(start != end) {
		if(*start == "(") {
			return true;
		}
		start++;
	}
	return false;
}

bool has_obj_after_list(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end) {
	start++;
	while(start != end) {
		if(*start == ")" && *(start + 1) != ")") {
			return true;
		}
		start++;
	}
	return false;
}

//this function makes an object, getting the positions of the
// opening and closing parentheses(the object can have nested list_objects)
token make(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end)
{
	start++;
	if(!is_nested(start, end)) {
		token obj(start, end);
		return obj;
	}
	token current_obj;
	current_obj.set_type(list);
	while(*start != "(") {
		token a(*start);
		current_obj.push_arg(a);
		start++;	
	}
	std::vector<std::string>::iterator ind = find_closing(start, end);
	current_obj.push_arg(make(start, ind));
	return current_obj;
}

//this function makes an object, getting the positions of the
//opening and closing parentheses(the object can have multiple list_objects)
//for example (begin (...) (...) ...)
token make_obj(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end)
{
	if(!is_nested(start, end)) {
		token a(start, end);
		return a;
	}
	token obj;
	start++;
	while(start != end) {
		while(*start != "(" && *start != ")") {
			token a(*start);
	                obj.push_arg(a);
        	        start++;
		}
		if(start != end) {
			std::vector<std::string>::iterator beg = start;
			std::vector<std::string>::iterator ind = find_closing(beg, end);
			token temp;
			if(has_obj_after_list(start, ind)) {
				temp = make_obj(start, ind);
			}
			else {
				temp = make(start, ind);
			}
			obj.push_arg(temp);
			start = ind;
			start++;
		}
	}
	return obj;
}

//this function receives a vector of strings makes the tree of token objects
//and returns a vector of objects
std::vector<token> make_objs(std::vector<std::string> v) 
{
	std::vector<token> objs;
	std::vector<std::string>::iterator start = v.begin();
	while(start != v.end()) {
        	std::vector<std::string>::iterator ind = find_closing(start, v.end());
		objs.push_back(make_obj(start, ind));
		if(ind == v.end() - 1) {
			break;
		}
		start = ind + 1;
		if(start != v.end() && *start != "(") {
			std::cerr << "Syntax error" << std::endl;
			exit(1);
		}
	}
	return objs;
}

//this function checks if the list object is an arithmetic expression
bool token::is_arithmethic()
{
        std::string val = args[0].get_value();
        if(val == "+" || val == "-" || val == "/" || val == "*" || val == "remainder") {
                return true;
        }
        return false;
}

//this function returns true if there aren't any nested lists in the given object
bool token::not_nested()
{
        for(int i = 0; i < args.size(); i++) {
                if(args[i].get_type() == list) {
                        return false;
                }
        }
        return true;
}

//this function checks if the list object is a conditional expression
bool token::is_conditional()
{
	std::string val = args[0].get_value();
        if(val == ">" || val == "<" || val == "<=" || val == ">=" || val == "equal?" || val == "=") {
                return true;
        }
        return false;
}
