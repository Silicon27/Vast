import os

from tokenize_lexer import convert_to_token


# Read the config file to get the file name
with open("config.xvast") as config_file:
    for line in config_file:
        line = line.replace(" ", "")
        if "runfile:" in line:
            runfile = line.split(":")
            file = runfile[1].strip()

# Keywords and their corresponding tokens
keywords = ["print", "(", ")", '"', "'", "{", "}", 'create', 'expand']
tokens = ["PRINT", "(", ")", '"', "'", "{", "}", 'CREATE', 'EXPAND']

# Convert the source code to tokens
interpret = convert_to_token(keywords, file, tokens)
tokenized_output = list(interpret.tokenize())
print(tokenized_output)

# Global dictionary to store functions
global functions
functions = {}

# Class to interpret the tokenized output
class Interpret:
    def __init__(self, tokenized_output):
        self.tokenized_output = tokenized_output
        self.position = 0  # Index of the current position in the tokenized output

    def interpret(self):
        while self.position < len(self.tokenized_output):
            token = self.tokenized_output[self.position]

            # Handle different statements
            if token == "PRINT":
                self.position += 1
                self._handle_print()
            elif token == "CREATE":
                self.position += 1
                self._handle_create()
            elif token == "EXPAND":
                self.position += 1
                self._handle_expand()
            else:
                self.position += 1

    def _handle_print(self) -> None:
        """
        _handle_print

        self : none

        Used to handle the print statements
        """
        if self.tokenized_output[self.position] == '(':
            self.position += 1  # Move past '('
            if self.tokenized_output[self.position] == '"' or self.tokenized_output[self.position] == "'":
                quote_type = self.tokenized_output[self.position]
                self.position += 1  # Move past '"' or "'"
                message = ""
                while self.position < len(self.tokenized_output) and self.tokenized_output[self.position] != quote_type:
                    message += self.tokenized_output[self.position]
                    self.position += 1
                self.position += 1  # Move past closing '"' or "'"
                if self.position < len(self.tokenized_output) and self.tokenized_output[self.position] == ')':
                    self.position += 1  # Move past ')'
                    print(message)
                else:
                    raise SyntaxError("Expected ')'")
            else:
                raise SyntaxError("Expected '\"' or \"'\"")
        else:
            raise SyntaxError("Expected '('")

    def _handle_create(self) -> None:
        """
        _handle_create
        self : none

        Used to handle the create statement for functions
        """
        function_name = ""
        function_arguments = []

        while self.position < len(self.tokenized_output) and self.tokenized_output[self.position] != '(':
            function_name += self.tokenized_output[self.position]
            self.position += 1
        function_name = function_name.strip()

        if self.position < len(self.tokenized_output) and self.tokenized_output[self.position] == '(':
            self.position += 1  # Move past '('
        else:
            raise SyntaxError("Expected '(' after function name")

        current_arg = ""
        while self.position < len(self.tokenized_output) and self.tokenized_output[self.position] != ')':
            if self.tokenized_output[self.position] == ',':
                function_arguments.append(current_arg.strip())
                current_arg = ""
            else:
                current_arg += self.tokenized_output[self.position]
            self.position += 1
        function_arguments.append(current_arg.strip())  # add the last argument


        if self.position < len(self.tokenized_output) and self.tokenized_output[self.position] == ')':
            self.position += 1
        else:
            raise SyntaxError("Expected ')' after function arguments")

        function_arguments = [arg.strip() for arg in ','.join(function_arguments).replace(" ", "").split(',')]
        print(function_arguments)

        # update the global functions dictionary
        functions[function_name] = function_arguments
        print(f"Function created: {function_name} with arguments {function_arguments}")
        #IMPORTANT: remember to update the create statement so that it stores the code inside the {} brackets too
        if self.tokenized_output[self.position] == '{':
            self.position += 1
            while self.tokenized_output[self.position] != "{" and self.position < len(self.tokenized_output) and self.tokenized_output[self.position] != "}":

                function_data: list = self.tokenized_output[self.position]
                print("function: " + function_data)
                self.position += 1
    def _handle_skip(self) -> None:
        """
        Handles skip

        _handle_skip -> None

        Used to skip the function it is passed into
        :return:
        """
        ...

    def _handle_expand(self) -> None:
        """
        Handles expand

        _handle_expand -> None

        used to expand and use another .vast file form this vast file (commonly known as libraries)
        :return:
        """
        if self.tokenized_output[self.position] == "example":
            print("expanded an example module")
        else:
            if os.path.exists(f".vastlibs/lib/vast/local-packages/{self.tokenized_output[self.position]}"):
                with open(f".vastlibs/lib/vast/local-packages/{self.tokenized_output[self.position]}", "r") as file:
                    pass
            else:
                raise FileNotFoundError(
                    f"Library {self.tokenized_output[self.position]} does not exist within this directory\n",
                    f"--> try to run \"vivt mkienv\"")



# Create an instance of the Interpret class and run the interpreter
interpreter = Interpret(tokenized_output)
interpreter.interpret()
