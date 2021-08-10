from enum import Enum


class Keywords(Enum):
    CLASS = 0
    CONSTRUCTOR = 1
    FUNCTION = 2
    METHOD = 3
    FIELD = 4
    STATIC = 5
    VAR = 6
    INT = 7
    CHAR = 8
    BOOLEAN = 9
    VOID = 10
    TRUE = 11
    FALSE = 12
    NULL = 13
    THIS = 14
    LET = 15
    DO = 16
    IF = 17
    ELSE = 18
    WHILE = 19
    RETURN = 20


keywords_string_set = {
    'class',
    'constructor',
    'function',
    'method',
    'field',
    'static',
    'var',
    'int',
    'char',
    'boolean',
    'void',
    'true',
    'false',
    'null',
    'this',
    'let',
    'do',
    'if',
    'else',
    'while',
    'return',
}


class Symbols(Enum):
    OPEN_CURLY = 0
    CLOSE_CURLY = 1
    OPEN_PAREN = 2
    CLOSE_PAREN = 3
    OPEN_SQUARE = 4
    CLOSE_SQUARE = 5
    DOT = 6
    COMMA = 7
    SEMI_COLON = 8
    PLUS = 9
    MINUS = 10
    ASTERISK = 11
    FORWARD_SLASH = 12
    AMPERSAND = 13
    VERTICAL_BAR = 14
    LESS_THAN = 15
    GREATER_THAN = 16
    EQUAL = 17
    TILDE = 18


symbols_string_set = {
    '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~',
}


class TokenTypes(Enum):
    KEYWORD = 0
    SYMBOL = 1
    INT_CONST = 2
    STRING_CONST = 3
    IDENTIFIER = 4
