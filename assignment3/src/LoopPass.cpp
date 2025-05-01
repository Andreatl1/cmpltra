#include "assignment3.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/BreadthFirstIterator.h"

using namespace llvm;

namespace assignment3 {

void printDomTree(const DomTreeNode *N, raw_ostream &OS, unsigned Indent) {
  OS.indent(Indent * 2);
  if (N->getBlock()) {
    OS << N->getBlock()->getName() << "\n";
  } else {
    OS << "<<null block>>\n";
  }
  for (const auto &Child : N->children()) {
    printDomTree(Child, OS, Indent + 2);
  }
}

PreservedAnalyses LoopPass::run(Function &F, FunctionAnalysisManager &FAM) {
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
    
    // Step 1: Check if CFG contains loops
    //if (LI.empty()) 
    //    return PreservedAnalyses::all();

    // Step 2: Print loop headers
    errs() << "Loop headers in function '" << F.getName() << "':\n";
    for (const BasicBlock &BB : F) {
        if (LI.isLoopHeader(&BB)) {
            errs() << "  " << BB.getName() << "\n";
        }
    }

    // Step 3: Process loops
    errs() << "\nProcessing loops in function '" << F.getName() << "':\n";
    for (Loop *L : LI.getLoopsInPreorder()) {
        bool isNormal = L->isLoopSimplifyForm();
        errs() << "Loop with header " << L->getHeader()->getName()
               << " is in normal form: " << (isNormal ? "Yes" : "No") << "\n";

        Function *Func = L->getHeader()->getParent();
        errs() << "CFG for function '" << Func->getName() << "':\n";
        Func->print(errs());

        errs() << "Blocks in loop " << L->getHeader()->getName() << ":\n";
        for (BasicBlock *BB : L->blocks()) {
            errs() << "  " << BB->getName() << "\n";
        }
        errs() << "\n";

    }


    // Dominance Tree Analysis
    DominatorTree &DT = FAM.getResult<DominatorTreeAnalysis>(F);
    errs() << "\nDominance Tree for function '" << F.getName() << "':\n";
    printDomTree(DT.getRootNode(), errs(), 0);

    return PreservedAnalyses::all();
}

} // namespace assignment3