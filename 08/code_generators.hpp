#pragma once
#include <string>
#include <vector>

class Statement
{
public:
    virtual std::vector<std::string> generate_code() = 0;
    virtual ~Statement() {};
    static void set_file_name(const std::string& a_file_name);
protected:
    static std::string file_name;
    std::vector<std::string>* code_ptr;
    void pop_stack_to_d();
    void push_value_in_d_to_stack();
    void push_value_in_d_to(const std::string& address);
    void push_value_in_d_to_pointed_address(const std::string& pointer_address);
    void load_d_with_a_val(const std::string& value);
    void load_d_with_m_val(const std::string& address);
    void load_r13();
    void operate_top_stack_one_element(const std::string& op);
    void goto_(const std::string& address);
    void append_code(const std::vector<std::string>& lines);
};

class BootStrap : public Statement
{
public:
    std::vector<std::string> generate_code();
};

class InfLoop : public Statement
{
public:
    std::vector<std::string> generate_code();
};

class MemoryAccess : public Statement
{
public:
    enum Operation
    {
        push, pop
    };
    enum Segment
    {
        ms_local, ms_argument, ms_this, ms_that, ms_constant, ms_static, ms_temp, ms_pointer
    };
    MemoryAccess(Operation operation, Segment segment, int offset);
    std::vector<std::string> generate_code();
private:
    const Operation operation;
    const Segment segment;
    const int offset;
    void push_dynamically_allocated_ms();
    void pop_dynamically_allocated_ms();
    void push_constant();
    void push_static_var();
    void pop_static_var();
    void push_statically_allocated_ms();
    void pop_statically_allocated_ms();
    std::string get_dynamic_segment_string() const;
    std::string get_static_alloc_ms_comment() const;
    std::string get_static_alloc_ms_address() const;
};

class Arithmetic : public Statement
{
public:
    enum Operation
    {
        ao_add, ao_sub, ao_neg, ao_eq, ao_gt, ao_lt, ao_and, ao_or, ao_not
    };
    Arithmetic(Operation operation);
    std::vector<std::string> generate_code();
private:
    const Operation operation;
    static int jump_count;
    void non_compare();
    void single_element_op();
    void double_element_op();
    void compare();
    std::string get_noncompare_op_string() const;
    std::string get_noncompare_comment_string() const;
    std::string get_compare_cond_string() const;
    std::string get_compare_comment_string() const;
};

class Branching : public Statement
{
public:
    enum BranchingType
    {
        b_label, b_goto, b_if_goto
    };
    Branching(BranchingType branching_type, const std::string& label);
    std::vector<std::string> generate_code();
private:
    const BranchingType branching_type;
    const std::string label;
    void if_goto_statement();
};

class Function : public Statement
{
public:
    enum FunctionStatementType
    {
        declare, call
    };
    Function(FunctionStatementType function_statement_type, const std::string& function_name, int vars);
    std::vector<std::string> generate_code();
private:
    FunctionStatementType function_statement_type;
    const std::string function_name;
    const int vars;
    static int function_call_count;
    void function_statement();
    void call_statement();
};

class Return : public Statement 
{
public:
    Return() {};
    std::vector<std::string> generate_code();
};
