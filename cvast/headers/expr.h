//
// Created by David Yang on 2025-02-07.
//

#pragma once

class Expr {
private:
    std::map<std::string, vec_str> vars;
    std::map<std::string, vec_str> funcs;
    vec_str expr;
    IRGenerator &irgen;
    std::string scope;
    // Maps temporary names to their llvm::Value* (for non-constant function call results).
    std::map<std::string, llvm::Value*> valueMap;
    int tmpCounter = 0;

public:
    Expr(const std::map<std::string, vec_str>& vars,
         const std::map<std::string, vec_str>& funcs,
         vec_str expr,
         IRGenerator &irgen,
         std::string &scope)
         : vars(vars), funcs(funcs), expr(std::move(expr)), irgen(irgen), scope(scope) {}

    void cleanup() {
        // Cleanup implementation if needed.
    }

    // Substitute variables with their stored values.
    void evalVars() {
        for (size_t i = 0; i < expr.size(); ++i) {
            if (!vars.contains(expr[i])) {
                continue;
            }
            // Skip if token is part of a function call (i.e. followed by "(").
            if (funcs.contains(expr[i]) && i + 1 < expr.size() && expr[i + 1] == "(") {
                continue;
            }
            // We assume only integer type for now.
            if (vars[expr[i]][0] != "I32") {
                throw std::runtime_error("Error: Expected integer type.");
            }
            expr[i] = vars[expr[i]][1];
        }
    }

    // Evaluate all function calls in the expression.
    // If a function call returns a nonconstant result, assign it a unique temporary name and store it for later use.
    // The function replaces the tokens corresponding to the function call with the temporary name or constant value.
    llvm::CallInst* evalFuncs() {
        llvm::CallInst* lastCall = nullptr;
        vec_str newExpr;
        for (size_t i = 0; i < expr.size(); i++) {
            // Check if this token is a function and is followed by "(".
            if (funcs.contains(expr[i]) && (i + 1 < expr.size() && expr[i + 1] == "(")) {
                std::string funcName = expr[i];
                // Skip the function name and "(".
                i += 2;
                vec_str argTokens;
                // Gather tokens for the argument list until we find a ")".
                while (i < expr.size() && expr[i] != ")") {
                    if (expr[i] != ",") {
                        argTokens.push_back(expr[i]);
                    }
                    i++;
                }
                // Now, i is at the ")".
                std::vector<llvm::Value*> args;
                for (size_t j = 0; j < argTokens.size(); j++) {
                    // For simplicity, assume that arguments are integer literals.
                    int argVal = std::stoi(argTokens[j]);
                    args.push_back(llvm::ConstantInt::get(llvm::Type::getInt32Ty(irgen.Context), argVal));
                }
                llvm::Function* function = irgen.Module->getFunction(funcName);
                if (!function) {
                    throw std::runtime_error("Error: Function '" + funcName + "' not found.");
                }
                lastCall = irgen.Builder.CreateCall(function, args);

                std::string callResultStr;
                // If the function returns an integer, try to get a constant value.
                if (function->getReturnType()->isIntegerTy()) {
                    if (auto constRes = llvm::dyn_cast<llvm::ConstantInt>(lastCall)) {
                        callResultStr = std::to_string(constRes->getSExtValue());
                    } else {
                        // For nonconstant results, assign a unique temporary name.
                        callResultStr = "tmp" + std::to_string(tmpCounter++);
                        lastCall->setName(callResultStr);
                        // Save the llvm::Value for later arithmetic use.
                        valueMap[callResultStr] = lastCall;
                    }
                } else {
                    callResultStr = "<func>";
                }
                // Replace the function call tokens with the evaluated result.
                newExpr.push_back(callResultStr);
            } else {
                newExpr.push_back(expr[i]);
            }
        }
        expr = newExpr;
        return lastCall;
    }

    // Evaluate the overall expression.
    // This method substitutes variable tokens, processes function calls,
    // and then handles arithmetic operations.
    std::tuple<llvm::CallInst*, llvm::Value*> eval() {
        // Replace variable names with their stored values.
        evalVars();

        // Process function calls. The evalFuncs method updates the expr tokens.
        llvm::CallInst* lastCall = evalFuncs();

        if (expr.empty()) {
            throw std::runtime_error("Error: Expression is empty after processing.");
        }

        // At this point, we assume the expression contains integer literals, temporary names, and arithmetic operators.
        llvm::Value* result = nullptr;
        {
            // Handle the first token.
            // Check if the token is present in our valueMap.
            if (valueMap.contains(expr[0])) {
                result = valueMap[expr[0]];
            } else {
                try {
                    int initialVal = std::stoi(expr[0]);
                    result = llvm::ConstantInt::get(llvm::Type::getInt32Ty(irgen.Context), initialVal);
                } catch (...) {
                    throw std::runtime_error("Invalid first operand: " + expr[0]);
                }
            }
        }

        // Process the rest of the tokens as operator-operand pairs.
        for (size_t i = 1; i < expr.size(); i += 2) {
            if (i + 1 >= expr.size()) {
                throw std::runtime_error("Malformed expression: operator without following operand.");
            }
            std::string op = expr[i];
            llvm::Value* operandVal = nullptr;
            // If the operand token is a temporary name, fetch its value.
            if (valueMap.contains(expr[i + 1])) {
                operandVal = valueMap[expr[i + 1]];
            } else {
                // Otherwise, attempt to interpret it as an integer literal.
                try {
                    int opVal = std::stoi(expr[i + 1]);
                    operandVal = llvm::ConstantInt::get(llvm::Type::getInt32Ty(irgen.Context), opVal);
                } catch (...) {
                    throw std::runtime_error("Invalid operand: " + expr[i + 1]);
                }
            }

            // Create the appropriate arithmetic instruction.
            if (op == "+") {
                result = irgen.Builder.CreateAdd(result, operandVal, "addtmp");
            } else if (op == "-") {
                result = irgen.Builder.CreateSub(result, operandVal, "subtmp");
            } else if (op == "*") {
                result = irgen.Builder.CreateMul(result, operandVal, "multmp");
            } else if (op == "/") {
                result = irgen.Builder.CreateSDiv(result, operandVal, "divtmp");
            } else {
                throw std::runtime_error("Unsupported operator: " + op);
            }
        }

        // Prepare a string representation of the final result.
        // std::string finalResult;
        // if (auto constInt = llvm::dyn_cast<llvm::ConstantInt>(result)) {
        //     finalResult = std::to_string(constInt->getSExtValue());
        // } else {
        //     finalResult = result->hasName() ? result->getName().str() : "<non-constant expression>";
        // }

        return std::make_tuple(lastCall, result);
    }
};
