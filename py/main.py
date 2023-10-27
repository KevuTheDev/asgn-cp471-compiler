from compiler import Compiler
from utils import *

def main():
    # get filename from user
    filename = input("Enter filename: ")
    compiler = Compiler()
    compiler.compile(filename)








if __name__ == "__main__":
    main()
