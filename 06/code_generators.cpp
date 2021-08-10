#include "code_generators.hpp"
#include <bitset>
#include <iostream>

bool Statement::is_label() const
{
    return false;
}

std::string Statement::get_identifier() const
{
    return "";
}

std::string Statement::generate_code() const
{
    return "";
}

AInstruction::AInstruction(bool is_identifier, const std::string& value) : is_identifier(is_identifier), value(value) {}

int AInstruction::var_count = 0;
std::map <std::string, int> AInstruction::symbol_table;

std::string AInstruction::generate_code() const
{
    std::string binary_string;
    if (is_identifier)
    {
        auto it = symbol_table.find(value);
        int var_address;
        if (it == symbol_table.end())
        {
            var_address = base_var_address + var_count;
            symbol_table[value] = var_address;
            var_count++;
        }
        else
        {
            var_address = it->second;
        }
        binary_string = std::bitset<16>(var_address).to_string();
    }
    else
    {
        binary_string = std::bitset<16>(std::stoi(value)).to_string();
    }
    return binary_string;
}

void AInstruction::set_symbol_table(std::map<std::string, int>& a_symbol_table)
{
    symbol_table = a_symbol_table;
}

CInstruction::CInstruction(Destination dest, Computation comp, Jump jump) 
    : destination(dest), computation(comp), jump(jump) {}

std::string CInstruction::generate_code() const
{
    std::string binary_string = "111";
    binary_string += get_comp_bits() + get_dest_bits() + get_jump_bits();
    return binary_string;
}

std::string CInstruction::get_comp_bits() const
{
    switch(computation)
    {
        case zero:        return "0101010";
        case one:         return "0111111";
        case minus_one:   return "0111010";
        case d:           return "0001100";
        case a:           return "0110000";
        case not_d:       return "0001101";
        case not_a:       return "0110001";
        case minus_d:     return "0001111";
        case minus_a:     return "0110011";
        case d_plus_one:  return "0011111";
        case a_plus_one:  return "0110111";
        case d_minus_one: return "0001110";
        case a_minus_one: return "0110010";
        case d_plus_a:    return "0000010";
        case d_minus_a:   return "0010011";
        case a_minus_d:   return "0000111";
        case d_and_a:     return "0000000";
        case d_or_a:      return "0010101";
        case m:           return "1110000";
        case not_m:       return "1110001";
        case minus_m:     return "1110011";
        case m_plus_one:  return "1110111";
        case m_minus_one: return "1110010";
        case d_plus_m:    return "1000010";
        case d_minus_m:   return "1010011";
        case m_minus_d:   return "1000111";
        case d_and_m:     return "1000000";
        case d_or_m:      return "1010101";
    }
    return "";
}

std::string CInstruction::get_jump_bits() const
{
    switch(jump)
    {
        case none: return "000";
        case jgt:  return "001";
        case jeq:  return "010";
        case jge:  return "011";
        case jlt:  return "100";
        case jne:  return "101";
        case jle:  return "110";
        case jmp:  return "111";
    }
    return "";
}

std::string CInstruction::get_dest_bits() const
{
    std::string dest_bits = "";
    if (destination.a)  dest_bits += "1";
    else                dest_bits += "0";

    if (destination.d)  dest_bits += "1";
    else                dest_bits += "0";

    if (destination.m)  dest_bits += "1";
    else                dest_bits += "0";
    return dest_bits;
}

GotoLabel::GotoLabel(const std::string& identifier) : identifier(identifier) {}

bool GotoLabel::is_label() const
{
    return true;
}

std::string GotoLabel::get_identifier() const
{
    return identifier;
}
