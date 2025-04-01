#pragma once
#ifndef ASSIGNMENT1_HPP
#define ASSIGNMENT1_HPP
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>

using namespace llvm;

namespace assignment1
{
    // New PM implementation
    class AlgebraicIdentity : public PassInfoMixin<AlgebraicIdentity>
    {
    public:
        bool runOnBasicBlock(BasicBlock &);
        bool runOnFunction(Function &);
        PreservedAnalyses run(Function &, FunctionAnalysisManager &);
    };

    class StrengthReduction : public PassInfoMixin<StrengthReduction>
    {
    public:
        bool runOnBasicBlock(BasicBlock &);
        bool runOnFunction(Function &);
        PreservedAnalyses run(Function &, FunctionAnalysisManager &);
        bool handleDivision(BinaryOperator *);
        bool handleMultiplication(BinaryOperator *);
    };

    class MultiInstructionOptimization : public PassInfoMixin<MultiInstructionOptimization>
    {
    public:
        bool runOnBasicBlock(BasicBlock &);
        bool runOnFunction(Function &);
        PreservedAnalyses run(Function &, FunctionAnalysisManager &);
    };
}
#endif