"""
Used to generate error messages for the interpreter

The function also executes the exit function

Fault Stack - A stack of errors that can occur during runtime
"""

def get_characters_at_indices(strings, indices):
    characters = []
    for string, index in zip(strings, indices):
        if index < 0 or index >= len(string):
            characters.append(None)  # Handle invalid index cases
        else:
            characters.append(string[index])
    return characters

class errgen:
    def __init__(self, line, filepath, value, error, err_type):
        # A list containing the lines of where the error(s) happened at
        self.line: list = line

        # Value of the line the error happened at
        self.value: list = value

        # Absolute path to the files the errors happened at
        self.file: list = filepath

        # Used to specify the character the error happened at in an index in the "value" list
        self.error: list = error

        # Specify the type of error that happened
        self.err_type: list = err_type

        # All indexes of every variable is the same, and the
        # same index in every variable is used to reference the same error

    def gen_err_msg(self):
        for i in range(len(self.line)):
            print("\033[1;91m", end="")
            print(f'File "{self.file[i]}", line {self.line[i]}')
            print(f'    {self.value[i]}')
            caret_position = ' ' * (self.error[i] + 4) + '^'
            print(caret_position)
            print(f'{get_characters_at_indices(value, error)[i]}: Error occurred here')
            print("\033[0m", end="")

        exit()

# Initialize the errgen class with sample data
line = [14, 17]
filepath = ['/Users/yangmingde/PycharmProjects/Vastlang2/errh/faultstack.py', '/Users/yangmingde/PycharmProjects/Vastlang2/errh/faultstack.py']
value = ['print("d")z', 'print("dadsa")s']
error = [10, 14]
err_type = ['SyntaxError', 'SyntaxError']

# Create an instance of the errgen class
error_generator = errgen(line, filepath, value, error, err_type)

# Generate the error message
error_generator.gen_err_msg()