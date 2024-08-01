"""
This module contains the functions that handle given conditions, returning a boolean value

"""

def check_if(conditions: list, variables: list) -> bool:
    """
    Check if the conditions are met

    :param conditions: The conditions to check
    :return: True if all conditions are met, False otherwise
    """
    # Get all indexes from conditions
    for condition in conditions:
        for variable in variables:
            if condition == variable["name"]:
                # Replace the condition with the value of the variable
                conditions[conditions.index(condition)] = variable["value"]

    print(conditions)
    string_condition = " ".join(conditions)
    return eval(string_condition)


