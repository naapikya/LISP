//here is the func class declaration
#include <iostream>
#include "token.h"

class func {
        private:
                std::string name;
                std::vector<std::pair<std::string, std::string>> variables;
                int number_of_parameters;
                std::vector<token> implement_objs;
                std::string return_value;
	public:
		func(std::string, std::vector<std::string>, std::vector<token>);
		void push_variable(std::pair<std::string, std::string>);
		std::string find_val(std::string);
		void change_val(std::string, std::string);
		void interprete_func();
		std::string get_name();
		std::string get_return_value();
		std::vector<std::pair<std::string, std::string>> get_vars();
		void set_pars(std::vector<std::string>);
		void print_variables();
};
