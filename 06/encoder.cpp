#include "encoder.hpp"
#include "code_generators.hpp"
#include <bitset>
#include <iostream>

Encoder::Encoder(std::ofstream* output_file_stream) : output_file_stream(output_file_stream) {}

void Encoder::encode(const std::vector<std::shared_ptr<Statement>>& statements) 
{
    std::map<std::string, int> symbol_table;
    map_symbols(statements, symbol_table);
    AInstruction::set_symbol_table(symbol_table);
    for (auto& statement: statements)
    {
        if (!statement->is_label())
            (*output_file_stream) << statement->generate_code() << std::endl;
    }
}

void Encoder::map_symbols(const std::vector<std::shared_ptr<Statement>>& statements, std::map<std::string, int>& symbol_table) const
{
    map_predefined_symbols(symbol_table);
    map_goto_labels(statements, symbol_table);
}

void Encoder::map_predefined_symbols(std::map<std::string, int>& symbol_table) const
{
    for (int i = 0; i < 16; i++) 
    {
        symbol_table["R" + std::to_string(i)] = i;
    }
    symbol_table["SCREEN"] = 16384;
    symbol_table["KBD"] = 24576;
    symbol_table["SP"] = 0;
    symbol_table["LCL"] = 1;
    symbol_table["ARG"] = 2;
    symbol_table["THIS"] = 3;
    symbol_table["THAT"] = 4;

}

void Encoder::map_goto_labels(const std::vector<std::shared_ptr<Statement>>& statements, std::map<std::string, int>& symbol_table) const
{
    int statement_line_number = 0;
    for (int i = 0; i < statements.size(); i++)
    {
        if ((statements[i])->is_label())
        {
            symbol_table[statements[i]->get_identifier()] = statement_line_number;
        }
        else
        {
            statement_line_number++;
        }
    }
}
