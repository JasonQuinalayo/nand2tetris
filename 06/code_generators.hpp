#pragma once
#include <string>
#include <map>

class Statement 
{
public:
    virtual bool is_label() const;
    virtual std::string get_identifier() const;
    virtual std::string generate_code() const;
    virtual ~Statement() {}
};

class AInstruction : public Statement 
{
    const bool is_identifier;
    const std::string value;
    static std::map<std::string, int> symbol_table;
    static const int base_var_address = 16;
    static int var_count;
public:
    AInstruction(bool isIdentifier, const std::string& value);
    std::string generate_code() const;
    static void set_symbol_table(std::map<std::string, int>& a_symbol_table);
};

class CInstruction : public Statement
{
public:
    struct Destination
    {
        const bool a, m, d;
    };
    enum Computation
    {
        zero,
        one,
        minus_one,
        d,
        a,
        not_d,
        not_a,
        minus_d,
        minus_a,
        d_plus_one,
        a_plus_one,
        d_minus_one,
        a_minus_one,
        d_plus_a,
        d_minus_a,
        a_minus_d,
        d_and_a,
        d_or_a,
        m,
        not_m,
        minus_m,
        m_plus_one,
        m_minus_one,
        d_plus_m,
        d_minus_m,
        m_minus_d,
        d_and_m,
        d_or_m,
    };
    enum Jump
    {
        none,
        jgt,
        jeq,
        jge,
        jlt,
        jne,
        jle,
        jmp
    };
    CInstruction(Destination dest, Computation comp, Jump jump);
    std::string generate_code() const;
private:
    const Destination destination;
    const Computation computation;
    const Jump jump;
    std::string get_jump_bits() const;
    std::string get_comp_bits() const;
    std::string get_dest_bits() const;
};

class GotoLabel: public Statement
{
    const std::string identifier;
public:
    GotoLabel(const std::string& identifier);
    bool is_label() const;
    std::string get_identifier() const;
};
