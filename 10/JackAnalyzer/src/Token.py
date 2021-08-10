from Constants import TokenTypes, Keywords, Symbols


class Token:
    def __init__(self, tok_val):
        self._tok_val = tok_val

    def get_token_type_string(self):
        raise NotImplementedError()

    def get_token_type(self):
        raise NotImplementedError()

    def get_keyword_type(self):
        return None

    def get_symbol_type(self):
        return None

    def get_val(self):
        return self._tok_val


class Keyword(Token):
    _keywords_map = {
        'class': Keywords.CLASS,
        'constructor': Keywords.CONSTRUCTOR,
        'function': Keywords.FUNCTION,
        'method': Keywords.METHOD,
        'field': Keywords.FIELD,
        'static': Keywords.STATIC,
        'var': Keywords.VAR,
        'int': Keywords.INT,
        'char': Keywords.CHAR,
        'boolean': Keywords.BOOLEAN,
        'void': Keywords.VOID,
        'true': Keywords.TRUE,
        'false': Keywords.FALSE,
        'null': Keywords.NULL,
        'this': Keywords.THIS,
        'let': Keywords.LET,
        'do': Keywords.DO,
        'if': Keywords.IF,
        'else': Keywords.ELSE,
        'while': Keywords.WHILE,
        'return': Keywords.RETURN,
    }

    def __init__(self, tok_val):
        super().__init__(tok_val)

    def get_token_type_string(self):
        return 'keyword'

    def get_token_type(self):
        return TokenTypes.KEYWORD

    def get_keyword_type(self):
        return self._keywords_map[self._tok_val]


class Symbol(Token):
    _symbols_map = {
        '{': Symbols.OPEN_CURLY,
        '}': Symbols.CLOSE_CURLY,
        '(': Symbols.OPEN_PAREN,
        ')': Symbols.CLOSE_PAREN,
        '[': Symbols.OPEN_SQUARE,
        ']': Symbols.CLOSE_SQUARE,
        '.': Symbols.DOT,
        ',': Symbols.COMMA,
        ';': Symbols.SEMI_COLON,
        '+': Symbols.PLUS,
        '-': Symbols.MINUS,
        '*': Symbols.ASTERISK,
        '/': Symbols.FORWARD_SLASH,
        '&': Symbols.AMPERSAND,
        '|': Symbols.VERTICAL_BAR,
        '<': Symbols.LESS_THAN,
        '>': Symbols.GREATER_THAN,
        '=': Symbols.EQUAL,
        '~': Symbols.TILDE,
    }

    def __init__(self, tok_val):
        super().__init__(tok_val)

    def get_token_type_string(self):
        return 'symbol'

    def get_token_type(self):
        return TokenTypes.SYMBOL

    def get_symbol_type(self):
        return self._symbols_map[self._tok_val]


class Identifier(Token):
    def __init__(self, tok_val):
        super().__init__(tok_val)

    def get_token_type_string(self):
        return 'identifier'

    def get_token_type(self):
        return TokenTypes.IDENTIFIER


class StringConst(Token):
    def __init__(self, tok_val):
        super().__init__(tok_val)

    def get_token_type_string(self):
        return 'stringConstant'

    def get_token_type(self):
        return TokenTypes.STRING_CONST


class IntConst(Token):
    def __init__(self, tok_val):
        super().__init__(tok_val)

    def get_token_type_string(self):
        return 'integerConstant'

    def get_token_type(self):
        return TokenTypes.INT_CONST
