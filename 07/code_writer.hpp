#pragma once
#include "code_generators.hpp"
#include <fstream>
#include <vector>
#include <memory>
#include <map>
#include <string>

class CodeWriter
{
    std::ofstream* output_file_stream;
public:
    CodeWriter(std::ofstream* output_file_stream);
    void write_code(const std::vector<std::shared_ptr<Statement>>& statements, const std::string& file_name);
    void write_inf_loop();
private:
    void write_lines(const std::vector<std::string>& assembly_statements);
};