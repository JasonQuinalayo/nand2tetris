#pragma once
#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <memory>
struct Statement;

class Encoder 
{
    std::ofstream* output_file_stream;
public:
    Encoder(std::ofstream* output_file_stream);
    void encode(const std::vector<std::shared_ptr<Statement>>& statements);
private:
    void map_symbols(const std::vector<std::shared_ptr<Statement>>& statements, std::map<std::string, int>& symbol_table) const;
    void map_predefined_symbols(std::map<std::string, int>& symbol_table) const;
    void map_goto_labels(const std::vector<std::shared_ptr<Statement>>& statements, std::map<std::string, int>& symbol_table) const;
};
