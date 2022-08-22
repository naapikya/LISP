#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <vector>
#include <string>

enum type {
        identifier, //0
        literal,   //1
        list      //2
};


class token
{
	private:
		std::string value;
		std::vector<token> args;
		type index_of_type;
	public:
		token();
		token(std::string);
		token(std::vector<std::string>::iterator, std::vector<std::string>::iterator);
		void print()
		{
			std::cout << "value = " << value << std::endl;
			if(index_of_type == list) {
				std::cout << "next value = " << args[0].value << std::endl;
			}
			std::cout << "number of arguments = " << args.size() << std::endl;
			std::cout << "type = " << index_of_type << std::endl;
		}
		std::vector<token>& get_args();
		void push_arg(token);
		void set_type(type);
		void set_value(std::string);
		std::string get_value();
		type get_type();
		void set_arg_value(std::string, int);
		bool is_arithmethic();
		bool not_nested();
		bool is_conditional();
};

#endif
