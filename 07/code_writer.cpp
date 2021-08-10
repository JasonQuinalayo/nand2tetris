#include "code_writer.hpp"

CodeWriter::CodeWriter(std::ofstream* output_file_stream) : 
    output_file_stream(output_file_stream) 
{
}

void CodeWriter::write_inf_loop()
{
    write_lines(InfLoop().generate_code());
}

void CodeWriter::write_code(const std::vector<std::shared_ptr<Statement>>& statements, const std::string& file_name)
{
    Statement::set_file_name(file_name);
    for (auto statement : statements)
    {
        write_lines((*statement).generate_code());
    }
}

void CodeWriter::write_lines(const std::vector<std::string>& assembly_statements)
{
    for (const std::string& assembly_statement : assembly_statements)
        (*output_file_stream) << assembly_statement << std::endl;
}
