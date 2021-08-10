from Constants import keywords_string_set, symbols_string_set
from Token import Keyword, Symbol, Identifier, IntConst, StringConst


class Tokenizer:
    def __init__(self, input_file_name):
        self._tokens = []
        self._source_it = -1
        self._cur = None
        self._next = None
        self._source = None
        self._source_len = 0
        self._tokenize(input_file_name)
        self._token_it = 0

    def advance(self, steps=1):
        self._token_it += steps

    def get_cur_tok(self):
        return self._tokens[self._token_it] if self._token_it < len(self._tokens) else None

    def get_next_tok(self):
        return self._tokens[self._token_it + 1] if self._token_it + 1 < len(self._tokens) else None

    def has_more_tokens(self):
        return self._token_it != len(self._tokens)

    def _tokenize(self, input_file_name):
        with open(input_file_name) as f:
            self._source = f.read()
            self._source_len = len(self._source)
        self._step()
        while self._cur is not None:
            if self._cur.isspace():
                self._step()
            elif self._cur == '/' and self._next == '/':
                while self._cur is not None and self._cur != '\n':
                    self._step()
            elif self._cur == '/' and self._next == '*':
                while self._next is not None and not (self._cur == '*' and self._next == '/'):
                    self._step()
                self._step(2)
            elif self._cur == '"':
                self._add_string()
            elif self._cur.isdigit():
                self._add_int()
            elif self._cur == '_' or self._cur.isalpha():
                self._add_keyword_or_identifier()
            else:
                self._add_symbol()
        return self._tokens[:]

    def _step(self, steps=1):
        self._source_it += steps
        self._cur = self._source[self._source_it] if self._source_it < self._source_len else None
        self._next = self._source[self._source_it + 1] if self._source_it + 1 < self._source_len else None

    def _add_string(self):
        self._step()
        start = self._source_it
        while self._cur is not None and self._cur != '"':
            self._step()
        self._tokens.append(StringConst(self._source[start: self._source_it]))
        self._step()

    def _add_int(self):
        start = self._source_it
        while self._cur is not None and self._cur.isdigit():
            self._step()
        self._tokens.append(IntConst(self._source[start: self._source_it]))

    def _add_keyword_or_identifier(self):
        start = self._source_it
        while self._cur is not None and not self._cur.isspace() and self._cur not in symbols_string_set:
            self._step()
        value = self._source[start: self._source_it]
        if value in keywords_string_set:
            self._tokens.append(Keyword(value))
        else:
            self._tokens.append(Identifier(value))

    def _add_symbol(self):
        self._tokens.append(Symbol(self._cur))
        self._step()
