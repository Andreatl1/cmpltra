#include "assignment3.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/Analysis/ValueTracking.h"

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

static bool checkOperands(Instruction &I, Loop *L, DominatorTree &DT, SmallPtrSet<Instruction*, 8> &Visited) {
    SmallVector<Instruction*, 8> Worklist;
    Worklist.push_back(&I);
    Visited.insert(&I);

    while (!Worklist.empty()) {
        Instruction *Current = Worklist.pop_back_val();

        if (PHINode *PN = dyn_cast<PHINode>(Current)) {
            if (L->contains(PN->getParent())) return false;
        }

        for (Value *Op : Current->operands()) {
            if (Instruction *OpInst = dyn_cast<Instruction>(Op)) {
                if (L->contains(OpInst->getParent())) {
                    if (!Visited.insert(OpInst).second) continue;
                    Worklist.push_back(OpInst);
                } else {
                    // Se l'istruzione è fuori dal loop ma non domina l’header, allora non è sicura
                    if (!DT.dominates(OpInst->getParent(), L->getHeader())) {
                        return false;
                    }
                }
            } else if (!isa<Constant>(Op) && !isa<Argument>(Op)) {
                return false;
            }
        }
    }
    return true;
}

static bool hoistInvariants(Loop *L, DominatorTree &DT, TargetLibraryInfo *TLI) {
    BasicBlock *Preheader = L->getLoopPreheader();
    if (!Preheader) return false;

    bool Changed = false;
    SmallVector<Instruction*, 16> ToHoist;
    SmallPtrSet<Instruction*, 8> Visited;

    for (BasicBlock *BB : L->blocks()) {
        for (Instruction &I : *BB) {
            if (isa<PHINode>(I) || I.isTerminator() || I.mayReadOrWriteMemory() || !isSafeToSpeculativelyExecute(&I, nullptr, nullptr, &DT,true)) continue;


            Visited.clear();
            if (checkOperands(I, L, DT, Visited)) {
                ToHoist.push_back(&I);
            }
        }
    }
    // Per ogni istruzione candidata, verifichiamo che tutti i suoi usi siano dominati dal preheader
    for (Instruction *I : ToHoist) {
        bool Safe = true;
        
        for (User *U : I->users()) {
            if (Instruction *UI = dyn_cast<Instruction>(U)) {
                if (!DT.dominates(Preheader, UI->getParent())) {
                    Safe = false;
                    break;
                }
            }
        }
        
        if (Safe) {
            I->moveBefore(Preheader->getTerminator());  // Spostiamo l’istruzione nel preheader
            Changed = true;
        }
    }

    return Changed;
}

PreservedAnalyses CustomLICMPass::run(Function &F, FunctionAnalysisManager &FAM) {
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
    DominatorTree &DT = FAM.getResult<DominatorTreeAnalysis>(F);
    TargetLibraryInfo &TLI = FAM.getResult<TargetLibraryAnalysis>(F);
    bool Changed = false;

    for (Loop *L : LI.getLoopsInPreorder()) {
        if (!L->isLoopSimplifyForm()) continue;
        
        if (hoistInvariants(L, DT, &TLI)) {
            Changed = true;
        }
    }

    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

} // namespace assignment3