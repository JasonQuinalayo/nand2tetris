#pragma once
#include "code_generators.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <map>

class Parser
{
    std::vector<std::shared_ptr<Statement>> results;
    const std::map<std::string, CInstruction::Computation> c_instruction_comp_map;
    const std::map<std::string, CInstruction::Jump> c_instruction_jump_map;
public:
    Parser();
    std::vector<std::shared_ptr<Statement>> parse(std::ifstream& input_file_stream);
private:
    void analyze_line_tokens(const std::vector<std::string>& tokens);
    std::map<std::string, CInstruction::Computation> get_c_instruction_comp_map();
    std::map<std::string, CInstruction::Jump> get_c_instruction_jump_map();
    void add_a_instruction_statement(const std::vector<std::string>& tokens);
    void add_c_instruction_statement(const std::vector<std::string>& tokens);
    void add_goto_label_statement(const std::vector<std::string>& tokens);
    std::vector<std::string> get_line_tokens(const std::string& line) const;
    void get_a_instruction_tokens(const std::string& line, std::vector<std::string>& tokens) const;
    void get_c_instruction_tokens(const std::string& line, std::vector<std::string>& tokens) const;
    void get_goto_label_tokens(const std::string& line, std::vector<std::string>& tokens) const;
    std::pair<std::string, int> get_chars_until(const std::string& line, int start, const std::string& delimiters = "") const;
    int get_char_index(const std::string& line, int cur) const;
    void error(int line_number) const;
};
