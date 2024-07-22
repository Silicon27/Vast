from xvast_lexer import convert_to_token
keywords = ["runfile", "export"]
tokens = ["RUNFILE", "EXPORT"]

debug_mode: bool = True

def get_file(file):
    global interpret
    interpret = convert_to_token(keywords, file, tokens)
    global tokenized_output
    tokenized_output = list(interpret.tokenize())
    if debug_mode:
        print("Tokenized output: ", tokenized_output)
    return tokenized_output

class xvast:
    def __init__(self, tokenized_output):
        self.tokenized_output = tokenized_output
        self.position = 0
    def xvast_run(self):
        while self.position < len(self.tokenized_output):
            token = self.tokenized_output[self.position]

            # Handle different statements
            if token == "RUNFILE":
                self.position += 1
                self._handle_runfile()

            else:
                self.position += 1

    def _handle_runfile(self):

        if ":" in self.tokenized_output[self.position]:
            self.tokenized_output[self.position] = self.tokenized_output[self.position].replace(" ", "")
            self.tokenized_output[self.position] = self.tokenized_output[self.position].replace(":", "")
            if debug_mode:
                print(self.tokenized_output[self.position])

            return self.tokenized_output[self.position]
        else:
            raise SyntaxError("Invalid Syntax")




get_file("config.xvast")
xvast = xvast(tokenized_output)
xvast.xvast_run()