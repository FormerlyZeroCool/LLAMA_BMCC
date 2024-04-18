#include <iostream>
#include "httplib.h"
#include "CrossPlatform.hpp"
#include <string>
#include <fstream>
#include <cstdlib>
#include <map>
#include <variant>
#include <cctype>


//helper function prototypes
std::string readFile(std::string filePath);
size_t now();
bool is_whitespace(char c)
{
        return c == ' ' || c == '\t';
}
void skip_whitespace(const std::string& text, size_t& index)
{
    while(text.size() > index && is_whitespace(text[index])) index++;
}
void skip_non_whitespace(const std::string& text, size_t& index)
{
    while(text.size() > index && text[index] != ' ' && text[index] != '\t' && text[index] != '\n') index++;
}
void trim_end(std::string& text)
{
        size_t last = 0, cur = 0;
        while(cur < text.size())
        {
                skip_non_whitespace(text, cur);
                last = cur;
                do {
                        cur++;
                        skip_whitespace(text, cur);
                } while(text[cur] == '\n');
        }
        text = text.substr(0, last);
}
void escape_json(std::string& text)
{
        std::string output;
        for(char c : text)
        {
                switch(c) {
                        case('\"'):
                                output += "\\\"";
                                break;
                        case('\\'):
                                output += "\\\\";
                                break;
                        case('\b'):
                                output += "\\b";
                                break;
                        case('\f'):
                                output += "\\f";
                                break;
                        case('\n'):
                                output += "\\n";
                                break;
                        case('\r'):
                                output += "\\r";
                                break;
                        case('\t'):
                                output += "\\t";
                                break;
                        default:
                                output += c;
                }
        }
        text = output;
}
std::string base_model(const std::string& text)
{
        size_t start_name = text.find("FROM");
        if(start_name == std::string::npos)
                return std::string();
        start_name += strlen("FROM");
        skip_whitespace(text, start_name);
        size_t end_name = start_name;
        skip_non_whitespace(text, end_name);
        return text.substr(start_name, end_name - start_name);
}

bool is_int(std::string& text)
{
        size_t i = text[0] == '-';
        bool is_int = true;
        while(i < text.size() && is_int) 
                is_int = isdigit(text[i++]);
        return is_int;
}
bool is_float(std::string& text)
{
        size_t i = text[0] == '-';
        bool is_float = true, is_beg = true;
        while(i < text.size() && is_float)
        {
                if(is_beg && text[i] == '.')
                {
                        is_beg = false;
                }
                else if(!isdigit(text[i]))
                        is_float = false;
                i++;
        }
        return is_float;
}

