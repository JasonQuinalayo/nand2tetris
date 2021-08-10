from enum import Enum
from Constants import MemSegments


class SymbolTable:
    def __init__(self):
        self._vars = {}
        self.count = {
            Variable.Kind.FIELD: 0,
            Variable.Kind.STATIC: 0,
            Variable.Kind.LOCAL: 0,
            Variable.Kind.ARGUMENT: 0
        }

    def add(self, name, type_, kind):
        self._vars[name] = Variable(name, type_, kind, self.count[kind])
        self.count[kind] += 1

    def get(self, name):
        if name in self._vars:
            return self._vars[name]
        return None

    def print(self):
        for k in self._vars:
            print(k)


class Variable:
    class Kind(Enum):
        FIELD = MemSegments.THIS
        STATIC = MemSegments.STATIC
        LOCAL = MemSegments.LOCAL
        ARGUMENT = MemSegments.ARGUMENT

    def __init__(self, name, type_, kind, num):
        self.name = name
        self.type = type_
        self.kind = kind
        self.num = num
