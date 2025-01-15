#include "headers/PythonTokenizer.h"

int main(int argc, char *argv[]) {
    // Define inputs
    const std::vector<std::string> keywords = {
        "print", "(", ")", "\"", "'", "{", "}", "<", ">", "=", "create", "expand", "export", "declare", "if"
    };
    const std::vector<std::string> tokens = {
        "PRINT", "(", ")", "\"", "'", "{", "}", "<", ">", "=", "CREATE", "EXPAND", "EXPORT", "DECLARE", "IF"
    };
    const std::vector<std::string> symbols = {
        "(", ")", "{", "}", "[", "]", ".", ",", "=", "<", ">", ">=", "<=", "//",
    };
    const std::string filePath = argv[1];

    // Call the PythonTokenizer class
    const PythonTokenizer tokenizer(keywords, tokens, symbols, filePath);
    try {
        std::vector<std::string> tokenizedOutputWithSpaces;
        std::vector<std::map<std::string, std::string>> tokenizedDict;
        std::vector<std::string> tokenizedOutput;
        tokenizer.tokenize(tokenizedOutput, tokenizedDict, tokenizedOutputWithSpaces);

        // You can now use tokenizedOutput, tokenizedDict, and tokenizedOutputWithSpaces in your program
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
