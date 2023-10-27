from math import e
from utils import check_file_extension

class Compiler:
    EXTENSION = ".cp"

    def __init__(self):
        self.name = "Hello World!"

    
    def __str__(self):
        return self.name
    
    def compile(self, filename):
        if not check_file_extension(filename, self.EXTENSION):
            print("Error: Invalid file extension")
            return False
        

        print("Compiling " + filename + "...")
        return True
