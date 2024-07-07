from tokenize import convert_to_token

with open("config.xvast") as config_file:
    for line in config_file:
        line = line.replace(" ", "")
        if "runfile:" in line:
            runfile = line.split(":")
            file = runfile[1].strip()

keywords = ["print", "(", ")", '"', "'"]
tokens = ["PRINT", "(", ")", '"', "'"]


interpret = convert_to_token(keywords, file, tokens)
tokenized_output = list(interpret.tokenize())

# create class for interpretation of tokenized output
class Interpret:
    def __init__(self, tokenized_output):
        self.tokenized_output = tokenized_output

        # index of which the pointer is on
        self.position = 0

    def interpret(self):
        while self.position < len(self.tokenized_output):
            token = self.tokenized_output[self.position]

            # All the statements of Vast are created and listed here
            if token == "PRINT":
                self.position += 1 # add one to position in index
                self._handle_print()
            else:
                self.position += 1

    def _handle_print(self):
        if self.tokenized_output[self.position] == '(':
            self.position += 1  # Move past '('
            if self.tokenized_output[self.position] == '"':
                self.position += 1  # Move past '"'
                message = ""
                while self.tokenized_output[self.position] != '"':
                    message += self.tokenized_output[self.position]
                    self.position += 1
                self.position += 1  # Move past closing '"'
                if self.tokenized_output[self.position] == ')':
                    self.position += 1  # Move past ')'
                    print(message)
                else:
                    raise SyntaxError("Expected ')'")
            else:
                raise SyntaxError("Expected '\"'")
        else:
            raise SyntaxError("Expected '('")


interpreter = Interpret(tokenized_output)
interpreter.interpret()