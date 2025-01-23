//
// Created by David Yang on 2025-01-23.
//

#pragma once

using parser_rt = std::tuple<int, bool>;


// TODO: Create custom exception handler file to handle exceptions
inline void raise(const std::string &message) {
    throw std::runtime_error(message);
}


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
            if (std::string str = tokenizedOutput[position]; std::ranges::all_of(str, ::isalnum)) {
                position++;
                return;
            }
            raise("Expected alphanumeric.");
        }
    }

    namespace abstract {
        inline void _ptype(int& position, const vec_str& tokenizedOutput, const vec_str& types) {
            // get type
            if (const std::string& type = tokenizedOutput[position]; std::ranges::find(types, type) != types.end()) {
                position++;
                return;
            }
            raise("Expected type.");
        }
    }
}
