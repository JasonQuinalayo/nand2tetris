#include "parser.hpp"
#include <ctype.h>

Parser::Parser() : memory_segment_map(create_memory_segments_map()), arithmetic_operation_map(create_arithmetic_operations_map()) {}

std::vector<std::shared_ptr<Statement>> Parser::parse(std::ifstream& input_file_stream)
{
    results.clear();
    std::string line;
    int line_number = 1;
    while (std::getline(input_file_stream, line))
    {
        const std::vector<std::string> line_tokens = get_line_tokens(line);
        analyze_line_tokens(line_tokens, line_number++);
    }
    return results;
}

std::map<std::string, MemoryAccess::Segment> Parser::create_memory_segments_map() const
{
    std::map<std::string, MemoryAccess::Segment> results; 
    typedef MemoryAccess::Segment sgmnt;
    results["local"]     = sgmnt::ms_local;
    results["argument"]  = sgmnt::ms_argument;
    results["this"]      = sgmnt::ms_this;
    results["that"]      = sgmnt::ms_that;
    results["constant"]  = sgmnt::ms_constant;
    results["static"]    = sgmnt::ms_static;
    results["temp"]      = sgmnt::ms_temp;
    results["pointer"]   = sgmnt::ms_pointer;
    return results;
}

std::map<std::string, Arithmetic::Operation> Parser::create_arithmetic_operations_map() const
{
    std::map<std::string, Arithmetic::Operation> results; 
    typedef Arithmetic::Operation ao;
    results["add"] = ao::ao_add;
    results["sub"] = ao::ao_sub;
    results["neg"] = ao::ao_neg;
    results["eq"]  = ao::ao_eq;
    results["gt"]  = ao::ao_gt;
    results["lt"]  = ao::ao_lt;
    results["and"] = ao::ao_and;
    results["or"]  = ao::ao_or;
    results["not"] = ao::ao_not;
    return results;
}

std::vector<std::string> Parser::get_line_tokens(const std::string& line) const
{
    std::vector<std::string> tokens;
    int next_char_index = get_next_char_index(line, 0);
    while (next_char_index != -1 && line[next_char_index] != '/')
    {
        std::pair<std::string, int> token = get_token(line, next_char_index);
        tokens.push_back(token.first);
        next_char_index = get_next_char_index(line, token.second + 1);
    }
    return tokens;
}

/* Extracts string until it encounters a white space or the end of the line and 
   returns a pair consisting of the string and the index of the last character plus one*/
std::pair<std::string, int> Parser::get_token(const std::string& line, int start) const
{
    std::string token = "";
    int cur = start;
    while (cur < line.size() && !isspace(line[cur]))
        token += line[cur++];
    return std::pair<std::string, int> (token, cur);
}

int Parser::get_next_char_index(const std::string& line, int start) const
{
    int cur = start;
    while (cur < line.size())
    {
        if (!isspace(line[cur])) return cur;
        else cur++;
    }
    return -1;
}

void Parser::analyze_line_tokens(const std::vector<std::string>& tokens, int line_number)
{
    if (tokens.empty()) return;
    if (tokens[0] == "push" || tokens[0] == "pop")
    {
        add_memory_access_statement(tokens);
    }
    else if (tokens[0] == "label" || tokens[0] == "goto" || tokens[0] == "if-goto")
    {
        add_branching_statement(tokens);
    }
    else if (tokens[0] == "function" || tokens[0] == "call")
    {
        add_function_statement(tokens);
    } else if (tokens[0] == "return")
    {
        add_return_statement();
    }
    else
    {
        add_arithmetic_operation_statement(tokens);
    }
}

void Parser::add_memory_access_statement(const std::vector<std::string>& tokens)
{
    results.push_back(std::make_shared<MemoryAccess>
    (
        MemoryAccess
        (
            tokens[0] == "push" ? MemoryAccess::Operation::push : MemoryAccess::Operation::pop, 
            memory_segment_map.find(tokens[1])->second, 
            std::stoi(tokens[2])
        )
    ));
}

void Parser::add_arithmetic_operation_statement(const std::vector<std::string>& tokens)
{
    results.push_back(std::make_shared<Arithmetic>(Arithmetic(arithmetic_operation_map.find(tokens[0])->second)));
}

void Parser::add_branching_statement(const std::vector<std::string>& tokens)
{
    Branching::BranchingType type;
    if (tokens[0] == "label")       type = Branching::BranchingType::b_label;
    else if (tokens[0] == "goto")   type = Branching::BranchingType::b_goto;
    else                            type = Branching::BranchingType::b_if_goto;
    results.push_back(std::make_shared<Branching>(Branching(type, tokens[1])));
}

void Parser::add_function_statement(const std::vector<std::string>& tokens)
{
    Function::FunctionStatementType type = tokens[0] == "function" ? 
        Function::FunctionStatementType::declare : Function::FunctionStatementType::call;
    results.push_back(std::make_shared<Function>(Function(type, tokens[1], std::stoi(tokens[2]))));
}

void Parser::add_return_statement()
{
    results.push_back(std::make_shared<Return>(Return()));
}