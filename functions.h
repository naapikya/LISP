#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include "token.h"
#include "func.h"



//functions of the tokenise.cpp file
std::string make_str (std::ifstream&);
bool balance(std::string);
std::string text(const std::string&, int&);
void add_space(std::string&);
std::vector<std::string> split(std::string&);
int find_closing(std::vector<std::string>, int);
bool is_nested(std::vector<std::string>);

//functions of the token.cpp file
type determine_type(std::string);
std::vector<std::string>::iterator find_closing(std::vector<std::string>::iterator, std::vector<std::string>::iterator); //kaskaceli
bool is_nested(std::vector<std::string>::iterator, std::vector<std::string>::iterator);
token make(std::vector<std::string>::iterator, std::vector<std::string>::iterator);
token make_obj(std::vector<std::string>::iterator, std::vector<std::string>::iterator);
std::vector<token> make_objs(std::vector<std::string>);

//functions of interprete.cpp file
std::string set_precision(double, int);
bool is_real(std::string);
bool is_num(std::string);
std::string find_variable_value(std::string);
void change_variable_value(std::string, std::string); 
std::string calculate_remainder(token&);
std::string calculate(token&);
std::string determine_conditional(token&);
std::string solve(token&);
std::string determine_equal(token&);
bool is_keyword(std::string);
std::string find_value(std::string);
void change_value(std::string, std::string);
void function_caller(token&);
void function_implement(token);
std::string define_interpreter(token);
void set_interpreter(token);
void execute_obj(token&, token);
void if_interpreter(token&);
void display_interpreter(token);
void newline_interpreter(token);
bool is_fname(std::string);
void interprete(token&);
std::vector<token> parser(std::ifstream&);
void interpreter(std::vector<token>);
void begin_interpreter(token);







