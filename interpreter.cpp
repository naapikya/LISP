#include "functions.h"


//INTERPRETER

bool in_function = false;
int function_index;
std::vector<func> funcs; //functions
std::vector<std::pair<std::string, std::string>> vars; //variables

//this function reseives a double number <a> and an integer <n>,
//and sets the precision of <a> as <n> and returns the result as a string
std::string set_precision(double a, int n)
{
        if(a == (-0)) {
                return "0";
	}
        std::string str = std::to_string(a); //12.34 => 12.340000
        int i = 0;
        while(str[i] != '.') {
                i++;
        }
        while((str.length()-1 > i) && str[str.length()-1] == '0') {
                str.pop_back();
        }
        if(str[str.length()-1] == '.') {
                str.pop_back();
                return str;
        }
        int after_point = str.length()-1-i;
        if(after_point > n) {
                for(int j = 0; j < after_point-n; j++) {
                        str.pop_back();
                }
        }
        return str;
}


//this function checks whether the string is a real number or not
bool is_real(std::string str)
{
        int countDots = 0;
        if(str[0] == '.' || str[str.length() - 1] == '.') {
                return false;
        }
        for(int i = 0; i < str.length(); i++) {
                if(str[i] == '-' && i == 0) {
                        continue;
                }
                if(str[i] < '0' || str[i] > '9') {
                        if(str[i] == '.') {
                                countDots++;
                        }
                        else{
                                return false;
                        }
                }
        }
        return countDots <= 1;
}

//this function checks if a given string is a number or not
bool is_num(std::string str)
{
        if((str[0] != '-') && (str[0] < '0' || str[0] > '9')) {
                return false;
        }
        for(int i = 1; i < str.length(); i++) {
                if(str[i] < '0' || str[i] > '9') {
                        return false;
                }
        }
        return true;
}

//this function finds the variable by the given identifier
std::string find_variable_value(std::string id)
{
        if(::in_function) {
                return funcs[::function_index].find_val(id);
        } 
        return find_value(id);
}

//this function changes the value of the given variable
void change_variable_value(std::string var, std::string value)
{
        if(::in_function) {
                funcs[::function_index].change_val(var, value);
        }
        change_value(var, value);
}

std::string calculate_remainder(token &obj)
{
	if(obj.get_args().size() != 3) {
        	std::cerr << "Wrong number of arguments! The Remainder function needs 2 arguments!" << std::endl;
                exit(1);
        }
        std::string str1 = obj.get_args()[1].get_value();
        std::string str2 = obj.get_args()[2].get_value();
        if (obj.get_args()[1].get_type() == identifier && !is_keyword(obj.get_args()[1].get_value())) {
        	str1 = find_variable_value(obj.get_args()[1].get_value());
        }
        if (obj.get_args()[2].get_type() == identifier && !is_keyword(obj.get_args()[2].get_value())) {
        	str2 = find_variable_value(obj.get_args()[2].get_value());
        }
        if(!is_num(str1) || !is_num(str2)) {
        	std::cerr << "Wrong type of arguments for the <remainder> function! Integer required!" << std::endl;
                exit(1);
        }
        int num1 = std::stoi(str1);
        int num2 = std::stoi(str2);
        int result = num1 % num2;
        obj.set_value(std::to_string(result));
        return std::to_string(result);
}

//this function calculates the given arithmethic expression
std::string calculate(token &obj)
{	
	if(obj.get_args()[0].get_value() == "remainder") {
		return calculate_remainder(obj);
        }
	std::vector<double> operands;
	for(int i = 1; i < obj.get_args().size(); i++) {
		if (obj.get_args()[i].get_type() == identifier && !is_keyword(obj.get_args()[i].get_value())) {
			operands.push_back(std::stod(find_variable_value(obj.get_args()[i].get_value())));

		} else if(obj.get_args()[i].get_type() == literal){
			operands.push_back(std::stod(obj.get_args()[i].get_value()));
		} else {
			interprete(obj.get_args()[i]);
			operands.push_back(std::stod(obj.get_args()[i].get_value()));
		}
	}
	char val = obj.get_args()[0].get_value()[0];
	double res;
	switch(val) {
		case '+':
			res = 0;
			for(int i = 0; i < operands.size(); i++) {
				res += operands[i];
			}
			break;
		case '-':
                        res = operands[0];
                        for(int i = 1; i < operands.size(); i++) {
                                res -= operands[i];
                        }
                        break;
		case '/':
                        res = operands[0];
                        for(int i = 1; i < operands.size(); i++) {
                                res /= operands[i];
                        }
                        break;
		case '*':
                        res = operands[0];
                        for(int i = 1; i < operands.size(); i++) {
                                res *= operands[i];
                        }
                        break;
	}
	obj.set_value(set_precision(res, 10));
	return set_precision(res, 10); 

}	

