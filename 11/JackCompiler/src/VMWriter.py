from Constants import BinaryOps, UnaryOps


class VMWriter:
    _ops_map = {
        BinaryOps.ADD: 'add',
        BinaryOps.SUB: 'sub',
        BinaryOps.MULT: 'call Math.multiply 2',
        BinaryOps.DIV: 'call Math.divide 2',
        BinaryOps.AND: 'and',
        BinaryOps.OR: 'or',
        BinaryOps.LT: 'lt',
        BinaryOps.GT: 'gt',
        BinaryOps.EQ: 'eq',
        UnaryOps.NOT: 'not',
        UnaryOps.NEG: 'neg',
    }

    def __init__(self):
        self._output_file_stream = None

    def set_output_file(self, output):
        self._output_file_stream = output

    def write_func_name(self, name, num_of_vars):
        self._output_file_stream.write('function ' + name + ' ' + str(num_of_vars) + '\n')

    def write_push(self, mem_segment, offset):
        self._output_file_stream.write('push' + ' ' + mem_segment.value + ' ' + str(offset) + '\n')

    def write_op(self, op):
        self._output_file_stream.write(self._ops_map[op] + '\n')

    def write_pop(self, mem_segment, offset):
        self._output_file_stream.write('pop' + ' ' + mem_segment.value + ' ' + str(offset) + '\n')

    def write_label(self, label):
        self._output_file_stream.write('label ' + label + '\n')

    def write_if_goto(self, label):
        self._output_file_stream.write('if-goto ' + label + '\n')

    def write_goto(self, label):
        self._output_file_stream.write('goto ' + label + '\n')

    def write_return(self):
        self._output_file_stream.write('return\n')

    def write_call(self, func_name, num_of_vars):
        self._output_file_stream.write('call ' + func_name + ' ' + str(num_of_vars) + '\n')
