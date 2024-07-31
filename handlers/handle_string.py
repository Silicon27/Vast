"""
This file contains the string handlers for different types of strings

The string handlers are used to handle strings in the tokenized output

The string handlers are:
    - f_string (under the works)
    - n_string
"""
from typing import Tuple

from errh import faultstack

# Normal string handler
def n_string(quote_type: str, tokenized_output: list, position: int) -> str:
    """
    NString, or normal string, is a string that is enclosed in single or double quotes, default string type

    :param quote_type: The quote type
    :param tokenized_output: The tokenized output
    :param position: The positional index in the tokenized output
    :return: The handled string
    """
    message = ""
    while position < len(tokenized_output) and tokenized_output[position] != quote_type:
        # print(f"handle_string.py: {tokenized_output[position]}")
        message += tokenized_output[position]
        position += 1


    return message, position


def e_string(quote_type: str, tokenized_output: list, position: int, tokenized_dict: list) -> tuple[str, int]:
    """
    EString, or editable string, is a string that can be modified without the need of any other symbols

    :param quote_type: The quote type
    :param tokenized_output: The tokenized output
    :param position: The positional index in the tokenized output
    :return: The handled string
    """
    message = ""
    e_contents = []
    while position < len(tokenized_output) and tokenized_output[position] != quote_type:
        # print(f"handle_string.py: {tokenized_output[position]}")
        if tokenized_output[position] == "$":
            position += 1
            if tokenized_output[position] == "{":
                position += 1
                while position < len(tokenized_output) and tokenized_output[position] != "}":
                    e_contents.append(tokenized_output[position])
                    position += 1
            else:
                faultstack.generate_error_message(tokenized_dict[position]["line"], __file__, list(tokenized_output[position]), faultstack.get_error_position(tokenized_output[position]), "$", "SyntaxError")

        message += tokenized_output[position]
        position += 1


    return message, position