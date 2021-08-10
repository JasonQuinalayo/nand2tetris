#pragma once
#include "code_generators.hpp"
#include <vector>
#include <fstream>
#include <memory>
#include <map>
#include <string>

class Parser
{
    const std::map<std::string, MemoryAccess::Segment> memory_segment_map;
    const std::map<std::string, Arithmetic::Operation> arithmetic_operation_map;
    std::vector<std::shared_ptr<Statement>> results;
public:
    Parser();
    std::vector<std::shared_ptr<Statement>> parse(std::ifstream& input_file_stream);
private:
    std::map<std::string, MemoryAccess::Segment> create_memory_segments_map() const;
    std::map<std::string, Arithmetic::Operation> create_arithmetic_operations_map() const;
    std::vector<std::string> get_line_tokens(const std::string& line) const;
    void analyze_line_tokens(const std::vector<std::string>& tokens, int line_number);
    void add_memory_access_statement(const std::vector<std::string>& tokens);
    void add_arithmetic_operation_statement(const std::vector<std::string>& tokens);
    void add_branching_statement(const std::vector<std::string>& tokens);
    void add_function_statement(const std::vector<std::string>& tokens);
    void add_return_statement();
    std::pair<std::string, int> get_token(const std::string& line, int start) const;
    int get_next_char_index(const std::string& line, int cur) const;
};