def track_braces(tokenized_output):
    brace_stack = []
    current_scope = []
    scopes = [current_scope]

    for token in tokenized_output:
        if token == '{':
            current_scope.append('{')  # Append the opening brace to the current scope
            new_scope = []
            current_scope.append(new_scope)  # Append the new list to the current scope
            brace_stack.append(current_scope)  # Push the current scope onto the stack
            current_scope = new_scope  # Set the current scope to the new list
        elif token == '}':
            current_scope = brace_stack.pop()  # Pop the previous scope from the stack and set it as the current scope
            current_scope.append('}')  # Append the closing brace to the current scope
        else:
            current_scope.append(token)  # Append the token to the current scope

    return scopes[0]

def print_nested_structure(nested_structure):
    for item in nested_structure:
        if isinstance(item, list):
            print_nested_structure(item)
        else:
            print(item, end=" ")


def process_scopes(tokenized_output: list, condition: bool, position: int):
    index = position
    if condition == False:
        for token in tokenized_output:
            index += 1

            if tokenized_output[index] == "}":
                print(token)
                break
    else:
        for token in tokenized_output:
            index += 1
            if isinstance(token, list):
                index = process_scopes(token, condition, index)
            else:
                print(token)
    position = index
    return position