//this function solves arithmethic expression(can be nested) and returns a string
std::string solve(token &obj)
{
	if(obj.get_type() == list && obj.is_conditional()) {
		obj.set_value(determine_conditional(obj));
		return determine_conditional(obj);
	}
	if(obj.get_type() == identifier) {
		return find_value(obj.get_value());
	}
	if(obj.get_type() == literal) {
		if(obj.get_value()[0] == '\"') {
			std::string res = obj.get_value();
			res.erase(0, 1);
			res.erase(res.length() - 1, 1);
			return res;
		}
		return obj.get_value();
	}
	if(obj.get_type() == list && obj.is_arithmethic()) {
		if(obj.not_nested()) {
			obj.set_value(calculate(obj));
			return calculate(obj);
		} else {
	                for(int i = 1; i < obj.get_args().size(); i++) {
                        	if(obj.get_args()[i].get_type() == list) {
                                	obj.set_arg_value(solve(obj.get_args()[i]), i);
                        	}
                	}
		}
        }
	obj.set_value(calculate(obj));
	return calculate(obj);
}

//this function returns "#t" if the two arguments of the given ojbect are equal
//(arguments can be both string and numeric)
//and returns "#f" otherwise
std::string determine_equal(token &obj)
{
	std::vector<std::string> str_operands;
        for(int i = 1; i < obj.get_args().size(); i++) {
        	if (obj.get_args()[i].get_type() == identifier && !is_keyword(obj.get_args()[i].get_value())) {
                	str_operands.push_back(find_variable_value(obj.get_args()[i].get_value()));
                } else if(obj.get_args()[i].get_type() == literal){
                	str_operands.push_back(obj.get_args()[i].get_value());
                } else {
                	interprete(obj.get_args()[i]);
                        str_operands.push_back(obj.get_args()[i].get_value());
        	}
         }
         for(int i = 1; i < str_operands.size(); i++) {
         	if(str_operands[i] != str_operands[i-1]) {
                	obj.set_value("#f");
                        return "#f";
                }
         }
         obj.set_value("#t");
         return "#t";
}

//this function returns "#t" if the given conditional expression is true
//and returns "#f" otherwise
std::string determine_conditional(token &obj) 
{
	if(obj.get_args()[0].get_value() == "equal?" || obj.get_args()[0].get_value() == "=") {
		return determine_equal(obj);
	} else {  
        	std::vector<double> operands;
        	for(int i = 1; i < obj.get_args().size(); i++) {
			std::string str;
                	if (obj.get_args()[i].get_type() == identifier && !is_keyword(obj.get_args()[i].get_value())) {
				str = find_variable_value(obj.get_args()[i].get_value());
			}else if (obj.get_args()[i].get_type() == literal){
                		str = obj.get_args()[i].get_value();
                	} else {
                		interprete(obj.get_args()[i]);
                        	str = obj.get_args()[i].get_value();
                	}
			operands.push_back(std::stod(str));
        	}
		if(obj.get_args()[0].get_value() == ">") {
               		for(int i = 1; i < operands.size(); i++) {
				if(operands[i] >= operands[i-1]) {
					obj.set_value("#f");
					return "#f";
				}
				obj.set_value("#t");
				return "#t";
			}
        	}
        	if(obj.get_args()[0].get_value() == "<") {
                	for(int i = 1; i < operands.size(); i++) {
                        	if(operands[i] <= operands[i-1]) {
					obj.set_value("#f");
                                	return "#f";
                        	}
				obj.set_value("#t");
                        	return "#t";
                	}
        	}
        	if(obj.get_args()[0].get_value() == ">=") {
                	for(int i = 1; i < operands.size(); i++) {
                        	if(operands[i] > operands[i-1]) {
					obj.set_value("#f");
                                	return "#f";
                        	}
				obj.set_value("#t");
                        	return "#t";
                	}
        	}
        	if(obj.get_args()[0].get_value() == "<=") {
                	for(int i = 1; i < operands.size(); i++) {
                        	if(operands[i] < operands[i-1]) {
					obj.set_value("#f");
                                	return "#f";
                        	}
				obj.set_value("#t");
                        	return "#t";
                	}
        	}
		obj.set_value("#t");
		return "#t";
	}
}

