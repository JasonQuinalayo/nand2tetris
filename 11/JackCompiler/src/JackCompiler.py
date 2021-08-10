from CompilationEngine import CompilationEngine
from sys import argv, exit
from os import path, chdir, listdir


def main(args):
    if len(args) != 2:
        print("usage: JackCompiler [filename|directory]")
        exit(1)
    input_file_name = argv[1]
    if not path.exists(input_file_name):
        print("File does not exist")
        exit(1)
    compilation_engine = CompilationEngine()
    if path.isdir(input_file_name):
        chdir(input_file_name)
        for file in listdir():
            if file[-5:] == ".jack":
                compilation_engine.compile(file)
    else:
        compilation_engine.compile(input_file_name)


if __name__ == "__main__":
    main(argv)