struct ParamType {
        enum param_type {null, integer, real, text} type;
        std::string description;
        ParamType(): type(param_type::null){}
        ParamType(std::string type_str, std::string desc): type(param_type::null), description(desc) 
        {
                if(type_str == "int")
                        type = param_type::integer;
                else if(type_str == "float")
                        type = param_type::real;
                else if(type_str == "string")
                        type = param_type::text;
        }
        bool is_valid(std::string value)
        {
                switch(type) {
                        case(param_type::integer):
                               return is_int(value);
                        case(param_type::real):
                                return is_float(value);
                        case(param_type::text):
                                return true;
                        default: return false;
                }
        }
        std::string to_string()
        { 
                switch(type) {
                        case(param_type::integer):
                                return "Integer";
                        case(param_type::real):
                                return "Float";
                        case(param_type::text):
                                return "String";
                        default:
                                return "Null";
                }
        }
        friend std::ostream& operator<<(std::ostream& o, ParamType& pt)
        {
                o << pt.to_string();
                return o;
        }
};
std::string substr(std::string& text, size_t start, size_t end)
{
        return text.substr(start, end - start);
}
std::map<std::string, ParamType> parse_param_types(std::string config)
{
        std::map<std::string, ParamType> types;
        size_t i = 0;
        while(i < config.size())
        {
                const size_t name_start = i;
                skip_non_whitespace(config, i);
                const size_t name_end = i;
                skip_whitespace(config, i);
                
                const size_t type_start = i;
                skip_non_whitespace(config, i);
                const size_t type_end = i;

                skip_whitespace(config, i);
                
                if(i >= config.size() || config[i] == '\n')
                {
                        i++; 
                        types[substr(config, name_start, name_end)] =
                                ParamType(substr(config, type_start, type_end), "");
                        continue;
                }
                
                const size_t desc_start = i;
                while(i < config.size() && config[i] != '\n') i++;
                const size_t desc_end = i++;
                std::string type = substr(config, type_start, type_end);
                std::string desc = substr(config, desc_start, desc_end);
                ParamType pt = ParamType(type, desc);
                types[substr(config, name_start, name_end)] = pt;
                        
        }
        return types;
}
std::map<std::string, std::string> parse_params(const std::string& text)
{
        size_t index = 0;
        std::map<std::string, std::string> params;
        while(index < text.size())
        {
                index = text.find("PARAMETER", index);
                if(index == std::string::npos)
                        break;
                index += strlen("PARAMETER");
                skip_whitespace(text, index);
                const size_t start_key = index;
                skip_non_whitespace(text, index);
                if(index >= text.size())
                        break;
                const size_t end_key = index;
                skip_whitespace(text, index);
                if(index >= text.size())
                        break;
                const size_t start_value = index;
                skip_non_whitespace(text, index);  
                if(index >= text.size())
                        break;
                params[text.substr(start_key, end_key - start_key)] = text.substr(start_value, index - start_value);
        }
        return params;
}
struct BaseModel {
        std::string name, params, size;
        std::string to_string()
        {
                std::string str;
                str += "Model: ";
                str += name;
                str += ", Parameters: ";
                str += params;
                str += ", Size: ";
                str += size;
                return str;
        }
};
std::map<std::string, BaseModel> parse_base_models(std::string config)
{
        size_t i = 0;
        std::map<std::string, BaseModel> map;
        while(i < config.size())
        {
                size_t name_s = i;
                skip_non_whitespace(config, i);
                size_t name_e = i;
                
                skip_whitespace(config, i);
                
                size_t param_s = i;
                skip_non_whitespace(config, i);
                size_t param_e = i;
                
                skip_whitespace(config, i);

                size_t size_s = i;
                skip_non_whitespace(config, i);
                size_t size_e = i;
                
                skip_whitespace(config, i);

                BaseModel model;
                model.name = substr(config, name_s, name_e);
                model.params = substr(config, param_s, param_e);
                model.size = substr(config, size_s, size_e);
                map[model.name] = model;
                i = config.find('\n', i);
                i++;
        }
        return map;
}
struct Model {
        std::string model_name = "test";
        std::string context;
        std::string filepath;
        std::string from_model;
        std::map<std::string, std::string> parameters;
        Model(std::string filepath): filepath(filepath){}
        void to_file() const 
        {
                std::fstream file(filepath + PATH_SEPARATOR + model_name + ".conf", std::ios::out | std::ios::trunc);
                file << to_string();
        }
        std::string to_string() const
        {
                std::string text;
                text += "FROM ";
                text += from_model;
                text += "\n";
                for(auto &[key, value] : parameters)
                {
                        text += "PARAMETER ";
                        text += key;
                        text += " ";
                        text += value;
                        text += "\n";
                }
                text += "\nSYSTEM \"\"\"";
                text += context;
                text += "\"\"\"\n";
                return text;
        }
        void load_ollama() const
        {
                std::string init_command = "ollama create ";
                init_command += model_name;
                init_command += " -f ";
                init_command += filepath;
                init_command += PATH_SEPARATOR;
                init_command += model_name;
                init_command += ".conf";
                std::cout<<init_command<<"\n";
                std::system(init_command.c_str());
        }
        void warm_up() const
        {
                run("");
        }
        std::string run(std::string prompt) const
        {
                std::string command = "ollama run test \"";
                command += prompt;
                command += "\" > model_output.txt";
                std::system(command.c_str());
                return readFile("model_output.txt");
        }
};
Model load_model_conf(std::string model_name, std::string filepath);

