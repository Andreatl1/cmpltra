//=============================================================================
// FILE:
//    MultiInstOpt.cpp
//
// DESCRIPTION:
//  Multi-Instruction Optimization Pass:
//  a = b + 1, c = a − 1 => a = b+ 1, c= b
//
// License: MIT
//=============================================================================
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstrTypes.h>
#include "assignment1.hpp"


namespace assignment1
{

bool MultiInstructionOptimization::runOnBasicBlock(BasicBlock &BB) {
    bool Transformed = false;               

    for (auto &I : BB) {

        // Controlliamo se l'istruzione è un operatore binario e ha degli usi
        BinaryOperator *InstBo = dyn_cast<BinaryOperator>(&I);
        if (!InstBo || InstBo->use_empty()) continue;

        // Otteniamo l'operazione inversa per l'ottimizzazione (es. Add -> Sub)
        Instruction::BinaryOps inverseOp;
        switch (InstBo->getOpcode()) {
            case Instruction::Add: inverseOp = Instruction::Sub; break;  // a + b → l'inverso è -
            case Instruction::Sub: inverseOp = Instruction::Add; break;  // a - b → l'inverso è +
            case Instruction::Mul: inverseOp = Instruction::SDiv; break; // a * b → l'inverso è /
            case Instruction::SDiv: inverseOp = Instruction::Mul; break; // a / b → l'inverso è *
            default: continue; 
        }

        // Iteriamo gli usi dell'operatore binario corrente
        for (auto *User : InstBo->users()) {
            BinaryOperator *UserBO = dyn_cast<BinaryOperator>(User);
            
            // Controlliamo che l'uso: 
            //    - Sia un operatore binario
            //    - Sia l'inverso dell'istruzione che vogliamo ottimizzare
            //    - Abbia lo stesso operando destro
            if (UserBO && 
                UserBO->getOpcode() == inverseOp &&         
                UserBO->getOperand(1) == InstBo->getOperand(1)) 
            {
                // Sostituiamo tutti gli usi con l'operando sinistro originale
                UserBO->replaceAllUsesWith(InstBo->getOperand(0));
                
                Transformed = true;          
            } 
        }
    }
    
    return Transformed; 
}

  bool MultiInstructionOptimization::runOnFunction(Function &F)
  {
    bool Transformed = false;

    for (auto Iter = F.begin(); Iter != F.end(); ++Iter)
      if (runOnBasicBlock(*Iter))
        Transformed = true;

    return Transformed;
  }


  PreservedAnalyses  MultiInstructionOptimization::run(Function &F, FunctionAnalysisManager &)
  {
    if (!runOnFunction(F))
      return PreservedAnalyses::all();

    return PreservedAnalyses::none();
  }

} 


