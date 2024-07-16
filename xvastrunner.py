from tokenize_lexer import convert_to_token
keywords = ["runfile", "export"]
tokens = ["RUNFILE", "EXPORT"]

def get_file(file):
    global interpret
    interpret = convert_to_token(keywords, file, tokens)
    global tokenized_output
    tokenized_output = list(interpret.tokenize())
    print(tokenized_output)
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
        print(self.tokenized_output[self.position])
        if ":" in self.tokenized_output[self.position]:
            self.tokenized_output[self.position] = self.tokenized_output[self.position].replace(" ", "")
            self.tokenized_output[self.position] = self.tokenized_output[self.position].replace(":", "")
            print(self.tokenized_output[self.position])




get_file("config.xvast")
xvast = xvast(tokenized_output)
xvast.xvast_run()