int main(int argc, char** argv)
{
  srand(now());

  std::cout << "starting server..." << std::endl;

  size_t file_index = 1;
  auto type_map = parse_param_types(readFile("./params.types"));
  auto base_models = parse_base_models(readFile("./models.list"));
  std::string model_name = argc > 2 ? argv[file_index++] : "test";
  Model model = load_model_conf("test", std::string(".."));

  model.load_ollama();
  model.warm_up();
  httplib::Server svr;
  svr.set_mount_point("/", "./web");
  svr.Get("/query", [model](const auto &req, auto &res) {
    
    std::string response;
    if (!req.has_param("prompt")) 
    {
        response += "{\"data\":\"\", \"error\":\"Exception, must supply parameter 'prompt'.\", \"success\":false }\n";
        res.set_content(response, "text/html");
        return;
    }

    std::string prompt = req.get_param_value("prompt");
    std::cout<<prompt<<"\n";
    std::string model_output = model.run(prompt);
    trim_end(model_output);
    escape_json(model_output);
    response += "{\"data\":\"" + model_output + "\", \"error\":\"\", \"sucess\":true}\n";
    res.set_content(response, "text/json");
    std::cout<<"responding to prompt:\n"<<prompt<<"\nwith output:\n"<<model_output<<"\n";
  });
  svr.Get("/set_context", [&model](const auto &req, auto &res) {
    if(!req.has_param("ctx"))
    {
        res.set_content("{ \"error\":\"must have param ctx for new model context.\", \"success\":false }\n", "text/json");
        return;
    }
    model.context = req.get_param_value("ctx");
    model.to_file();
    model.load_ollama();
    model.warm_up();
    res.set_content("{\"error\":\"\", \"success\":true}\n", "text/json");
  });
  svr.Get("/set_parameter", [&model, &type_map](const auto &req, auto &res) {
        if(!req.has_param("key") || !req.has_param("value"))
        {
                res.set_content("{\"error\":\"must include parameters key for parameter name, and value for value.\", \"success\":false }\n", "text/json");
                return;
        }
        if(type_map.count(req.get_param_value("key")) == 0) 
        {
                res.set_content("{\"error\":\"Error, invalid key: \'" + req.get_param_value("key") + "\'\", \"success\":false }\n", "text/json");
                return;
        }
        std::string key = req.get_param_value("key");
        std::string value = req.get_param_value("value");
        if(!type_map[key].is_valid(value))
        {
                res.set_content("{\"error\":\"Error, invalid value: \'" + value + "\' for parameter: \'" + key + "\' of type: " + type_map[key].to_string() + "\", \"success\":false }\n", "text/json");
                return;
        }
        std::cout << "updating param: " << key << " which is of type: " << type_map[key] << " with value: " << value << "\n";
        model.parameters[req.get_param_value("key")] = req.get_param_value("value");
        
        model.to_file();
        model.load_ollama();
        model.warm_up();

        res.set_content("{\"error\":\"\", \"success\":true}\n", "text/json");
  });
  svr.Get("/set_base_model", [&model, &base_models](const auto &req, auto &res) {
        if(!req.has_param("model"))
        {
                std::string resp = "{\"error\":\"";
                resp += "must include parameter model";
                resp += "\", \"success\":false}";
                res.set_content(resp, "text/json");
                return;
        }
        std::string model_name = req.get_param_value("model");
        if(base_models.count(model_name) == 0)
        {
                std::string resp = "{\"error\":\"";
                resp += "Error, invalid model name: \'";
                resp += model_name;
                resp += "\'";
                resp += "\", \"success\":false}";
                res.set_content(resp, "text/json");
                return;
        }
        
        model.from_model = std::move(model_name);
        model.to_file();
        model.load_ollama();
        model.warm_up();
        
        res.set_content(std::string("{\"info\":\"") + base_models[model.from_model].to_string() + "\", \"error\":\"\", \"success\":true}\n", "text/json");
  });
  std::cout << "Server running." << std::endl;
  svr.listen("0.0.0.0", 8080);
  
}

size_t now()
{
  return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
std::string readFile(std::string filePath)
{
  std::fstream file = std::fstream(filePath);
  std::string val((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return val;
}
Model load_model_conf(std::string model_name, std::string filepath)
{
        std::string text = readFile(filepath + PATH_SEPARATOR + model_name + ".conf");
        std::cout<<"loading conf file: "<<(filepath + PATH_SEPARATOR + model_name + ".conf")<<"\n";
        size_t end_of_initial = text.find("SYSTEM");
        if(end_of_initial == std::string::npos)
                end_of_initial = text.size();
        Model model(filepath);
        model.model_name = std::move(model_name);
        model.from_model = base_model(text);
        model.parameters = parse_params(text);
        do {
        if(end_of_initial < text.size())
        {
                size_t start_of_ctx = text.find("\"\"\"", end_of_initial);
                bool triple_quotes = true;
                if(start_of_ctx == std::string::npos)
                {
                        triple_quotes = false;
                        start_of_ctx = text.find("\"");
                        if(start_of_ctx != std::string::npos)
                                start_of_ctx++;
                }
                else
                        start_of_ctx += 3;
                if(start_of_ctx == std::string::npos)
                        break;

                size_t end_of_ctx = text.find(triple_quotes ? "\"\"\"" : "\"", start_of_ctx);
                if(end_of_ctx == std::string::npos)
                        break;
                model.context = text.substr(start_of_ctx, end_of_ctx - start_of_ctx);
        }
        } while(false);
        std::cout<<"loaded model:\n"<<model.to_string()<<"\n";
        return model;
}
