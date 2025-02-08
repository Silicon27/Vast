#include <utility>

//
// Created by David Yang on 2025-02-07.
//

#pragma once

class Expr {
private:
    std::map<std::string, vec_str> vars;
    std::map<std::string, vec_str> funcs;
    vec_str expr;

public:

    Expr(const std::map<std::string, vec_str>& vars, const std::map<std::string, vec_str>& funcs, vec_str expr) : vars(vars), funcs(funcs), expr(std::move(expr)) {}

    void evalVars() {
        for (std::string &token : expr) {
            if (!vars.contains(token)) {
                continue;
            }
            // Change later to check for if in all types, not just i32
            if (vars[token][0] != "i32") {
                throw std::runtime_error("Error: Expected integer type.");
            }

            token = vars[token][1];

        }
    }

    void evalFuncs() {
        for (int i = 0; i < expr.size(); i++) {
            if (funcs.contains(expr[i])) {
                // Check if the next token is an opening parenthesis
                if (expr[i + 1] != "(") {
                    throw std::runtime_error("Error: Expected '(' after function name.");
                }

                // Get the function arguments
                std::vector<std::string> args;
                for (int j = i + 2; j < expr.size(); j++) {
                    if (expr[j] == ")") {
                        i = j;
                        break;
                    }
                    args.push_back(expr[j]);
                }

                // Check if the function exists
                if (!funcs.contains(expr[i])) {
                    throw std::runtime_error("Error: Function '" + expr[i] + "' does not exist.");
                }

                // Check if the number of arguments match
                if (funcs[expr[i]].size() != args.size()) {
                    throw std::runtime_error("Error: Expected " + std::to_string(funcs[expr[i]].size()) + " arguments for function '" + expr[i] + "'.");
                }

                // Replace the function call with the return value
                expr[i] = funcs[expr[i]][1];
            }
        }
    }
};
