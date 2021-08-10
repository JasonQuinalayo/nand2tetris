#include "parser.hpp"
#include "encoder.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
struct Statement;

int main(int argc, char** argv)
{
    std::ifstream input_file_stream(argv[1]);
    if (!input_file_stream.is_open()) 
    {
        std::cout << "Cannot open file. Exiting.\n";
        return 1;
    }
    Parser parser;
    const std::vector<std::shared_ptr<Statement>> parsed_contents = parser.parse(input_file_stream);
    input_file_stream.close();
    const std::string file_name_with_extension = std::string(argv[1]);
    const std::string file_name_without_extension = file_name_with_extension.substr(0, file_name_with_extension.size() - 4);
    std::ofstream output_file_stream(file_name_without_extension + ".hack");
    Encoder encoder(&output_file_stream);
    encoder.encode(parsed_contents);
    output_file_stream.close();
    return 0;
}
