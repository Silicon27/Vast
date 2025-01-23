//
// Created by David Yang on 2025-01-23.
//

#pragma once

using parser_rt = std::tuple<int, bool>;


namespace parsers {
    namespace keyword {
        // var
        inline parser_rt _pvar(int position, const vec_str &tokenizedOutput) {
            if (tokenizedOutput[position] == "var") {
                return {position + 1, true};
            }
            return {position, false};
        }
    }


    namespace symbol {
        inline parser_rt _pplus_sym(int position, const vec_str &tokenizedOutput) {
            if (tokenizedOutput[position] == "+") {
                return {position + 1, true};
            }
            return {position, false};
        }

        inline parser_rt _pequals_sym(int position, const vec_str &tokenizedOutput) {
            if (tokenizedOutput[position] == "=") {
                return {position + 1, true};
            }
            return {position, false};
        }
    }

    namespace custom {
        // for later...
    }
}
