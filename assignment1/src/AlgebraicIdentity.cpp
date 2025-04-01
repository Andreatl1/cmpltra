//=============================================================================
// FILE:
//    Assignment.cpp
//
// DESCRIPTION:
//  Implementare tre passi LLVM che realizzano le seguenti ottimizzazioni:
//  1. Algebraic Identity
//    X + 0 = 0 + X -> X
//    X * 1 = 1 * X -> X
//  2. Strength Reduction (più avanzato)
//  15 * X = X * 15 -> (x << 4) – X
//  y = X / 8 -> y = X >> 3
//  • 3. Multi-Instruction Optimization
//  a = b + 1, c= a− 1 -> a= b+ 1, c= b
//
//
//
// License: MIT
//=============================================================================
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>

#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstrTypes.h>
#include "assignment1.hpp"

//-----------------------------------------------------------------------------
// Assignment implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
namespace assignment1
{

  bool AlgebraicIdentity::runOnBasicBlock(BasicBlock &B) {
    bool Transformed = false;
    
    for (auto &I : B) {
        // Gestisce l'addizione x + 0 o 0 + x
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
            if (BinOp->getOpcode() == Instruction::Add) {
                // Controlla se uno degli operandi è 0
                if (auto *C = dyn_cast<ConstantInt>(BinOp->getOperand(0))) {
                    if (C->isZero()) {
                        // Sostituisce 0 + x con x
                        BinOp->replaceAllUsesWith(BinOp->getOperand(1));
                        Transformed = true;
                    }
                }
                else if (auto *C = dyn_cast<ConstantInt>(BinOp->getOperand(1))) {
                    if (C->isZero()) {
                        // Sostituisce x + 0 con x
                        BinOp->replaceAllUsesWith(BinOp->getOperand(0));
                        Transformed = true;
                    }
                }
            }
            //Gestisce la moltiplicazione x * 1 o 1 * x
            else if (BinOp->getOpcode() == Instruction::Mul) {
                // Controlla se uno degli operandi è 1
                if (auto *C = dyn_cast<ConstantInt>(BinOp->getOperand(0))) {
                    if (C->isOne()) {
                        // Sostituisce 1 * x con x
                        BinOp->replaceAllUsesWith(BinOp->getOperand(1));
                        Transformed = true;
                    }
                }
                else if (auto *C = dyn_cast<ConstantInt>(BinOp->getOperand(1))) {
                    if (C->isOne()) {
                        // Sostituisce x * 1 con x
                        BinOp->replaceAllUsesWith(BinOp->getOperand(0));
                        Transformed = true;
                    }
                }
            }
        }
    }

    
    return Transformed;
}

  bool AlgebraicIdentity::runOnFunction(Function &F)
  {
    bool Transformed = false;

    for (auto Iter = F.begin(); Iter != F.end(); ++Iter)
    {
      if (runOnBasicBlock(*Iter))
      {
        Transformed = true;
      }
    }

    return Transformed;
  }

  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses  AlgebraicIdentity::run(Function &F, FunctionAnalysisManager &)
  {
    if (!runOnFunction(F))
      return PreservedAnalyses::all();

    return PreservedAnalyses::none();
  }


} // namespace
