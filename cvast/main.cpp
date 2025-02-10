#include "headers/PythonTokenizer.h"
#include <iostream>
#include <string>
#include <ranges>
#include <fstream>
#include <sstream>
#include <filesystem>


// store all values as strings for variables, that is, if it's an array, then it's "[1,2,3]",
// and how some sort of string parser to parse it later

using vec_str = std::vector<std::string>;


#include <map>
#include <vector>
#include "headers/constants.h"
#include "headers/runner.h"
#include "headers/preprocessor.h"

int main(int argc, char *argv[]) {

    // Define inputs
    const std::vector<std::string> keywords = {
        "expand", "export", "if", "var", "i16", "i32", "i64", "fn", "return"
    };
    const std::vector<std::string> tokens = {
        "EXPAND", "EXPORT", "IF", "VAR", "I16", "I32", "I64", "FN", "RETURN"
    };
    const std::vector<std::string> symbols = {
      "[>]", "[?]", "[.]",
      "<=", ">=", "==", "!=", "&&", "||", "++", "--", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "~=", "<<", ">>", "->",
      "(", ")", "{", "}", "<", ">", "=", "+", "-", "*", "/", "%", "!", "&", "|", "^", "~", ":", ";", ",", ".", "?", "@", "#", "$",
    };

    vec_str types = {
        "I16", "I32", "I64", "FLOAT", "STRING", "BOOL", "ARRAY", "DICT", "TUPLE", "SET", "FILE", "CLASS", "FUNCTION", "MODULE"
    };

    if (argc > 4) {
        std::cerr << "Error: Too many arguments. Usage: vast <input_file> -o <output_file>" << std::endl;
        return 1;
    }

    std::string input_file;
    std::string output_file;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-o") {
            // Ensure there is an argument after "-o"
            if (i + 1 < argc) {
                output_file = argv[i + 1];
                i++; // Skip next argument as it's the output file
            } else {
                std::cerr << "Error: No output file specified after '-o'" << std::endl;
                return 1;
            }
        } else if (input_file.empty()) {
            // Assume it's the input file if input_file not already set
            input_file = argv[i];
        } else {
            // Any additional argument not following the "-o" pattern is invalid
            std::cerr << "Error: Unexpected argument '" << argv[i] << "'" << std::endl;
            return 1;
        }
    }

    // Perform preprocessing
    Preprocessor preprocessor(input_file);
    std::string import_appends = preprocessor.preprocess();

    std::cout << input_file << std::endl;


    // Call the PythonTokenizer class
    const PythonTokenizer tokenizer(keywords, tokens, symbols, input_file);

    vec_str tokenizedOutputWithSpaces;
    std::vector<std::map<std::string, std::string>> tokenizedDict;
    vec_str tokenizedOutput;

    std::map<std::string, vec_str> var_map = {};
    std::map<std::string, vec_str> func_map = {};
    std::map<std::string, vec_str> func_args = {};
    std::map<std::string, std::string> func_return = {};

    // store all the indexes where scopes start of var_map
    std::vector<int> scopeStarts;

    try {
        tokenizer.tokenize(tokenizedOutput, tokenizedDict, tokenizedOutputWithSpaces);
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }



    // Create a Runner object
    Runner runner(var_map, func_map, func_args, func_return, tokenizedOutput, tokenizedOutputWithSpaces, tokenizedDict, types);

    runner.run();

    irgen.emitBinary();
    irgen.dump();

    return 0;
}
