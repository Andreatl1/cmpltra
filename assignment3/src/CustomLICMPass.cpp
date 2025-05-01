#include "assignment3.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/DepthFirstIterator.h"

using namespace llvm;

namespace assignment3 {

void printDomTree1(const DomTreeNode *N, raw_ostream &OS, unsigned Indent) {
    OS.indent(Indent * 2);
    if (N->getBlock()) {
        OS << N->getBlock()->getName() << "\n";
    } else {
        OS << "<<null block>>\n";
    }
    for (const auto &Child : N->children()) {
        printDomTree1(Child, OS, Indent + 2);
    }
}

static bool isSafeToHoist(Instruction &I, Loop *L, DominatorTree &DT) {
    SmallVector<BasicBlock*, 4> ExitBlocks;
    L->getExitBlocks(ExitBlocks);
    
    for (BasicBlock *Exit : ExitBlocks) {
        if (!DT.dominates(I.getParent(), Exit)) return false;
    }
    return true;
}


static bool checkOperands(Instruction &I, Loop *L, DominatorTree &DT, SmallPtrSet<Instruction*, 8> &Visited) {
    if (Visited.contains(&I)) return true;
    Visited.insert(&I);

    // Blocca le istruzioni che usano PHI del loop corrente
    if (PHINode *PN = dyn_cast<PHINode>(&I)) {
        if (L->contains(PN->getParent())) return false;
    }

    for (Value *Op : I.operands()) {
        if (Instruction *OpInst = dyn_cast<Instruction>(Op)) {
            if (L->contains(OpInst->getParent())) {
                if (!checkOperands(*OpInst, L, DT, Visited)) return false;
            } else {
                // Verifica che l'operando esterno domini il preheader
                if (!DT.dominates(OpInst->getParent(), L->getHeader())) {
                    return false;
                }
            }
        } else if (!isa<Constant>(Op) && !isa<Argument>(Op)) {
            return false;
        }
    }
    return true;
}

static bool hoistInvariants(Loop *L, DominatorTree &DT) {
    BasicBlock *Preheader = L->getLoopPreheader();
    if (!Preheader) return false;

    bool Changed = false;
    SmallVector<Instruction*, 16> ToHoist;
    SmallPtrSet<Instruction*, 8> Visited;

    for (BasicBlock *BB : L->blocks()) {
        for (Instruction &I : *BB) {
            if (isa<PHINode>(I) || I.isTerminator()) continue;
            if (I.mayReadOrWriteMemory()) continue;

            Visited.clear();
            if (checkOperands(I, L, DT, Visited)) {
                ToHoist.push_back(&I);
            }
        }
    }

    for (Instruction *I : reverse(ToHoist)) {
        bool Safe = true;
        BasicBlock *Parent = I->getParent();
        
        // Verifica che tutti gli users siano dominati dal preheader
        for (User *U : I->users()) {
            if (Instruction *UI = dyn_cast<Instruction>(U)) {
                if (!DT.dominates(Preheader, UI->getParent())) {
                    Safe = false;
                    break;
                }
            }
        }
        
        // Verifica che l'istruzione non sia in un blocco con PHI
        if (Safe && !isa<PHINode>(Parent->front())) {
            I->moveBefore(Preheader->getTerminator());
            Changed = true;
        }
    }

    return Changed;
}

PreservedAnalyses CustomLICMPass::run(Function &F, FunctionAnalysisManager &FAM) {
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
    DominatorTree &DT = FAM.getResult<DominatorTreeAnalysis>(F);
    bool Changed = false;

    errs() << "Analyzing function: " << F.getName() << "\n";
    for (const BasicBlock &BB : F) {
        if (LI.isLoopHeader(&BB)) {
            errs() << " - Loop header: " << BB.getName() << "\n";
        }
    }

    for (Loop *L : LI.getLoopsInPreorder()) {
        errs() << "\nProcessing loop: " << L->getHeader()->getName() 
               << " (Normal form: " << (L->isLoopSimplifyForm() ? "Yes" : "No") << ")\n";
        
        if (hoistInvariants(L, DT)) {
            Changed = true;
            errs() << "Hoisted instructions!\n";
        }
    }

    errs() << "\nFinal Dominance Tree:\n";
    printDomTree1(DT.getRootNode(), errs(), 0);

    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

} // namespace assignment3