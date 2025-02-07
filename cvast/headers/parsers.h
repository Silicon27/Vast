//
// Created by David Yang on 2025-01-23.
//

#pragma once

using parser_rt = std::tuple<int, bool>;


// TODO: Create custom exception handler file to handle exceptions
inline void raise(const std::string &message) {
    throw std::runtime_error(message);
}

using parserType = std::function<void(int&, const vec_str&)>;

inline vec_str type;


namespace parsers {
    namespace keyword {
        // var
        inline void _pvar(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == "VAR") {
                position++;
                return;
            }
            raise("Expected 'var' keyword.");
        }

        inline void _pfn(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == "FN") {
                position++;
                return;
            }
            raise("Expected 'fn' keyword.");
        }

        inline void _preturn(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == "RETURN") {
                position++;
                return;
            }
            raise("Expected 'return' keyword.");
        }
    }


    namespace symbol {
        inline void _pplus_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == "+") {
                position++;
                return;
            }
            raise("Expected '+' symbol.");
        }

        inline void _pequals_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == "=") {
                position++;
                return;
            }
            raise("Expected '=' symbol.");
        }

        inline void _psemicolon_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == ";") {
                position++;
                return;
            }
            raise("Expected ';' symbol.");
        }

        inline void _pcolon_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == ":") {
                position++;
                return;
            }
            raise("Expected ':' symbol.");
        }

        inline void _popen_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == "(") {
                position++;
                return;
            }
            raise("Expected '(' symbol.");
        }

        inline void _pclose_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == ")") {
                position++;
                return;
            }
            raise("Expected ')' symbol.");
        }

        inline void _popencurly_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == "{") {
                position++;
                return;
            }
            raise("Expected '{' symbol.");
        }

        inline void _pclosecurly_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == "}") {
                position++;
                return;
            }
            raise("Expected '}' symbol.");
        }

        inline void _pangleleft_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == "<") {
                position++;
                return;
            }
            raise("Expected '<' symbol.");
        }

        inline void _pangleright_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == ">") {
                position++;
                return;
            }
            raise("Expected '>' symbol.");
        }

        inline void _pcomma_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == ",") {
                position++;
                return;
            }
            raise("Expected ',' symbol.");
        }

        inline void _ppointer_sym(int& position, const vec_str& tokenizedOutput) {
            if (tokenizedOutput[position] == "->") {
                position++;
                return;
            }
            raise("Expected '->' symbol.");
        }

    }

    namespace ascii {
        inline void _pint(int& position, const vec_str& tokenizedOutput) {
            /* check if the token is an integer */
            for (const char c : tokenizedOutput[position]) {
                if (!isdigit(c)) {
                    raise("Expected integer.");
                }
            }
            position++;
        }

        inline void isalnum(int& position, const vec_str& tokenizedOutput) {
            /* check if the token is an integer */
            if (std::string str = tokenizedOutput[position];
                std::ranges::all_of(str,
                    [](const char c) { return std::isalnum(c) || c == '_'; })) {
                position++;
                return;
            }
            raise("Expected alphanumeric.");
        }

        inline std::string isalnum_ret(int& position, const vec_str& tokenizedOutput) {
            if (std::string str = tokenizedOutput[position];
                std::ranges::all_of(str,
                    [](const char c) { return std::isalnum(c) || c == '_'; })) {
                position++;
                return str;
            }
            raise("Expected alphanumeric.");
            return "";
        }
    }

    namespace abstract {
        inline void _ptype(int& position, const vec_str& tokenizedOutput) {
            // get type
            if (const std::string& typ = tokenizedOutput[position]; std::ranges::find(type, typ) != type.end()) {
                position++;
                return;
            }
            raise("Expected type.");
        }

        inline void _poptional(int& position, const vec_str& tokenizeOutput, const parserType& func) {
            try {
                func(position, tokenizeOutput);
            } catch (const std::runtime_error& e) {
                // do nothing
            }
        }

        inline void _poptional_matchAll(int& position, const vec_str& tokenizeOutput, const std::function<void(int&, const vec_str&, const parserType)>& func, const parserType& matchForFunc) {
            try {
               func(position, tokenizeOutput, matchForFunc);
            } catch (const std::runtime_error& e) {
                // do nothing
            }
        }
    }

    namespace modifier {
        inline void matchAll(int& position, const vec_str& tokenizedOutput, const parserType &func) {
            while (position < tokenizedOutput.size()) {
                func(position, tokenizedOutput);
            }
        }

        inline void quickParser(int& position, const vec_str& tokenizedOutput, const std::string& matchFor) {
            if (tokenizedOutput[position] == matchFor) {
                position++;
                return;
            }
            raise("Expected '" + matchFor + "'.");
        }

        inline parserType combine(std::initializer_list<parserType> funcs) {

            // Return a lambda function that processes all functions in the initializer list
            return [funcs](int& pos, const vec_str& tokenizedOutput) {
                for (const auto& func : funcs) {
                    func(pos, tokenizedOutput);
                }
            };

        }
    }
}
