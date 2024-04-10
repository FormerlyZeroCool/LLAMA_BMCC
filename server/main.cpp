#include <iostream>
#include "httplib.h"
#include "CrossPlatform.hpp"
#include <string>
#include <fstream>
#include <cstdlib>


//helper function prototypes
std::string readFile(std::string filePath);
size_t now();
struct Model {
        std::string model_name = "test";
        std::string initial_text;
        std::string context;
        std::string filepath;
        Model(std::string initial_text, std::string filepath): initial_text(initial_text), filepath(filepath){}
        void to_file() const 
        {
                std::fstream file(filepath + PATH_SEPARATOR + model_name + ".conf");
                file << initial_text << "\"\"\"" << context << "\"\"\"";
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
  std::string model_name = argc > 2 ? argv[file_index++] : "test";
  Model model = load_model_conf("test", std::string(".."));

  model.load_ollama();
  model.warm_up();
  httplib::Server svr;
  svr.set_mount_point("/", "./web");
  svr.Get("/query", [model](const auto &req, auto &res) {
    
    std::string response;
    if (req.has_param("prompt")) {
      std::string prompt = req.get_param_value("prompt");
      std::string model_output = model.run(prompt);
      response += "{\"data\":\"" + model_output + "\", \"error\":\"\"}";
      res.set_content(response, "text/json");
      std::cout<<"responding to prompt:\n"<<prompt<<"\nwith output:\n"<<model_output<<"\n";
    }
    else
    {
        response += "{\"data\":\"\", \"error\":\"Exception, must supply parameter 'prompt'.\"}";
        res.set_content(response, "text/html");
    }
  });
  svr.Get("/set_context", [&model](const auto &req, auto &res) {
    //read in template file, parse out two parts
    //part1 initial part before context
    //part2 end of file after context
    if(!req.has_param("ctx"))
    {
        res.set_content("{\"error\":\"true\", \"success\":\"false\"}", "text/json");
        return;
    }
    model.context = req.get_param_value("ctx");
    model.to_file();
    model.load_ollama();
    model.warm_up();
    res.set_content("{\"error\":\"false\", \"success\":\"true\"}", "text/json");
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
        std::string text = readFile(filepath + PATH_SEPARATOR + model_name);
        size_t end_of_initial = text.find("SYSTEM");
        if(end_of_initial == std::string::npos)
                end_of_initial = text.size();
        Model model(text.substr(0, end_of_initial), filepath);
        model.model_name = std::move(model_name);
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
        return model;
}
