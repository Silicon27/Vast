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

def generate_error_message(line, filepath, value, error, err_type):
    for i in range(len(line)):
        print("\033[1;91m", end="")
        print(f'File "{filepath[i]}", line {line[i]}')
        print(f'    {value[i]}')
        caret_position = ' ' * (error[i] + 4) + '^'
        print(caret_position)
        print(f'{get_characters_at_indices(value, error)[i]}: Error occurred here')
        print("\033[0m", end="")

    exit()

# Sample data
line = [14, 17]
filepath = ['/Users/yangmingde/PycharmProjects/Vastlang2/errh/faultstack.py', '/Users/yangmingde/PycharmProjects/Vastlang2/errh/faultstack.py']
value = ['print("d")z', 'print("dadsa")s']
error = [10, 14]
err_type = ['SyntaxError', 'SyntaxError']

# Generate the error message
generate_error_message(line, filepath, value, error, err_type)