"""
Vast curly brace handler
"""


def count_braces(tokenized_output):
    """
    Count the number of opening and closing braces in the tokenized output
    :param tokenized_output: list
    :return:
    """
    open_brace_count = 0
    close_brace_count = 0
    for token in tokenized_output:
        if token == '{':
            open_brace_count += 1
        elif token == '}':
            close_brace_count += 1

    return open_brace_count, close_brace_count
