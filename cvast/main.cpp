#include "headers/PythonTokenizer.h"
#include <iostream>
#include <string>
#include <algorithm>

// store all values as strings for variables, that is, if it's an array, then it's "[1,2,3]",
// and how some sort of string parser to parse it later

using vec_str = std::vector<std::string>;


#include <map>
#include <string>
#include <vector>
#include "headers/constants.h"
#include "headers/runner.h"

int main(int argc, char *argv[]) {
    // Define inputs
    const std::vector<std::string> keywords = {
        "print", "create", "expand", "export", "declare", "if", "var", "int"
    };
    const std::vector<std::string> tokens = {
        "PRINT","CREATE", "EXPAND", "EXPORT", "DECLARE", "IF", "VAR", "INT"
    };
    const std::vector<std::string> symbols = {
      "[>]", "[?]", "[.]",
      "<=", ">=", "==", "!=", "&&", "||", "++", "--", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "~=", "<<", ">>", "->",
      "(", ")", "{", "}", "<", ">", "=", "+", "-", "*", "/", "%", "!", "&", "|", "^", "~", ":", ";", ",", ".", "?", "@", "#", "$",
    };

    vec_str types = {
        "INT", "FLOAT", "STRING", "BOOL", "ARRAY", "DICT", "TUPLE", "SET", "FILE", "CLASS", "FUNCTION", "MODULE"
    };

    const std::string filePath = argv[1]; 

    // Call the PythonTokenizer class
    const PythonTokenizer tokenizer(keywords, tokens, symbols, filePath);

    vec_str tokenizedOutputWithSpaces;
    std::vector<std::map<std::string, std::string>> tokenizedDict;
    vec_str tokenizedOutput;

    std::map<std::string, vec_str> var_map = {};
    std::map<std::string, vec_str> func_map = {};

    // store all the indexes where scopes start of var_map
    std::vector<int> scopeStarts;

    try {
        tokenizer.tokenize(tokenizedOutput, tokenizedDict, tokenizedOutputWithSpaces);
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Create a Runner object
    Runner runner(var_map, func_map, tokenizedOutput, tokenizedOutputWithSpaces, tokenizedDict, types);

    runner.run();


    return 0;
}
