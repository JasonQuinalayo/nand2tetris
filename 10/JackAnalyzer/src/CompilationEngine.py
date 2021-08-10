from Tokenizer import Tokenizer
from Constants import Symbols, Keywords, TokenTypes
from os import remove
from sys import exit


class CompilationEngine:
    _class_subroutine_dec_keywords = {Keywords.METHOD, Keywords.CONSTRUCTOR, Keywords.FUNCTION}
    _class_var_dec_keywords = {Keywords.FIELD, Keywords.STATIC}
    _ops = {
        Symbols.PLUS, Symbols.MINUS, Symbols.ASTERISK, Symbols.FORWARD_SLASH,
        Symbols.AMPERSAND, Symbols.VERTICAL_BAR, Symbols.LESS_THAN, Symbols.GREATER_THAN, Symbols.EQUAL
    }
    _unary_ops = {
        Symbols.TILDE, Symbols.MINUS
    }
    _keyword_constants = {
        Keywords.THIS, Keywords.TRUE, Keywords.FALSE, Keywords.NULL
    }

    def __init__(self):
        self._tokenizer = None
        self._writer = CompilationEngine._Writer()
        self._cur_tok = None
        self._output_file_name = None

    def compile(self, input_file_name):
        self._tokenizer = Tokenizer(input_file_name)
        self._cur_tok = self._tokenizer.get_cur_tok()
        if input_file_name[-5:] == ".jack":
            input_file_name = input_file_name[:-5]
        output_file_name = input_file_name + ".xml"
        self._output_file_name = output_file_name
        with open(output_file_name, 'w') as output_stream:
            self._writer.set_output_file(output_stream)
            self._compile_class()

    def _compile_class(self):
        self._writer.write_begin('class')
        self._writer.write('\n')
        self._write_tokens(3)   # class varName {
        self._compile_class_var_dec()
        self._compile_class_subroutine_dec()
        self._write_cur_token()      # }
        self._writer.write_end('class')

    def _compile_class_var_dec(self):
        while self._cur_tok.get_keyword_type() not in self._class_subroutine_dec_keywords:
            self._writer.write_begin('classVarDec')
            self._writer.write('\n')
            self._compile_vars()
            self._writer.write_end('classVarDec')

    def _compile_class_subroutine_dec(self):
        while self._cur_tok.get_symbol_type() != Symbols.CLOSE_CURLY:
            self._writer.write_begin('subroutineDec')
            self._writer.write('\n')
            self._write_tokens(4)       # function|constructor|method  returnType  subroutineName  (
            self._compile_param_list()
            self._write_cur_token()     # )
            self._compile_subroutine_body()
            self._writer.write_end('subroutineDec')

    def _compile_param_list(self):
        self._writer.write_begin('parameterList')
        self._writer.write('\n')
        while self._cur_tok.get_symbol_type() != Symbols.CLOSE_PAREN:
            self._write_cur_token()     # args
        self._writer.write_end('parameterList')

    def _compile_subroutine_body(self):
        self._writer.write_begin('subroutineBody')
        self._writer.write('\n')
        self._write_cur_token()     # {
        self._compile_subroutine_var_dec()
        self._compile_statements()
        self._write_cur_token()     # }
        self._writer.write_end('subroutineBody')

    def _compile_subroutine_var_dec(self):
        while self._cur_tok.get_keyword_type() == Keywords.VAR:
            self._writer.write_begin('varDec')
            self._writer.write('\n')
            self._compile_vars()
            self._writer.write_end('varDec')

    def _compile_statements(self):
        self._writer.write_begin('statements')
        self._writer.write('\n')
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
        self._writer.write_end('statements')

    def _compile_let(self):
        self._writer.write_begin('letStatement')
        self._writer.write('\n')
        self._write_tokens(2)  # let varName
        if self._cur_tok.get_symbol_type() == Symbols.OPEN_SQUARE:
            self._write_cur_token()  # [
            self._compile_expression()
            self._write_cur_token()  # ]
        self._write_cur_token()      # =
        self._compile_expression()
        self._write_cur_token()      # ;
        self._writer.write_end('letStatement')

    def _compile_do(self):
        self._writer.write_begin('doStatement')
        self._writer.write('\n')
        self._write_cur_token()     # do
        while self._cur_tok.get_symbol_type() != Symbols.OPEN_PAREN:
            self._write_cur_token()
        self._write_cur_token()     # (
        self._compile_expression_list()
        self._write_tokens(2)     # ) ;
        self._writer.write_end('doStatement')

    def _compile_if(self):
        self._writer.write_begin('ifStatement')
        self._writer.write('\n')
        self._write_tokens(2)     # if (
        self._compile_expression()
        self._write_tokens(2)     # ) {
        self._compile_statements()
        self._write_cur_token()     # }
        if self._cur_tok.get_keyword_type() == Keywords.ELSE:
            self._write_tokens(2)  # else {
            self._compile_statements()
            self._write_cur_token()  # }
        self._writer.write_end('ifStatement')

    def _compile_while(self):
        self._writer.write_begin('whileStatement')
        self._writer.write('\n')
        self._write_tokens(2)   # while (
        self._compile_expression()
        self._write_tokens(2)   # ) {
        self._compile_statements()
        self._write_cur_token() # }
        self._writer.write_end('whileStatement')

    def _compile_return(self):
        self._writer.write_begin('returnStatement')
        self._writer.write('\n')
        self._write_cur_token()     # return
        if self._cur_tok.get_symbol_type() != Symbols.SEMI_COLON:
            self._compile_expression()
        self._write_cur_token()     # ;
        self._writer.write_end('returnStatement')

    def _compile_expression(self):
        self._writer.write_begin('expression')
        self._writer.write('\n')
        self._compile_term()
        while self._cur_tok.get_symbol_type() in self._ops:
            self._write_cur_token()
            self._compile_term()
        self._writer.write_end('expression')

    def _compile_expression_list(self):
        self._writer.write_begin('expressionList')
        self._writer.write('\n')
        while self._cur_tok.get_symbol_type() != Symbols.CLOSE_PAREN:
            self._compile_expression()
            if self._cur_tok.get_symbol_type() == Symbols.COMMA:
                self._write_cur_token()     # ,
        self._writer.write_end('expressionList')

    def _compile_term(self):
        self._writer.write_begin('term')
        self._writer.write('\n')
        token_type = self._cur_tok.get_token_type()
        symbol_type = self._cur_tok.get_symbol_type()
        if token_type == TokenTypes.INT_CONST or token_type == TokenTypes.STRING_CONST or \
                self._cur_tok.get_keyword_type() in self._keyword_constants:
            self._write_cur_token()
        elif symbol_type in self._unary_ops:
            self._write_cur_token()
            self._compile_term()
        elif symbol_type == Symbols.OPEN_PAREN:
            self._write_cur_token()
            self._compile_expression()
            self._write_cur_token()
        else:
            self._write_cur_token()
            symbol_type = self._cur_tok.get_symbol_type()
            if symbol_type == Symbols.OPEN_SQUARE:
                self._write_cur_token()     # [
                self._compile_expression()
                self._write_cur_token()     # ]
            elif symbol_type == Symbols.DOT:
                self._write_tokens(3)     # . subroutine (
                self._compile_expression_list()
                self._write_cur_token()   # )
            elif symbol_type == Symbols.OPEN_PAREN:
                self._write_cur_token()   # (
                self._compile_expression_list()
                self._write_cur_token()   # )
        self._writer.write_end('term')

    def _compile_vars(self):
        for _ in range(2):
            self._write_cur_token()     # static|field|var  type
        while self._cur_tok.get_symbol_type() != Symbols.SEMI_COLON:
            self._write_cur_token()     # varName(s)
        self._write_cur_token()

    def _write_tokens(self, num):
        for _ in range(num):
            self._write_cur_token()

    def _write_cur_token(self):
        if self._cur_tok is None:
            self._error()
        self._writer.write_token(self._cur_tok.get_token_type_string(), self._cur_tok.get_val())
        self._advance()

    def _advance(self):
        self._tokenizer.advance()
        self._cur_tok = self._tokenizer.get_cur_tok()

    def _error(self):
        print("error: invalid code")
        remove(self._output_file_name)
        exit(1)

    class _Writer:
        _xml_special_symbols_map = {'&': '&amp;', '>': '&gt;', '<': '&lt;'}

        def __init__(self):
            self._output_file_stream = None

        def set_output_file(self, output):
            self._output_file_stream = output

        def write_token(self, token_type_string, token_value):
            self.write_begin(token_type_string)
            self.write(' ')
            if token_value in self._xml_special_symbols_map:
                self.write(self._xml_special_symbols_map[token_value])
            else:
                self.write(token_value)
            self.write(' ')
            self.write_end(token_type_string)

        def write_begin(self, value):
            self.write('<' + value + '>')

        def write_end(self, value):
            self.write('</' + value + '>\n')

        def write(self, value):
            self._output_file_stream.write(value)
