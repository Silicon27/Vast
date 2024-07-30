"""
This file contains the string handlers for different types of strings

The string handlers are used to handle strings in the tokenized output

The string handlers are:
    - f_string (under the works)
    - n_string
"""
# Normal string handler
def n_string(quote_type: str, tokenized_output: list, position: int) -> str:
    """
    Handle the string by removing the quotes and escaping the special characters

    :param tokenized_output: The tokenized output
    :return: The handled string
    """
    message = ""
    while tokenized_output[position] != quote_type:
        # print(f"handle_string.py: {tokenized_output[position]}")
        message += tokenized_output[position]
        position += 1


    return message, position