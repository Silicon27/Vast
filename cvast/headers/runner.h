//
// Created by David Yang on 2025-01-22.
//

#pragma once

#include "parsers.h"
#include "toasm.h"

class Runner {
private:
    std::map<std::string, vec_str> var_map;
    std::map<std::string, vec_str> func_map;
    vec_str tokenizedOutput;
    vec_str tokenizedOutputWithSpaces;
    std::vector<std::map<std::string, std::string>> tokenizedDict;
    vec_str types;

public:
    // Constructor to initialize member variables if needed
    explicit Runner(
        const std::map<std::string, vec_str>& varMap = {},
        const std::map<std::string, vec_str>& funcMap = {},
        vec_str   tokenOutput = {},
        vec_str  tokenOutputWithSpaces = {},
        const std::vector<std::map<std::string, std::string>>& tokenDict = {},
        vec_str  types = {},
        const std::string&  outputFile = "")
        : var_map(varMap),
          func_map(funcMap),
          tokenizedOutput(std::move(tokenOutput)),
          tokenizedOutputWithSpaces(std::move(tokenOutputWithSpaces)),
          tokenizedDict(tokenDict),
          types(std::move(types))
    {
        if (!outputFile.empty()) {
            initializeOutputFile(outputFile);
        }
    }

    // define the functions for each of the keywords
    int _var(int pos) {
        /*
         * var int a = 10;
         *
         * var h;
         */

        // // store the initial position
        // int initial = pos;

        // check syntax
        parsers::keyword::_pvar(pos, tokenizedOutput); // var

        const std::string var_type = tokenizedOutput[pos];
        parsers::abstract::_ptype(pos, tokenizedOutput, types); // type

        const std::string var_name = tokenizedOutput[pos];
        parsers::ascii::isalnum(pos, tokenizedOutput); // variable name

        parsers::symbol::_pequals_sym(pos, tokenizedOutput); // =

        if (var_type == "INT") {
            parsers::ascii::_pint(pos, tokenizedOutput); // int
        } else {
            raise("Expected '" + var_type + "' type.");
        }
        const auto var_value = tokenizedOutput[pos - 1];

        parsers::symbol::_psemicolon_sym(pos, tokenizedOutput); // ;

        // store the variable in the map
        var_map[var_name] = {var_type, var_value};

        // !NOTE: this is a test, will be removed later
        toasm::dmov::_pmov("x20", "x21");

        return pos;
    }
    int _func(int pos);
    int _if(int pos);
    int _elif(int pos);
    int _else(int pos);


    // The run method
    void run() {
        for (int i = 0; i < tokenizedOutput.size(); i++) {
            std::cout << tokenizedOutput[i] << std::endl;
            if (tokenizedOutput[i] == "VAR") {
                i = _var(i);
            }
        }

        for (const auto& [key, value] : var_map) {
            std::cout << key << " : " << value[0] << " : " << value[1] << std::endl;
        }
    }

    // Getter and Setter methods to manipulate internal data if required
    void setVarMap(const std::map<std::string, vec_str>& map) { var_map = map; }
    [[nodiscard]] const std::map<std::string, vec_str>& getVarMap() const { return var_map; }

    void setFuncMap(const std::map<std::string, vec_str>& map) { func_map = map; }
    [[nodiscard]] const std::map<std::string, vec_str>& getFuncMap() const { return func_map; }

    void setTokenizedOutput(const vec_str& output) { tokenizedOutput = output; }
    [[nodiscard]] const vec_str& getTokenizedOutput() const { return tokenizedOutput; }

    void setTokenizedOutputWithSpaces(const vec_str& output) { tokenizedOutputWithSpaces = output; }
    [[nodiscard]] const vec_str& getTokenizedOutputWithSpaces() const { return tokenizedOutputWithSpaces; }

    void setTokenizedDict(const std::vector<std::map<std::string, std::string>>& dict) { tokenizedDict = dict; }
    [[nodiscard]] const std::vector<std::map<std::string, std::string>>& getTokenizedDict() const { return tokenizedDict; }

};