//this function returns true if the given string is a keyword
bool is_keyword(std::string str)
{
	std::vector<std::string> keywords {"begin", "set!",
	       				   "define", "display", "newline", "+", "-", 
					   "*", "/", "remainder", ">", "<", 
					   ">=", "<=", "euqal?", "=", "if"};
	for(int i = 0; i < keywords.size(); i++) {
		if(keywords[i] == str) {
			return true;
		}
	}
	return false;
}

//this function finds the value of the given variable_name from the <vars> vector
std::string find_value(std::string str)
{ 
	for(int i = 0; i < vars.size(); i++)//a 7 c 8 d 9
	{
                if (str == vars[i].first) {
                        return vars[i].second;
                }

	}
	std::cerr << "Unknown variable " << str << std::endl;
	return "";
}

//this function changes the value of the given variable 
void change_value(std::string var, std::string value)
{
	bool found = false;
        for(int i = 0; i < vars.size(); i++)//a 7 c 8 d 9
        {
                if (var == vars[i].first) {
			found = true;
			vars[i].second = value;
                }
        }
	if(!found) {
        	std::cerr << "Unknown variable " << var << std::endl;
		exit(1);
        }
}

//this function performs the function calling
void function_caller(token &obj)
{
	std::vector<token> args = obj.get_args();
	std::vector<std::string> args_values;
	for(int i = 1; i < args.size(); i++) {
		if(args[i].get_type() == identifier) {
			args_values.push_back(find_variable_value(args[i].get_value()));
		} else if (args[i].get_type() == literal) {
			args_values.push_back(args[i].get_value());
		} else {
			interprete(args[i]);
			args_values.push_back(args[i].get_value());
		}
	}
	for(int i = 0; i < funcs.size(); i++) {
		if(args[0].get_value() == funcs[i].get_name()) {
			funcs[i].set_pars(args_values);  
			::in_function = true;
			::function_index = i;
			funcs[i].interprete_func();
			obj.set_value(funcs[i].get_return_value());
			::in_function = false;
			break;
		}
	}
}

//this function receives an object which is a function definition
//and creates a func object with the corresponding parameters
void function_implement(token obj)
{
	std::vector<token> args = obj.get_args();
	std::vector<token> parameters = args[1].get_args();
	std::string fname = parameters[0].get_value();
	std::vector<std::string> par_ids;
	for(int i = 1; i < parameters.size(); i++) {
		par_ids.push_back(parameters[i].get_value());
	}
	std::vector<token> objs;
	for(int i = 2; i < args.size(); i++) {
		objs.push_back(args[i]);
	}
	func f(fname, par_ids, objs);
	funcs.push_back(f);
}

//this function interpretes the define function
std::string define_interpreter(token obj) 
{
	std::vector<token> args = obj.get_args();
	if(args[1].get_type() == list) {
		function_implement(obj);
	} else {
		if(args[1].get_type() != identifier || is_keyword(args[1].get_value())) {
			std::cerr << "Invalid identifier" << std::endl;
			exit(1);
		}
		if (is_keyword(args[2].get_value())) {
			std::cerr << "Invalid arguments for define!" << std::endl;	
			exit(1);
		}
		std::pair<std::string, std::string> var;
		var.first = args[1].get_value();
		if (args[2].get_type() == identifier) {
			var.second = find_variable_value(args[2].get_value());
		} else if(args[2].get_type() == list){
			interprete(args[2]);
			var.second = args[2].get_value();
		} else {
			var.second = args[2].get_value();
		}
		if(::in_function) {
			funcs[::function_index].push_variable(var);
		} else {
			vars.push_back(var);
		}
	}
	return "";
}

//this function interpretes the set! function
void set_interpreter(token obj)
{
        std::vector<token> args = obj.get_args();
        if(args[1].get_type() != identifier) {
                std::cerr << "Invalid identifier" << std::endl;
		exit(1);
        }
        if(is_keyword(args[2].get_value())) {
                std::cerr << "Invalid arguments for set! !" << std::endl;
		exit(1);
        }
        if (args[2].get_type() == identifier) {
		std::string value = find_variable_value(args[2].get_value());
		change_variable_value(args[1].get_value(), value);
        } else if(args[2].get_type() == list){
		interprete(args[2]);
                change_variable_value(args[1].get_value(), args[2].get_value()); 
        } else if(args[2].get_type() == literal){
		change_variable_value(args[1].get_value(), args[2].get_value());
	}
}

