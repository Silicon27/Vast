import os
from tokenize_lexer import convert_to_token

# Set this too false to remove debug view
debug_mode: bool = False

# Read the config file to get the file name
with open("config.xvast") as config_file:
    for line in config_file:
        line = line.replace(" ", "")
        if "runfile:" in line:
            runfile = line.split(":")
            file = runfile[1].strip()


# Keywords and their corresponding tokens
keywords = ["print", "(", ")", '"', "'", "{", "}", 'create', 'expand', 'export']
tokens = ["PRINT", "(", ")", '"', "'", "{", "}", 'CREATE', 'EXPAND', 'EXPORT']
SYMBOL = ["(", ")", "{", "}", "[", "]", ".", ","]

# Convert the source code to tokens
interpret = convert_to_token(keywords, file, tokens, SYMBOL)
tokenized_output, tokenized_dict = list(interpret.tokenize())

if debug_mode:
    print(tokenized_output)
    print(tokenized_dict)

# Global dictionary to store functions
functions = {}
expanded_items: list = []


# Class to interpret the tokenized output
class Interpret:
    def __init__(self, tokenized_outputc):
        self.tokenized_output = tokenized_outputc
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
            elif token == "EXPORT":
                self.position += 1
                self._handle_export()
            else:
                self.position += 1

    def _handle_print(self) -> None:
        """
        _handle_print

        self : none

        Used to handle the print statements
        """

        if debug_mode:
            print("\033[0;36m'print' statement function invoked\033[0m")

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
                    if debug_mode:
                        print("\033[0;32mNormal Output:\033[0m " + message)
                    else:
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

        if debug_mode:
            print("\033[0;36m'create' statement function invoked\033[0m")

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
        if debug_mode:
            print(function_arguments)

        # update the global functions dictionary
        functions[function_name] = function_arguments
        if debug_mode:
            print(f"Function created: {function_name} with arguments {function_arguments}")
        # IMPORTANT: remember to update the "create" statement so that it stores the code inside the {} brackets too
        if self.tokenized_output[self.position] == '{':
            self.position += 1
            while (self.tokenized_output[self.position] != "{"
                   and self.position < len(self.tokenized_output)
                   and self.tokenized_output[self.position] != "}"):

                function_data: list = self.tokenized_output[self.position]
                if debug_mode:
                    print("function: " + str(function_data))
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

        Used to expand and use another .vast file form this vast file (commonly known as packages)
        packages are currently to be made with python, with the .py extension
        :return:

        packages: str = package that is being expanded

        expanded_items: list = packages to be expanded
        """

        if debug_mode:
            print("\033[0;36m'expand' statement function invoked\033[0m")
        # check for if it's an example module
        if self.tokenized_output[self.position] == "example":
            if debug_mode:
                print("successfully expanded an example package")
            self.position += 1

        elif self.position < len(self.tokenized_output) and self.tokenized_output[self.position] != "example":
            package = str(tokenized_output[self.position].split(" "))
            if debug_mode:
                print("package: " + package)

            expanded_items.append(tokenized_output[self.position])
            if debug_mode:
                print("\033[92m Expanded items: " + str(expanded_items))
            times_through_packages_loop = 0
            for packages in expanded_items:
                times_through_packages_loop += 1
                if os.path.exists(f".vastlibs/lib/vast/local-packages/{packages}"):
                    with open(f'.vastlibs/lib/vast/local-packages/{packages}/__package__.xvast', 'r+') as package_file:
                        if debug_mode:
                            # if times_through_packages_loop is bigger than 1, then that means there's a repeated import
                            print("\033[92m__package__ file contents of "
                                  + f"'{packages}' package: \033[94m"
                                  + package_file.read()
                                  + " \033[0m")
                            print(f"_______________________________ "
                                  f"{times_through_packages_loop} "
                                  f"Expand Chunk(s) "
                                  f"_______________________________")
                            package_file.seek(0)
                        if times_through_packages_loop > 1:
                            raise ImportError("Package " + str(packages) + " is already expanded")

                        # continue on the logics of the .xvast file
                        package_file_list = package_file.read().split("\n")
                        if debug_mode:
                            print(package_file_list)

                        # Nested for loop
                        for package_file_lines in package_file_list:
                            if package_file_lines.startswith("!#"):
                                continue
                            elif package_file_lines.startswith("export"):
                                package_file_lines = package_file_lines.split("export")
                                package_file_lines = list(filter(None, package_file_lines))

                                if debug_mode:
                                    print("line: \033[38;5;206;48;5;57m"
                                          + str(package_file_lines).replace(" ", "")
                                          + "\033[0m")
                                    print(f"\033[1;33mImported {package_file_lines[0]}\033[0m")
                                    # Get the type of the package_file_lines variable (Should be of 'list' type)
                                    print("Extracted package file variable type: " + str(type(package_file_lines)))
                else:
                    if debug_mode:
                        print(f"\033[0;31mPackage '{packages}' does not exist\033[0m")
                    raise ImportError("Package '" + packages + "' does not exist")

                # Remove item from the list to avoid repeated imports
                expanded_items.remove(packages)

            if debug_mode:
                print("expanded packages:" + str(expanded_items))
            self.position += 1

        else:
            raise FileNotFoundError(f"Library {self.tokenized_output[self.position]}"
                                    f" does not exist within this directory\n",
                                    f"--> try to run \"vivt mkienv\"")
    if debug_mode:
        print(f"_______________________________"
              f"\033[1;31m End \033[0m"
              f"Expand Chunk(s) "
              f"_______________________________")


    def _handle_export(self) -> None:
        """
        Handles export

        _handle_export -> None

        Used to export python functions from a Python-based module
        :return:
        """
        pass


# Create an instance of the Interpret class and run the interpreter
interpreter = Interpret(tokenized_output)
interpreter.interpret()
