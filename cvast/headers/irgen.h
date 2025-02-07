//
// Created by David Yang on 2025-01-30.
//

#pragma once


#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/Program.h>
#include <llvm/TargetParser/Triple.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/TargetParser/Host.h>


class IRGenerator {
public:
    llvm::LLVMContext Context;
    llvm::IRBuilder<> Builder;
    std::unique_ptr<llvm::Module> Module;

    explicit IRGenerator(const std::string &ModuleName)
        : Builder(Context), Module(std::make_unique<llvm::Module>(ModuleName, Context)) {}

    llvm::Function* createFunction(const std::string &Name, llvm::Type *ReturnType, const std::vector<llvm::Type*> &ArgsType,
        const std::vector<Variable>& Arguments) {
        /**
         * Function creation method for IRGenerator class.
         *
         * @param Name The name of the function.
         * @param ReturnType The return type of the function.
         * @param ArgsType The types of the arguments.
         * @param Arguments The arguments of the function.
         * @return The created function.
         */

        auto *FunctionType = llvm::FunctionType::get(ReturnType, ArgsType, false);

        // Create the function
        llvm::Function *Function = llvm::Function::Create(
            FunctionType, llvm::Function::ExternalLinkage, Name, Module.get());

        // Set names for all arguments
        size_t idx = 0;
        for (auto &Arg : Function->args()) {
            Arg.setName(Arguments[idx].name);
            idx++;
        }

        // put a entry basic block in the function
        llvm::BasicBlock *BB = llvm::BasicBlock::Create(Context, "entry", Function);
        Builder.SetInsertPoint(BB);

        return Function;
    }

    std::variant<llvm::GlobalVariable *, llvm::AllocaInst *, llvm::Value *> createVariable(const std::string &Name,
        llvm::Type *Type, llvm::Value *value, const std::string &scope) {
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

        std::variant<llvm::GlobalVariable*, llvm::AllocaInst*, llvm::Value*> var;
        llvm::Value* varValue;
        if (scope == "global") {
            auto *globalVar = new llvm::GlobalVariable(*Module, Type, false, llvm::GlobalValue::ExternalLinkage, nullptr, Name);
            var = globalVar;
        } else {
            llvm::Function *ExistingFunction = Module->getFunction(scope);

            llvm::BasicBlock *EntryBlock = &ExistingFunction->getEntryBlock();

            Builder.SetInsertPoint(EntryBlock, EntryBlock->getFirstInsertionPt());

            llvm::AllocaInst* allocaInst = Builder.CreateAlloca(Type, nullptr, Name);
            var = allocaInst;

            Builder.CreateStore(value, allocaInst);
        }

        return var;
    }

    void createReturn(llvm::Value *Value, const std::string &scope) {
        /**
         * Return creation method for IRGenerator class.
         *
         * @param Value The value to return.
         */

        llvm::Function *Function = Module->getFunction(scope);

        Builder.SetInsertPoint(&Function->getEntryBlock(), Function->getEntryBlock().end());
        Builder.CreateRet(Value);

    }



    void dump() const {
        Module->print(llvm::errs(), nullptr);
    }

    void validityCheck() const {
        if (verifyModule(*Module, &llvm::errs())) {
            llvm::errs() << "Error: Module verification failed." << "\n";
        } else {
            llvm::outs() << "Module verification passed." << "\n";
        }

    }

    llvm::LLVMContext &getContext() {
        return Context;
    }

    void emitBinary() const {

        // Create a .ll file
        std::error_code ec;
        llvm::raw_fd_ostream outFile("output.ll", ec, llvm::sys::fs::OF_Text);
        if (ec) {
            llvm::errs() << "Could not open output.ll: " << ec.message() << "\n";
            return;
        }
        Module->print(outFile, nullptr);
        outFile.flush();
        // Optionally, check the size of the file after closing:
        outFile.close();

        // use llc -filetype=obj output.ll -o output.o
        const std::string command = "llc -filetype=obj output.ll -o output.o";
        std::system(command.c_str());

        // delete the .ll file

        // use clang output.o -o output

        try {
            std::system("clang output.o -o output");

        } catch (const std::exception& e) {
            llvm::errs() << "Error: " << e.what() << "\n";
        }
        
    }
};


