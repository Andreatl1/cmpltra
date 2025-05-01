#include "assignment3.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace assignment3 {

PreservedAnalyses LoopPass::run(Function &F, FunctionAnalysisManager &FAM) {
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
    
    // Step 1: Check if CFG contains loops
    if (LI.empty()) 
        return PreservedAnalyses::all();

    // Step 2: Find and print loop headers
    errs() << "Loop headers in function '" << F.getName() << "':\n";
    for (const BasicBlock &BB : F) {
        if (LI.isLoopHeader(&BB)) {
            errs() << "  " << BB.getName() << "\n";
        }
    }

    // Step 3: Process all loops
    errs() << "\nProcessing loops in function '" << F.getName() << "':\n";
    for (Loop *L : LI.getLoopsInPreorder()) {
        // 3a: Check normal form
        bool isNormal = L->isLoopSimplifyForm();
        errs() << "Loop with header " << L->getHeader()->getName()
               << " is in normal form: " << (isNormal ? "Yes" : "No") << "\n";

        // 3b: Print containing function's CFG
        Function *Func = L->getHeader()->getParent();
        errs() << "CFG for function '" << Func->getName() << "':\n";
        
        Func->print(errs());
        

        // 3c: Print all loop blocks
        errs() << "Blocks in loop " << L->getHeader()->getName() << ":\n";
        for (BasicBlock *BB : L->blocks()) {
            errs() << "  " << BB->getName() << "\n";
        }
        errs() << "\n";
    }

    return PreservedAnalyses::all();
}

} // namespace assignment3