//
// Created by David Yang on 2025-01-30.
//

#pragma once


#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"



class IRGenerator {
public:
    llvm::LLVMContext Context;
    llvm::IRBuilder<> Builder;
    std::unique_ptr<llvm::Module> Module;

    explicit IRGenerator(const std::string &ModuleName)
        : Builder(Context), Module(std::make_unique<llvm::Module>(ModuleName, Context)) {}

    llvm::Function* createFunction(const std::string &Name, llvm::Type *ReturnType, const std::vector<llvm::Type*> &Args) const {
        auto *FunctionType = llvm::FunctionType::get(ReturnType, Args, false);
        return llvm::Function::Create(FunctionType, llvm::Function::ExternalLinkage, Name, Module.get());
    }

    std::variant<llvm::GlobalVariable*, llvm::AllocaInst*> createVariable(const std::string &Name,
        llvm::Type *Type, auto value, const std::string& scope, llvm::Function *func = nullptr) {
        /**
         * Variable creation method for IRGenerator class.
         *
         * @param Name The name of the variable.
         * @param Type The type of the variable.
         * @param value The value of the variable.
         * @param scope The scope of the variable.
         * @param func The function to create the variable in.
         * @return A variant containing the created global or local variable.
         */

        std::variant<llvm::GlobalVariable*, llvm::AllocaInst*> var;
        if (scope == "global") {
            var = new llvm::GlobalVariable(*Module, Type, false, llvm::GlobalValue::ExternalLinkage, nullptr, Name);

            Module->print(llvm::outs(), nullptr);
        } else {
            llvm::BasicBlock *EntryBlock = llvm::BasicBlock::Create(Context, "entry", func);
            Builder.SetInsertPoint(EntryBlock);

            var = Builder.CreateAlloca(Type, nullptr, Name);

            Builder.CreateStore(
                llvm::Constant::getNullValue(Type),
                std::visit([](auto&& ptr) -> llvm::Value* { return static_cast<llvm::Value*>(ptr); }, var)
            );

            Builder.CreateRetVoid();

            Module->print(llvm::outs(), nullptr);
        }

        return var;

    }

    void dump() const {
        Module->print(llvm::errs(), nullptr);
    }
};


