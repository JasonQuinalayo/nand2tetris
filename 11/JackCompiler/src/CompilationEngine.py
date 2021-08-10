from Tokenizer import Tokenizer
from Constants import Symbols, Keywords, TokenTypes, BinaryOps, UnaryOps, MemSegments, SubroutineTypes
from VMWriter import VMWriter
from SymbolTable import Variable, SymbolTable
from os import remove
from sys import exit


class CompilationEngine:
    _class_subroutine_dec_keywords = {
        Keywords.METHOD, Keywords.CONSTRUCTOR, Keywords.FUNCTION}
    _class_var_dec_keywords = {
        Keywords.FIELD, Keywords.STATIC}
    _binary_ops_symbols = {
        Symbols.PLUS, Symbols.MINUS, Symbols.ASTERISK, Symbols.FORWARD_SLASH,
        Symbols.AMPERSAND, Symbols.VERTICAL_BAR, Symbols.LESS_THAN, Symbols.GREATER_THAN, Symbols.EQUAL
    }
    _unary_ops_symbols = {
        Symbols.TILDE, Symbols.MINUS
    }
    _keyword_constants = {
        Keywords.THIS, Keywords.TRUE, Keywords.FALSE, Keywords.NULL
    }

    def __init__(self):
        self._tokenizer = None
        self._writer = VMWriter()
        self._cur_tok = None
        self._next_tok = None
        self._cur_output_file_name = ''
        self._cur_class_name = ''
        self._cur_class_sym_table = None
        self._cur_subroutine_sym_table = None
        self._cur_subroutine_type = None
        self._cur_subroutine_name = ''
        self._if_count = 0
        self._while_count = 0
        self._num_of_field_vars = 0

    def compile(self, input_file_name):
        self._tokenizer = Tokenizer(input_file_name)
        self._num_of_field_vars = 0
        self._if_count = 0
        self._while_count = 0
        self._cur_tok = self._tokenizer.get_cur_tok()
        if input_file_name[-5:] == ".jack":
            input_file_name = input_file_name[:-5]
        output_file_name = input_file_name + ".vm"
        self._cur_output_file_name = output_file_name
        with open(output_file_name, 'w') as output_stream:
            self._writer.set_output_file(output_stream)
            self._compile_class()

    def _compile_class(self):
        self._cur_class_sym_table = SymbolTable()
        self._advance()     # class
        self._cur_class_name = self._eat_cur_tok().get_val()
        self._advance()
        self._compile_class_var_dec()
        self._compile_class_subroutine_dec()

    def _compile_class_var_dec(self):
        while self._cur_tok.get_keyword_type() not in self._class_subroutine_dec_keywords:
            kind = Variable.Kind.FIELD if self._eat_cur_tok().get_keyword_type() == Keywords.FIELD else \
                Variable.Kind.STATIC
            type_ = self._eat_cur_tok().get_val()
            self._cur_class_sym_table.add(self._eat_cur_tok().get_val(), type_, kind)
            self._num_of_field_vars += 1
            while self._eat_cur_tok().get_symbol_type() != Symbols.SEMI_COLON:
                self._cur_class_sym_table.add(self._eat_cur_tok().get_val(), type_, kind)
                if kind == Variable.Kind.FIELD:
                    self._num_of_field_vars += 1

    def _compile_class_subroutine_dec(self):
        while self._cur_tok.get_symbol_type() != Symbols.CLOSE_CURLY:
            self._cur_subroutine_sym_table = SymbolTable()
            keyword_type = self._eat_cur_tok().get_keyword_type()
            if keyword_type == Keywords.METHOD:
                self._cur_subroutine_type = SubroutineTypes.METHOD
            elif keyword_type == Keywords.CONSTRUCTOR:
                self._cur_subroutine_type = SubroutineTypes.CONSTRUCTOR
            else:
                self._cur_subroutine_type = SubroutineTypes.FUNCTION
            self._advance()
            self._cur_subroutine_name = self._eat_cur_tok().get_val()
            self._advance()
            self._compile_param_list()
            self._advance()
            self._compile_subroutine_body()

    def _compile_param_list(self):
        if self._cur_subroutine_type == SubroutineTypes.METHOD:
            self._cur_subroutine_sym_table.add('this', self._cur_class_name, Variable.Kind.ARGUMENT)
        while self._cur_tok.get_symbol_type() != Symbols.CLOSE_PAREN:
            type_ = self._eat_cur_tok().get_val()
            name = self._eat_cur_tok().get_val()
            self._cur_subroutine_sym_table.add(name, type_, Variable.Kind.ARGUMENT)
            if self._cur_tok.get_symbol_type() == Symbols.COMMA:
                self._advance()

    def _compile_subroutine_body(self):
        self._advance()
        self._compile_subroutine_var_dec()
        if self._cur_subroutine_type == SubroutineTypes.CONSTRUCTOR:
            self._writer.write_push(MemSegments.CONSTANT, self._num_of_field_vars)
            self._writer.write_call('Memory.alloc', 1)
            self._writer.write_pop(MemSegments.POINTER, 0)
        elif self._cur_subroutine_type == SubroutineTypes.METHOD:
            self._writer.write_push(MemSegments.ARGUMENT, 0)
            self._writer.write_pop(MemSegments.POINTER, 0)
        self._compile_statements()
        self._advance()

    def _compile_subroutine_var_dec(self):
        num_of_lcl_vars = 0
        while self._cur_tok.get_keyword_type() == Keywords.VAR:
            kind = Variable.Kind.LOCAL
            self._advance()
            type_ = self._eat_cur_tok().get_val()
            self._cur_subroutine_sym_table.add(self._eat_cur_tok().get_val(), type_, kind)
            num_of_lcl_vars += 1
            while self._eat_cur_tok().get_symbol_type() != Symbols.SEMI_COLON:
                self._cur_subroutine_sym_table.add(self._eat_cur_tok().get_val(), type_, kind)
                num_of_lcl_vars += 1
        self._writer.write_func_name(self._cur_class_name + '.' + self._cur_subroutine_name, num_of_lcl_vars)

    def _compile_statements(self):
        while self._cur_tok.get_symbol_type() != Symbols.CLOSE_CURLY:
            keyword = self._cur_tok.get_keyword_type()
            if keyword == Keywords.LET:
                self._compile_let()
            elif keyword == Keywords.DO:
                self._compile_do()
            elif keyword == Keywords.WHILE:
                self._compile_while()
            elif keyword == Keywords.IF:
                self._compile_if()
            else:
                self._compile_return()

    def _compile_let(self):
        self._advance()
        var_name_tok = self._eat_cur_tok()
        var_entry = self._get_var_entry(var_name_tok.get_val())
        if self._eat_cur_tok().get_symbol_type() == Symbols.OPEN_SQUARE:
            self._writer.write_push(var_entry.kind.value, var_entry.num)
            self._compile_expression()
            self._writer.write_op(BinaryOps.ADD)
            self._advance(2)
            self._compile_expression()
            self._writer.write_pop(MemSegments.TEMP, 0)
            self._writer.write_pop(MemSegments.POINTER, 1)
            self._writer.write_push(MemSegments.TEMP, 0)
            self._writer.write_pop(MemSegments.THAT, 0)
        else:
            self._compile_expression()
            self._writer.write_pop(var_entry.kind.value, var_entry.num)
        self._advance()

    def _compile_do(self):
        self._advance()
        self._compile_subroutine_call()
        self._writer.write_pop(MemSegments.TEMP, 0)
        self._advance()

    def _compile_if(self):
        label = self._eat_cur_if_label()
        end_label = 'end_' + label
        self._advance(2)     # if (
        self._compile_expression()
        self._writer.write_op(UnaryOps.NOT)
        self._writer.write_if_goto(label)
        self._advance(2)     # ) {
        self._compile_statements()
        self._advance()      # }
        self._writer.write_goto(end_label)
        self._writer.write_label(label)
        if self._cur_tok.get_keyword_type() == Keywords.ELSE:
            self._advance(2)  # else {
            self._compile_statements()
            self._advance()   # }
        self._writer.write_label(end_label)

    def _compile_while(self):
        label = self._eat_cur_while_label()
        end_label = 'end_' + label
        self._advance(2)       # while (
        self._writer.write_label(label)
        self._compile_expression()
        self._writer.write_op(UnaryOps.NOT)
        self._writer.write_if_goto(end_label)
        self._advance(2)       # ) {
        self._compile_statements()
        self._advance()
        self._writer.write_goto(label)
        self._writer.write_label(end_label)

    def _compile_return(self):
        self._advance()     # return
        if self._cur_tok.get_symbol_type() != Symbols.SEMI_COLON:
            self._compile_expression()
        else:
            self._writer.write_push(MemSegments.CONSTANT, 0)     # ;
        self._writer.write_return()
        self._advance()

    def _compile_expression(self):
        self._compile_term()
        while self._cur_tok.get_symbol_type() in self._binary_ops_symbols:
            op = self._eat_cur_tok().get_binary_op()
            self._compile_term()
            self._writer.write_op(op)

    def _compile_expression_list(self):
        num_of_expressions = 0
        while self._cur_tok.get_symbol_type() != Symbols.CLOSE_PAREN:
            self._compile_expression()
            num_of_expressions += 1
            if self._cur_tok.get_symbol_type() == Symbols.COMMA:
                self._advance()     # ,
        return num_of_expressions

    def _compile_term(self):
        token_type = self._cur_tok.get_token_type()
        symbol_type = self._cur_tok.get_symbol_type()
        if token_type == TokenTypes.INT_CONST:
            self._writer.write_push(MemSegments.CONSTANT, int(self._eat_cur_tok().get_val()))
        elif token_type == TokenTypes.STRING_CONST:
            self._compile_string_const()
        elif self._cur_tok.get_keyword_type() in self._keyword_constants:
            self._compile_keyword_const()
        elif symbol_type in self._unary_ops_symbols:
            tok = self._eat_cur_tok()
            self._compile_term()
            self._writer.write_op(tok.get_unary_op())
        elif symbol_type == Symbols.OPEN_PAREN:
            self._advance()
            self._compile_expression()
            self._advance()
        else:
            next_symbol_type = self._next_tok.get_symbol_type()
            if next_symbol_type == Symbols.OPEN_SQUARE:
                self._compile_array_exp()
            elif next_symbol_type == Symbols.DOT or next_symbol_type == Symbols.OPEN_PAREN:
                self._compile_subroutine_call()
            else:
                var_entry = self._get_var_entry(self._eat_cur_tok().get_val())
                self._writer.write_push(var_entry.kind.value, var_entry.num)

    def _compile_array_exp(self):
        var_entry = self._get_var_entry(self._eat_cur_tok().get_val())
        self._writer.write_push(var_entry.kind.value, var_entry.num)
        self._advance()
        self._compile_expression()
        self._writer.write_op(BinaryOps.ADD)
        self._writer.write_pop(MemSegments.POINTER, 1)
        self._writer.write_push(MemSegments.THAT, 0)
        self._advance()

    def _compile_string_const(self):
        cur_tok_string = self._eat_cur_tok().get_val()
        str_len = len(cur_tok_string)
        self._writer.write_push(MemSegments.CONSTANT, str(str_len))
        self._writer.write_call('String.new', 1)
        for c in cur_tok_string:
            self._writer.write_push(MemSegments.CONSTANT, ord(c))
            self._writer.write_call('String.appendChar', 2)

    def _compile_keyword_const(self):
        keyword_type = self._eat_cur_tok().get_keyword_type()
        if keyword_type == Keywords.NULL or keyword_type == Keywords.FALSE:
            self._writer.write_push(MemSegments.CONSTANT, 0)
        elif keyword_type == Keywords.TRUE:
            self._writer.write_push(MemSegments.CONSTANT, 1)
            self._writer.write_op(UnaryOps.NEG)
        else:
            self._writer.write_push(MemSegments.POINTER, 0)

    def _compile_subroutine_call(self):
        var_entry = self._get_var_entry(self._cur_tok.get_val())
        if var_entry:
            self._advance(2)
            func_name = self._eat_cur_tok().get_val()
            self._writer.write_push(var_entry.kind.value, var_entry.num)
            self._advance()
            num_of_expressions = self._compile_expression_list()
            self._writer.write_call(var_entry.type + '.' + func_name, num_of_expressions + 1)
            self._advance()
        elif self._next_tok.get_symbol_type() == Symbols.OPEN_PAREN:
            func_name = self._eat_cur_tok().get_val()
            self._advance()
            self._writer.write_push(MemSegments.POINTER, 0)
            num_of_expressions = self._compile_expression_list()
            self._writer.write_call(self._cur_class_name + '.' + func_name, num_of_expressions + 1)
            self._advance()
        else:
            func_name = ''
            for _ in range(3):
                func_name += self._eat_cur_tok().get_val()
            self._advance()
            num_of_expressions = self._compile_expression_list()
            self._writer.write_call(func_name, num_of_expressions)
            self._advance()

    def _eat_cur_if_label(self):
        res = 'if_' + str(self._if_count)
        self._if_count += 1
        return res

    def _eat_cur_while_label(self):
        res = 'while_' + str(self._while_count)
        self._while_count += 1
        return res

    def _get_var_entry(self, name):
        variable = self._cur_subroutine_sym_table.get(name)
        if variable:
            return variable
        variable = self._cur_class_sym_table.get(name)
        if variable:
            return variable
        return None

    def _eat_cur_tok(self):
        if self._cur_tok is None:
            print("error: invalid code")
            remove(self._cur_output_file_name)
            exit(1)
        cur_tok = self._cur_tok
        self._advance()
        return cur_tok

    def _advance(self, steps=1):
        self._tokenizer.advance(steps)
        self._cur_tok = self._tokenizer.get_cur_tok()
        self._next_tok = self._tokenizer.get_next_tok()

