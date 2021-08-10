#include "code_generators.hpp"

std::string Statement::file_name;

void Statement::set_file_name(const std::string& a_file_name)
{
    file_name = a_file_name;
}

void Statement::pop_stack_to_d()
{
    append_code
    ({
        "@SP",
        "M=M-1",
        "A=M",
        "D=M"
    });
}

void Statement::push_value_in_d_to_stack() 
{
    append_code
    ({
        "@SP",
        "A=M",
        "M=D",
        "@SP",
        "M=M+1"
    });
}

void Statement::push_value_in_d_to(const std::string& address)
{
    append_code
    ({
        "@" + address,
        "M=D"
    });
}

void Statement::push_value_in_d_to_pointed_address(const std::string& pointer_address)
{
    append_code
    ({
        "@" + pointer_address,
        "A=M",
        "M=D"
    });
}

void Statement::load_d_with_a_val(const std::string& value)
{
    append_code
    ({
        "@" + value,
        "D=A",
    });
}

void Statement::load_d_with_m_val(const std::string& address)
{
    append_code
    ({
        "@" + address,
        "D=M"
    });
}

void Statement::load_r13()
{
    append_code
    ({
        "@R13",
        "M=D"
    });
}

void Statement::operate_top_stack_one_element(const std::string& op)
{
    append_code
    ({
        "@SP",
        "A=M-1",
        "M=" + op
    });
}

void Statement::goto_(const std::string& address)
{
    append_code
    ({
        "@" + address,
        "0;JMP"
    });
}

void Statement::append_code(const std::vector<std::string>& lines)
{
    (*code_ptr).insert((*code_ptr).end(), lines.begin(), lines.end());
}

std::vector<std::string> InfLoop::generate_code()
{
    std::vector<std::string> code;
    code_ptr = &code;
    append_code({"(INF)"});
    goto_("INF");
    return code;
}

MemoryAccess::MemoryAccess(Operation operation, Segment segment, int offset) : operation(operation), segment(segment), offset(offset) {}

std::vector<std::string> MemoryAccess::generate_code()
{
    std::vector<std::string> code;
    code_ptr = &code;
    if (operation == Operation::push)
    {
        if (segment == Segment::ms_constant)
            push_constant();
        else if (segment == Segment::ms_static)
            push_static_var();
        else if (segment == Segment::ms_temp || segment == Segment::ms_pointer)
            push_statically_allocated_ms();
        else // lcl, arg, this, that
            push_dynamically_allocated_ms();
    }
    else
    {
        if (segment == Segment::ms_static)
            pop_static_var();
        else if (segment == Segment::ms_temp || segment == Segment::ms_pointer)
            pop_statically_allocated_ms();
        else
            pop_dynamically_allocated_ms();

    }
    return code;
}


void MemoryAccess::push_dynamically_allocated_ms()
{
    append_code({"// PUSH DYNAMIC ALLOC MS"});
    load_d_with_a_val(std::to_string(offset));
    append_code
    ({
        "@" + get_dynamic_segment_string(),
        "A=M+D",
        "D=M"
    });
    push_value_in_d_to_stack();
}
void MemoryAccess::pop_dynamically_allocated_ms()
{
    const std::string offset_str = std::to_string(offset);
    append_code({"// POP DYNAMIC ALLOC MS"});
    load_d_with_a_val(offset_str);
    append_code
    ({
        "@" + get_dynamic_segment_string(),
        "D=D+M"
    });
    load_r13();
    pop_stack_to_d();
    push_value_in_d_to_pointed_address("R13");
}

