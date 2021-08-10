#include "parser.hpp"
#include "code_writer.hpp"
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
struct Statement;
void process_file(const std::string& file_name, Parser& parser, CodeWriter& code_writer);
std::string extract_file_name(const std::string& full_path);

int main(int argc, char** argv)
{
    const std::string input_file_name = std::string(argv[1]);
    std::string output_file_name;
    std::ofstream output_file_stream;
    Parser parser;
    output_file_name = input_file_name.substr(0, input_file_name.size() - 3) + ".asm";
    output_file_stream.open(output_file_name);
    CodeWriter code_writer(&output_file_stream);
    process_file(input_file_name, parser, code_writer);
    code_writer.write_inf_loop();
    output_file_stream.close();
    return 0;
}

void process_file(const std::string& file_name, Parser& parser, CodeWriter& code_writer)
{
    std::ifstream input_stream(file_name);
    if (!input_stream.is_open())
    {
        std::cerr << "Cannot open file" << std::endl;
        exit(1);
    }
    const std::vector<std::shared_ptr<Statement>> parsed_contents = parser.parse(input_stream);
    input_stream.close();
    const std::string file_name_without_path = extract_file_name(file_name);
    const std::string file_name_only = file_name_without_path.substr(0, file_name_without_path.size() - 3);
    code_writer.write_code(parsed_contents, file_name_only);
}

std::string extract_file_name(const std::string& file_name)
{
    const std::size_t last_sep = file_name.find_last_of("/\\");
    return file_name.substr(last_sep + 1);
}