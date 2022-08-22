//here are the implementations of the func class_methods
#include "functions.h"

//this is our func constructor
func::func(std::string Name, std::vector<std::string> par_ids, std::vector<token> objs)
{
        name = Name;
        implement_objs = objs;
        for(int i = 0; i < par_ids.size(); i++) {
                std::pair<std::string, std::string> temp;
                temp.first = par_ids[i];
                variables.push_back(temp);
        }
        return_value = "";
        number_of_parameters = par_ids.size();
}

//this function pushs the given variable into the variables vector
void func::push_variable(std::pair<std::string, std::string> par)
{
        variables.push_back(par);
}

//this function finds the value of the given variable_name from the vector <variables> 
std::string func::find_val(std::string str)
{
        for(int i = 0; i < variables.size(); i++)//a 7 c 8 d 9
        {
                if (str == variables[i].first) {
                        return variables[i].second;
                }
        }
        std::cerr << "Unknown variable " << str << std::endl;
        exit(1);
        return "";
}

//this function changes the value of the given variable_name in the vector <variables>
void func::change_val(std::string var, std::string value)
{
        bool found = false;
        for(int i = 0; i < variables.size(); i++)//a 7 c 8 d 9
        {
                if (var == variables[i].first) {
                        found = true;
                        variables[i].second = value;
                }
        }
        if(!found) {
                std::cerr << "Unknown variable " << var << std::endl;
                exit(1);
        }
}

//this function interpretes the body of the function
//and sets the return_value
void func::interprete_func()
{
        for(int i = 0; i < implement_objs.size() - 1; i++) {
                if(implement_objs[i].get_type() == list) {
                        interprete(implement_objs[i]);
                }
        }
        if(implement_objs[implement_objs.size() - 1].get_type() == literal) {
                return_value = implement_objs[implement_objs.size() - 1].get_value();
        } else if (implement_objs[implement_objs.size() - 1].get_type() == identifier) {
                return_value = find_val(implement_objs[implement_objs.size() - 1].get_value());
        } else {
                interprete(implement_objs[implement_objs.size() - 1]);
                return_value = implement_objs[implement_objs.size() - 1].get_value();
        }
}

//this function returns the name of the fuunction
std::string func::get_name()
{
        return name;
}

//this function returns the return_value of the function
std::string func::get_return_value() 
{
        return return_value;
}

//this function returns the vector of the variables in the function
std::vector<std::pair<std::string, std::string>> func::get_vars()
{
        return variables;
}

//this function sets the variables as the given parameters
void func::set_pars(std::vector<std::string> pars)
{
        if(number_of_parameters != pars.size()) {
                std::cerr << "Invalid number of arguments for function " << name << std::endl;
                exit(1);
        }
        for(int i = 0; i < variables.size(); i++) {
                //in the vector variables there are only passed variables yet
                variables[i].second = pars[i];
        }
}

//this function prints all the variable_names and their values in the function
void func::print_variables()
{
        for(int i = 0; i < variables.size(); i++) {
                std::cout << "id = " << variables[i].first << std::endl;
                std::cout << "value = " << variables[i].second << std::endl;
        }
}