void MemoryAccess::push_constant()
{
    append_code({"// PUSH CONSTANT"});
    load_d_with_a_val(std::to_string(offset));
    push_value_in_d_to_stack();
}
void MemoryAccess::push_static_var()
{
    const std::string var_name = file_name + "." + std::to_string(offset);
    append_code({"// PUSH STATIC"});
    load_d_with_m_val(var_name);
    push_value_in_d_to_stack();
}
void MemoryAccess::pop_static_var()
{
    const std::string var_name = file_name + "." + std::to_string(offset);
    append_code({"// POP STATIC"});
    load_d_with_a_val(var_name);
    load_r13();
    pop_stack_to_d();
    push_value_in_d_to_pointed_address("R13");
}
void MemoryAccess::push_statically_allocated_ms()
{
    const std::string comment = "// PUSH" + get_static_alloc_ms_comment();
    const std::string address = get_static_alloc_ms_address();
    append_code({comment});
    load_d_with_a_val(std::to_string(offset));
    append_code
    ({
        "@" + address,
        "A=D+A",
        "D=M"
    });
    push_value_in_d_to_stack();
}
void MemoryAccess::pop_statically_allocated_ms()
{
    const std::string comment = "// POP" + get_static_alloc_ms_comment();
    const std::string address = get_static_alloc_ms_address();
    append_code({comment});
    load_d_with_a_val(address);
    append_code
    ({
        "@" + std::to_string(offset),
        "D=D+A"
    });
    load_r13();
    pop_stack_to_d();
    push_value_in_d_to_pointed_address("R13");
}

std::string MemoryAccess::get_dynamic_segment_string() const
{
    switch(segment)
    {
        case ms_local:    return "LCL";
        case ms_argument: return "ARG";
        case ms_this:     return "THIS";
        case ms_that:     return "THAT";
    }
    return "";
}

std::string MemoryAccess::get_static_alloc_ms_comment() const
{
    if (segment == ms_temp) return "TEMP";
    return "POINTER";
}

std::string MemoryAccess::get_static_alloc_ms_address() const
{
    if (segment == ms_temp) return "5";
    return "3";
}

Arithmetic::Arithmetic(Operation operation) : operation(operation) {}

std::vector<std::string> Arithmetic::generate_code()
{
    std::vector<std::string> code;
    code_ptr = &code;
    if (operation == ao_gt || operation == ao_eq || operation == ao_lt)
        compare();
    else
        non_compare();
    return code;
}

int Arithmetic::jump_count = 0;

void Arithmetic::non_compare()
{
    if (operation == ao_neg || operation == ao_not)
        single_element_op();
    else 
        double_element_op();
}

void Arithmetic::single_element_op()
{
    const std::string op = get_noncompare_op_string();
    const std::string comment = get_noncompare_comment_string();
    append_code({comment});
    operate_top_stack_one_element(op);
}

void Arithmetic::double_element_op()
{
    const std::string op = get_noncompare_op_string();
    const std::string comment = get_noncompare_comment_string();
    append_code({comment});
    pop_stack_to_d();
    append_code
    ({
        "A=A-1",
        "M=" + op
    });
}

void Arithmetic::compare()
{
    const std::string jump_label = std::to_string(jump_count++);
    const std::string comment = get_compare_comment_string();
    const std::string cond = get_compare_cond_string();
    append_code({comment});
    pop_stack_to_d();
    append_code
    ({
        "A=A-1",
        "D=M-D",
        "@FALSE_JUMP" + jump_label,
        cond
    });
    operate_top_stack_one_element("-1");
    goto_("END_JUMP" + jump_label);
    append_code({"(FALSE_JUMP" + jump_label + ")"});
    operate_top_stack_one_element("0");
    append_code({"(END_JUMP" + jump_label + ")"});
}

std::string Arithmetic::get_noncompare_op_string() const
{
    switch (operation)
    {
        case ao_add: return "D+M";
        case ao_sub: return "M-D";
        case ao_neg: return "-M";
        case ao_not: return "!M";
        case ao_or: return "D|M";
        case ao_and: return "D&M";
    }
    return "";
}

std::string Arithmetic::get_noncompare_comment_string() const
{
    switch (operation)
    {
        case ao_add: return "// ADD";
        case ao_sub: return "// SUB";
        case ao_neg: return "// NEG";
        case ao_not: return "// NOT";
        case ao_or: return "// OR";
        case ao_and: return "// AND";
    }
    return "";
}

std::string Arithmetic::get_compare_comment_string() const
{
    if (operation == ao_eq)
        return "// EQ";
    else if (operation == ao_gt)
        return "// GT";
    return "// LT";
}

std::string Arithmetic::get_compare_cond_string() const
{
    if (operation == ao_eq)
        return "D;JNE";
    else if (operation == ao_gt)
        return "D;JLE";
    return "D;JGE";
}