void execute_obj(token &obj, token arg)
{
        if(arg.get_type() == list) {
        	interprete(arg);
		obj.set_value(arg.get_value());
	} else if(arg.get_type() == identifier) {
		if(::in_function) {
			obj.set_value(funcs[::function_index].find_val(arg.get_value()));
		} else {
        		obj.set_value(find_value(arg.get_value()));
		}
        } else {
        	obj.set_value(arg.get_value());
        }
}

//this function interprets the if conditional operation
void if_interpreter(token &obj)
{
	std::vector<token> args = obj.get_args();
	if(args.size() > 4 || args.size() < 3) {
		std::cerr << "Syntax error for <if> !" << std::endl;
		exit(1);
	} else if(!args[1].is_conditional()) {
		std::cerr << "Invalid condition for <if> !" << std::endl;
                exit(1);
	} else if(determine_conditional(args[1]) == "#t") {
		execute_obj(obj, args[2]);
	} else if(determine_conditional(args[1]) == "#f" && args.size() == 4) {
		execute_obj(obj, args[3]);
	}

}

//this function interprets the display function
void display_interpreter(token obj)
{
	 std::vector<token> args = obj.get_args();

	 if(args.size() != 2) {
		std::cerr << "Syntax error for <display> !" << std::endl;
                exit(1);
	 }

	 if(args[1].get_type() == identifier) {

		 if(!is_keyword(args[1].get_value())) {
			 std::cout << find_variable_value(args[1].get_value());
		 } else {
			 std::cerr << "Invalid argument for <display> ! The argument should NOT be a keyword !" << std::endl;
                	 exit(1);
		 }
	 } else if(args[1].get_type() == list) {
		 interprete(args[1]);
		 std::cout << args[1].get_value();
	 } else if(args[1].get_type() == literal) {
		 std::cout << solve(args[1]);
	 }
}

//this function interprets the newline function
void newline_interpreter(token obj)
{
	std::vector<token> args = obj.get_args();
	if(args.size() > 1) {
		std::cerr << "Syntax error for <newline> !" << std::endl;
		exit(1);
	}
	std::cout << std::endl;
}

//this function checks if the given string is a function name or not
bool is_fname(std::string val)
{
	for(int i = 0; i < funcs.size(); i++) {
		if(val == funcs[i].get_name()) {
			return true;
		}
	}
	return false;
}


//here is the function which calls the special interpreters
void interprete(token &obj)
{
        std::vector<token> args = obj.get_args();
        if(!is_keyword(args[0].get_value()) && !is_fname(args[0].get_value())) {
                std::cerr << "Unknown keyword or function_name!" << std::endl;
		exit(1);
        } else if(is_fname(args[0].get_value())) {
		function_caller(obj);
	} else if(args[0].get_value() == "display") {
                display_interpreter(obj);
        } else if(args[0].get_value() == "define") {
		define_interpreter(obj);
	} else if(obj.is_arithmethic() || obj.is_conditional()) {
        	solve(obj);
        } else if(args[0].get_value() == "set!") {
		set_interpreter(obj);
	} else if(args[0].get_value() == "begin") {
		begin_interpreter(obj);
	} else if(args[0].get_value() == "if") {
		if_interpreter(obj);
	} else if(args[0].get_value() == "newline") {
		newline_interpreter(obj);
	}
}

//this function interprets the begin operatien
void begin_interpreter(token obj)
{
	std::vector<token> args = obj.get_args();
	for(int i = 1; i < args.size(); i++) {
		if(args[i].get_args()[0].get_value() == "begin") {
			begin_interpreter(args[i]);
		} else {
			interprete(args[i]);
		}
	}
}

//this function receives the file and returns the vector of the objects
std::vector<token> parser(std::ifstream &i_file)
{
	std::string line = make_str(i_file);
        if(!balance(line)) {
                std::cerr << "Incorrect parentheses" << std::endl;
		exit(1);
        }
        add_space(line);
        std::vector<std::string> v = split(line);
	std::vector<token> objs = make_objs(v);
	return objs;
}

//this function interpretes all the objects of the file
void interpreter(std::vector<token> objs) 
{
        for(int i = 0; i < objs.size(); i++) {
                interprete(objs[i]);
        }
}
