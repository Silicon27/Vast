"""
    Main Vast interpreter file

    WARNING: DO NOT DELETE THIS FILE, DELETION OR ANY MODIFICATION OF THIS FILE MAY RESULT IN UNEXPECTED ERRORS
"""

import re

class convert_to_token:
    def __init__(self, keywords, file, tokens, SYMBOL):
        self.keywords = keywords
        self.file = file
        self.tokens = tokens
        self.tokens = tokens
        self.symbol = SYMBOL

    def tokenize(self):
        tokenized_output = []
        tokenized_dict = []
        identifiers = []

        with open(self.file, "r") as file:
            for line in file:
                # split the details on the line
                temp = re.split(rf"({'|'.join(map(re.escape, self.keywords))})", line)
                detail = [ele for ele in temp if ele.strip()]

                # tokenize the split list
                for ele in detail:
                    if ele in self.keywords:
                        tokenized_output.append(self.tokens[self.keywords.index(ele)])
                    else:
                        tokenized_output.append(ele.strip())

                    tokenized_dict.append(
                        {"value": ele,
                         "type": "SYMBOL" if ele in self.symbol else "KEYWORD" if ele in self.keywords else "IDENTIFIER"}
                    )

        return tokenized_output, tokenized_dict
