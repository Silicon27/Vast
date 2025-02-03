//
// Created by David Yang on 2025-01-22.
//

#pragma once

#include <utility>

#include "parsers.h"
#include "irgen.h"
#include <stack>

inline IRGenerator irgen("__main__");

class Runner {
private:
    std::map<std::string, vec_str> var_map;
    std::map<std::string, vec_str> func_map;
    vec_str tokenizedOutput;
    vec_str tokenizedOutputWithSpaces;
    std::vector<std::map<std::string, std::string>> tokenizedDict;
    vec_str types;
    std::string scope = "global"; // currect scope

public:
    // Constructor to initialize member variables if needed
    explicit Runner(
        const std::map<std::string, vec_str>& varMap = {},
        const std::map<std::string, vec_str>& funcMap = {},
        vec_str   tokenOutput = {},
        vec_str  tokenOutputWithSpaces = {},
        const std::vector<std::map<std::string, std::string>>& tokenDict = {},
        vec_str  types = {},
        std::string scope = "global")
        : var_map(varMap),
          func_map(funcMap),
          tokenizedOutput(std::move(tokenOutput)),
          tokenizedOutputWithSpaces(std::move(tokenOutputWithSpaces)),
          tokenizedDict(tokenDict),
          types(std::move(types)),
          scope(std::move(scope))
    {
    }

    static int getScope(int pos, const vec_str& tokenizedOutput) {
        int inScope = 0;
        for (int i = pos; i < tokenizedOutput.size(); i++) {
            if (tokenizedOutput[i] == "{") {
                inScope++;
            } else if (tokenizedOutput[i] == "}") {
                inScope--;
            }
            if (inScope == 0) {
                return i;
            }
        }
        return {};
    }

    // define the functions for each of the keywords
    [[nodiscard]] int _var(int pos) {
        /*
         * var int a = 10;
         *
         * var h;
         */

        // // store the initial position
        // int initial = pos;

        // check syntax
        parsers::keyword::_pvar(pos, tokenizedOutput); // var

        std::string var_type = tokenizedOutput[pos];
        parsers::abstract::_ptype(pos, tokenizedOutput); // type

        const std::string var_name = tokenizedOutput[pos];
        parsers::ascii::isalnum(pos, tokenizedOutput); // variable name

        parsers::symbol::_pequals_sym(pos, tokenizedOutput); // =

        if (var_type == "I32") {
            parsers::ascii::_pint(pos, tokenizedOutput); // i32
        } else {
            raise("Expected '" + var_type + "' type.");
        }
        const auto var_value = tokenizedOutput[pos - 1];

        parsers::symbol::_psemicolon_sym(pos, tokenizedOutput); // ;

        // store the variable in the map
        var_map[var_name] = {var_type, var_value};

        // // !NOTE: this is a test, will be removed later
        // toasm::dmov::_tmov("x20", "x21");

        // createModule();

        irgen.createVariable(var_name, llvm::Type::getInt32Ty(irgen.Context), var_value, "global");

        return --pos;
    }

    [[nodiscard]] int _func(int pos) const {
        /*
         * fn <int> add(int a, int b) {
         *   return a + b;
         * }
         */

        parsers::keyword::_pfn(pos, tokenizedOutput); // fn

        parsers::ascii::isalnum(pos, tokenizedOutput); // function name
        const std::string funcName = tokenizedOutput[pos - 1];

        parsers::symbol::_popen_sym(pos, tokenizedOutput); // (

        parsers::abstract::_poptional_matchAll(pos, tokenizedOutput,
            parsers::modifier::matchAll,
            parsers::modifier::combine({parsers::ascii::isalnum, parsers::symbol::_pcolon_sym, parsers::abstract::_ptype, parsers::symbol::_pcomma_sym})); // arguments

        parsers::symbol::_pclose_sym(pos, tokenizedOutput); // )

        parsers::symbol::_ppointer_sym(pos, tokenizedOutput); // ->

        parsers::abstract::_ptype(pos, tokenizedOutput); // return type

        const int scopeEnd = getScope(pos, tokenizedOutput);

        const vec_str funcTokenizedOutput(tokenizedOutput.begin() + pos+1, tokenizedOutput.begin() + scopeEnd);
        const vec_str funcTokenizedOutputWithSpaces(tokenizedOutputWithSpaces.begin() + pos+1, tokenizedOutputWithSpaces.begin() + scopeEnd);
        const std::vector funcTokenizedDict(tokenizedDict.begin() + pos+1, tokenizedDict.begin() + scopeEnd);

        for (const auto & i : funcTokenizedOutput) {
            std::cout << "thing: " << i << std::endl;
        }


        parsers::symbol::_popencurly_sym(pos, tokenizedOutput); // {

        Runner runner(this->getVarMap(), this->getFuncMap(), funcTokenizedOutput, funcTokenizedOutputWithSpaces, funcTokenizedDict, this->types, funcName);
        runner.run();

        return --pos;
    }
    int _if(int pos);
    int _elif(int pos);
    int _else(int pos);


    // The run method
    void run() {

        IRGenerator irgen("main");
        type = this->types;

        std::cout << "Running the code..." << std::endl;

        for (int i = 0; i < tokenizedOutput.size(); i++) {
            std::cout << tokenizedOutput[i] << std::endl;
            if (tokenizedOutput[i] == "VAR") {
                i = _var(i);
            } else if (tokenizedOutput[i] == "FN") {
                i = _func(i);
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