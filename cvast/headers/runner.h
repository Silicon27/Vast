//
// Created by David Yang on 2025-01-22.
//

#pragma once

struct Variable {
    std::string name;
    std::string type;
    std::string value;
};

#include "parsers.h"
#include "irgen.h"

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

    static int getScope(const int pos, const vec_str& tokenizedOutput) {
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
        return pos;
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

        std::string var_value;
        llvm::Value* value;
        if (var_type == "I32") {
            parsers::ascii::_pint(pos, tokenizedOutput); // i32
            var_value = tokenizedOutput[pos - 1];

            value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(irgen.Context), std::stoi(var_value));

        } else {
            raise("Expected '" + var_type + "' type.");
            value = nullptr;
        }


        parsers::symbol::_psemicolon_sym(pos, tokenizedOutput); // ;

        // store the variable in the map
        var_map[var_name] = {var_type, var_value};

        // // !NOTE: this is a test, will be removed later
        // toasm::dmov::_tmov("x20", "x21");

        // createModule();

        irgen.createVariable(var_name, llvm::Type::getInt32Ty(irgen.Context), value, scope);

        return --pos;
    }

    [[nodiscard]] int _func(int pos) const {
        /*
         * fn <int> add(int a, int b) {
         *   return a + b;
         * }
         */

        parsers::keyword::_pfn(pos, tokenizedOutput); // fn

        const std::string funcName = parsers::ascii::isalnum_ret(pos, tokenizedOutput); // function name

        parsers::symbol::_popen_sym(pos, tokenizedOutput); // (

        const int original_pos = pos;

        parsers::abstract::_poptional_matchAll(pos, tokenizedOutput,
            parsers::modifier::matchAll,
            parsers::modifier::combine(
                {parsers::ascii::isalnum, parsers::symbol::_pcolon_sym,
                    parsers::abstract::_ptype, parsers::symbol::_pcomma_sym})); // arguments


        std::vector<Variable> arguments_list;
        for (int i = original_pos; i < pos; i++) {
            if (tokenizedOutput[i] == ")") {
                break;
            }
            Variable var;

            if (std::string str = tokenizedOutput[i];
                std::ranges::all_of(str,
                    [](const char c) { return std::isalnum(c) || c == '_'; })) {
                i++;
                var.name = str;
            } else {
                raise("Expected alphanumeric.");
            }

            if (tokenizedOutput[i] == ":") {
                i++;
            } else {
                raise("Expected ':' symbol.");
            }

            if (tokenizedOutput[i] == "I32") {
                i++;
                var.type = "I32";
            } else {
                raise("Expected valid type.");
            }

            var.value = std::to_string(cvast::constants::Null::null);

            arguments_list.push_back(var);
        }

        parsers::symbol::_pclose_sym(pos, tokenizedOutput); // )

        parsers::symbol::_ppointer_sym(pos, tokenizedOutput); // ->

        parsers::abstract::_ptype(pos, tokenizedOutput); // return typex

        llvm::Type *returnType;
        if (tokenizedOutput[pos - 1] == "I32") {
            returnType = llvm::Type::getInt32Ty(irgen.Context);
        } else {
            raise("Expected valid return type.");
        }

        std::vector<llvm::Type*> argTypes;

        argTypes.reserve(arguments_list.size());
        for (const auto& arg : arguments_list) {
            argTypes.push_back(arg.type == "I32" ? llvm::Type::getInt32Ty(irgen.Context) : nullptr);
        }

        irgen.createFunction(funcName, returnType, argTypes, arguments_list);

        const int scopeEnd = getScope(pos, tokenizedOutput);

        const vec_str funcTokenizedOutput(tokenizedOutput.begin() + pos+1, tokenizedOutput.begin() + scopeEnd);
        const vec_str funcTokenizedOutputWithSpaces(tokenizedOutputWithSpaces.begin() + pos+1, tokenizedOutputWithSpaces.begin() + scopeEnd);
        const std::vector funcTokenizedDict(tokenizedDict.begin() + pos+1, tokenizedDict.begin() + scopeEnd);

        parsers::symbol::_popencurly_sym(pos, tokenizedOutput); // {
        pos = scopeEnd;

        Runner runner(this->getVarMap(), this->getFuncMap(), funcTokenizedOutput, funcTokenizedOutputWithSpaces, funcTokenizedDict, this->types, funcName);
        runner.run();

        parsers::symbol::_pclosecurly_sym(pos, tokenizedOutput); // }

        irgen.dump();

        return --pos;
    }

    int _return(int pos) {
        parsers::keyword::_preturn(pos, tokenizedOutput); // return

        parsers::ascii::_pint(pos, tokenizedOutput); // i32
        std::string value = tokenizedOutput[pos - 1];

        parsers::symbol::_psemicolon_sym(pos, tokenizedOutput); // ;

        irgen.createReturn(llvm::ConstantInt::get(llvm::Type::getInt32Ty(irgen.Context), std::stoi(value)), scope);

        return pos;
    }

    int _if(int pos);
    int _elif(int pos);
    int _else(int pos);


    // The run method
    void run() {

        IRGenerator irgen(scope);
        type = this->types;

        std::cout << "Running the code..." << std::endl;

        for (int i = 0; i < tokenizedOutput.size(); i++) {
            std::cout << tokenizedOutput[i] << std::endl;
            if (tokenizedOutput[i] == "VAR") {
                i = _var(i);
            } else if (tokenizedOutput[i] == "FN") {
                i = _func(i);
            } else if (tokenizedOutput[i] == "RETURN") {
                i = _return(i);
            }
        }

        for (const auto& [key, value] : var_map) {
            std::cout << key << " : " << value[0] << " : " << value[1] << std::endl;
        }

        irgen.validityCheck();
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