#pragma once
#ifndef ASSIGNMENT3_HPP
#define ASSIGNMENT3_HPP

#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Analysis/LoopInfo.h>

using namespace llvm;

namespace assignment3 {
    class LoopPass : public PassInfoMixin<LoopPass> {
    public:
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
    };
} // namespace assignment3

#endif