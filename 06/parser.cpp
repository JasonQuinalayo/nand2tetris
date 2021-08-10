#include "parser.hpp"
#include <ctype.h>
#include <iostream>
#include <algorithm>

Parser::Parser() : c_instruction_comp_map(get_c_instruction_comp_map()) , c_instruction_jump_map(get_c_instruction_jump_map())
{
}

std::vector<std::shared_ptr<Statement>> Parser::parse(std::ifstream& input_file_stream) 
{
    results.clear();
    std::string line;
    int line_number = 1;
    while (std::getline(input_file_stream, line))
    {
        const std::vector<std::string> line_tokens = get_line_tokens(line);
        analyze_line_tokens(line_tokens);
    }
    return results;
}

std::map<std::string, CInstruction::Computation> Parser::get_c_instruction_comp_map()
{
    typedef CInstruction::Computation comp;
    std::map<std::string, CInstruction::Computation> map;
    map["0"]    = comp::zero;
    map["1"]    = comp::one;
    map["-1"]   = comp::minus_one;
    map["D"]    = comp::d;
    map["A"]    = comp::a;
    map["!D"]   = comp::not_d;
    map["!A"]   = comp::not_a;
    map["-D"]   = comp::minus_d;
    map["-A"]   = comp::minus_a;
    map["D+1"]  = comp::d_plus_one;
    map["A+1"]  = comp::a_plus_one;
    map["D-1"]  = comp::d_minus_one;
    map["A-1"]  = comp::a_minus_one;
    map["D+A"]  = comp::d_plus_a;
    map["D-A"]  = comp::d_minus_a;
    map["A-D"]  = comp::a_minus_d;
    map["D&A"]  = comp::d_and_a;
    map["D|A"]  = comp::d_or_a;
    map["M"]    = comp::m;
    map["!M"]   = comp::not_m;
    map["-M"]   = comp::minus_m;
    map["M+1"]  = comp::m_plus_one;
    map["M-1"]  = comp::m_minus_one;
    map["D+M"]  = comp::d_plus_m;
    map["D-M"]  = comp::d_minus_m;
    map["M-D"]  = comp::m_minus_d;
    map["D&M"]  = comp::d_and_m;
    map["D|M"]  = comp::d_or_m;
    return map;
}

std::map<std::string, CInstruction::Jump> Parser::get_c_instruction_jump_map()
{
    typedef CInstruction::Jump jump;
    std::map<std::string, CInstruction::Jump> map;
    map[""]     = jump::none;
    map["JGT"]  = jump::jgt;
    map["JEQ"]  = jump::jeq;
    map["JGE"]  = jump::jge;
    map["JLT"]  = jump::jlt;
    map["JNE"]  = jump::jne;
    map["JLE"]  = jump::jle;
    map["JMP"]  = jump::jmp;
    return map;
}

void Parser::analyze_line_tokens(const std::vector<std::string>& tokens)
{
    if (tokens.empty()) return;
    if (tokens[0].compare("@") == 0)
    {
        add_a_instruction_statement(tokens);
    }
    else if (tokens[0].compare("(") == 0)
    {
        add_goto_label_statement(tokens);
    }
    else
    {
        add_c_instruction_statement(tokens);
    }
}

void Parser::add_a_instruction_statement(const std::vector<std::string>& tokens)
{
    results.push_back(std::make_shared<AInstruction>(AInstruction(!isdigit(tokens[1][0]), tokens[1])));
}

void Parser::add_c_instruction_statement(const std::vector<std::string>& tokens)
{
    std::string destination;
    std::string computation;
    std::string jump;
    if (tokens.size() == 5) // all three
    {
        destination = tokens[0];
        computation = tokens[2];
        jump = tokens[4];
    }
    else if (tokens[1].compare(";") == 0) // comp;jump only
    {
        computation = tokens[0];
        jump = tokens[2];
    }
    else // dest=comp only
    {
        destination=tokens[0];
        computation=tokens[2];
    }
    bool dest_a = false, dest_m = false, dest_d = false;
    if (destination.find("A") != std::string::npos) dest_a = true;
    if (destination.find("M") != std::string::npos) dest_m = true;
    if (destination.find("D") != std::string::npos) dest_d = true;
    CInstruction::Destination dest{dest_a, dest_m, dest_d};
    results.push_back(std::make_shared<CInstruction>
        (CInstruction(dest, c_instruction_comp_map.find(computation)->second, c_instruction_jump_map.find(jump)->second)));
}

void Parser::add_goto_label_statement(const std::vector<std::string>& tokens)
{
    results.push_back(std::make_shared<GotoLabel>(GotoLabel(tokens[1])));
}

std::vector<std::string> Parser::get_line_tokens(const std::string& line) const
{
    std::vector<std::string> tokens;
    int first_char_index = get_char_index(line, 0);
    if (first_char_index == -1) 
    {
        return tokens;
    }
    char c = line[first_char_index];
    if (c == '@')
    {
        get_a_instruction_tokens(line, tokens);
    }
    else if (c == '(')
    {
        get_goto_label_tokens(line, tokens);
    }
    else if (c == '/')
    {
        return tokens;
    }
    else
    {
        get_c_instruction_tokens(line, tokens);
    }
    return tokens;
}

void Parser::get_a_instruction_tokens(const std::string& line, std::vector<std::string>& tokens) const
{
    tokens.push_back("@");
    int cur = get_char_index(line, 0) + 1;
    std::string value = get_chars_until(line, cur, "/").first;
    tokens.push_back(value);
}

void Parser::get_c_instruction_tokens(const std::string& line, std::vector<std::string>& tokens) const
{
    std::pair<std::string, int> first_component = get_chars_until(line, 0, "=;");
    tokens.push_back(first_component.first);
    tokens.push_back(std::string(1, line[first_component.second]));
    std::pair<std::string, int> second_component = get_chars_until(line, first_component.second + 1, "/;");
    tokens.push_back(second_component.first);
    if (second_component.second != -1 && line[second_component.second] != '/') 
    {
        tokens.push_back(";");
        tokens.push_back(get_chars_until(line, second_component.second + 1).first);
    }
}

void Parser::get_goto_label_tokens(const std::string& line, std::vector<std::string>& tokens) const
{
    tokens.push_back("(");
    int cur = get_char_index(line, 0) + 1;
    std::string identifier = get_chars_until(line, cur, ")").first;
    tokens.push_back(identifier);
    tokens.push_back(")");
}

// ignores white space
std::pair<std::string, int> Parser::get_chars_until(const std::string& line, int start, const std::string& delimiters) const
{
    int cur = get_char_index(line, start);
    std::string str = "";
    while (cur != -1 && (delimiters.empty() || delimiters.find(line[cur]) == std::string::npos))
    {
        str += line[cur];
        cur = get_char_index(line, cur + 1);
    }
    return std::pair<std::string, int> (str, cur);
}

int Parser::get_char_index(const std::string& line, int cur) const
{
    while (cur < line.size())
    {
        if (!isspace(line[cur])) return cur;
        else cur++;
    }
    return -1;
}
