//
// Created by David Yang on 2025-01-22.
//

#pragma once

#include "constants.h"

class Runner {
private:
    std::map<std::string, std::string> var_map;
    std::map<std::string, vec_str> func_map;
    vec_str tokenizedOutput;
    vec_str tokenizedOutputWithSpaces;
    std::vector<std::map<std::string, std::string>> tokenizedDict;

public:
    // Constructor to initialize member variables if needed
    explicit Runner(
        const std::map<std::string, std::string>& varMap = {},
        const std::map<std::string, vec_str>& funcMap = {},
        const vec_str&  tokenOutput = {},
        const vec_str& tokenOutputWithSpaces = {},
        const std::vector<std::map<std::string, std::string>>& tokenDict = {})
        : var_map(varMap),
          func_map(funcMap),
          tokenizedOutput(tokenOutput),
          tokenizedOutputWithSpaces(tokenOutputWithSpaces),
          tokenizedDict(tokenDict)
    {
    }

    // The run method
    void run() {
        for (int i = 0; i < tokenizedOutput.size(); i++) {
            std::cout << tokenizedOutput[i] << std::endl;

        }
    }

    // Getter and Setter methods to manipulate internal data if required
    void setVarMap(const std::map<std::string, std::string>& map) { var_map = map; }
    const std::map<std::string, std::string>& getVarMap() const { return var_map; }

    void setFuncMap(const std::map<std::string, vec_str>& map) { func_map = map; }
    const std::map<std::string, vec_str>& getFuncMap() const { return func_map; }

    void setTokenizedOutput(const vec_str& output) { tokenizedOutput = output; }
    const vec_str& getTokenizedOutput() const { return tokenizedOutput; }

    void setTokenizedOutputWithSpaces(const vec_str& output) { tokenizedOutputWithSpaces = output; }
    const vec_str& getTokenizedOutputWithSpaces() const { return tokenizedOutputWithSpaces; }

    void setTokenizedDict(const std::vector<std::map<std::string, std::string>>& dict) { tokenizedDict = dict; }
    const std::vector<std::map<std::string, std::string>>& getTokenizedDict() const { return tokenizedDict; }
};

