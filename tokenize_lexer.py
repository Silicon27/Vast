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
        self.symbol = SYMBOL

    def tokenize(self):
        tokenized_output = []
        tokenized_dict = []
        tokenized_output_w_spaces = []
        toeknized_output_w_everything = []

        with open(self.file, "r") as file:
            for position, line in enumerate(file, start=1):

                # Step 1: Escape each keyword
                escaped_keywords = list(map(re.escape, self.keywords))

                # Step 2: Join the escaped keywords with the pipe '|' symbol
                joined_keywords = '|'.join(escaped_keywords)

                # Step 3: Construct the regular expression pattern
                # Match exact keywords and split on non-alphanumeric characters
                pattern = rf"\b({joined_keywords})\b|([^\w\s])"

                # Step 4: Use re.split with the constructed pattern
                temp = re.split(pattern, line)

                filtered_list = [x for x in temp if x and x.strip()]

                # Append each non-empty item to the tokenized_output_w_spaces
                tokenized_output_w_spaces.extend(filtered_list)
                # Remove trailing, leading whitespaces, and empty strings
                detail = [ele for ele in temp if ele and ele.strip()]

                # Tokenize the split list
                for ele in detail:
                    if ele in self.keywords:
                        tokenized_output.append(self.tokens[self.keywords.index(ele)])
                    elif ele in self.symbol:
                        tokenized_output.append(ele)
                    else:
                        tokenized_output.append(ele.strip())

                for ele in detail:
                    tokenized_dict.append(
                        {"value": ele,
                         "line": position,
                         "type": "SYMBOL" if ele in self.symbol else "KEYWORD" if ele in self.keywords else "IDENTIFIER"}
                    )

                # Iterate over a copy of the list to avoid modifying it while iterating
                for item in tokenized_dict[:]:
                    if not item["value"]:
                        tokenized_dict.remove(item)
                    elif "\n" in item["value"]:
                        item["value"] = item["value"].replace("\n", "")

        return tokenized_output, tokenized_dict, tokenized_output_w_spaces