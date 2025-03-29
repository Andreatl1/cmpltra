//=============================================================================
// FILE:
//    LocalOpts.cpp
//
// DESCRIPTION:
//    Visits all functions in a module and prints their names. Strictly speaking,
//    this is an analysis pass (i.e. //    the functions are not modified). However,
//    in order to keep things simple there's no 'print' method here (every analysis
//    pass should implement it).
//
// USAGE:
//    New PM
//      opt -load-pass-plugin=<path-to>libTestPass.so -passes="test-pass" `\`
//        -disable-output <input-llvm-file>
//
//
// License: MIT
//=============================================================================
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"

using namespace llvm;

//-----------------------------------------------------------------------------
// LocalOpts implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
namespace
{

  /*
  il metodo run prende in ingresso un handle ad un oggetto Function. Potete passare lo stesso handle al metodo runOnFunctiontrovato in LocalOpts_skeleton.cpp. Rinominate anche il flag di attivazione del passo da test-pass in local-opts
  */

  // New PM implementation
  struct LocalOpts : PassInfoMixin<LocalOpts>
  {
    bool runOnBasicBlock(BasicBlock &B)
    {

      // Preleviamo le prime due istruzioni del BB
      Instruction &Inst1st = *B.begin(), &Inst2nd = *(++B.begin());

      // L'indirizzo della prima istruzione deve essere uguale a quello del
      // primo operando della seconda istruzione (per costruzione dell'esempio)
      assert(&Inst1st == Inst2nd.getOperand(0));

      // Stampa la prima istruzione
      outs() << "PRIMA ISTRUZIONE: " << Inst1st << "\n";
      // Stampa la prima istruzione come operando
      outs() << "COME OPERANDO: ";
      Inst1st.printAsOperand(outs(), false);
      outs() << "\n";

      // User-->Use-->Value
      outs() << "I MIEI OPERANDI SONO:\n";
      for (auto *Iter = Inst1st.op_begin(); Iter != Inst1st.op_end(); ++Iter)
      {
        Value *Operand = *Iter;

        if (Argument *Arg = dyn_cast<Argument>(Operand))
        {
          outs() << "\t" << *Arg << ": SONO L'ARGOMENTO N. " << Arg->getArgNo()
                 << " DELLA FUNZIONE " << Arg->getParent()->getName()
                 << "\n";
        }
        if (ConstantInt *C = dyn_cast<ConstantInt>(Operand))
        {
          outs() << "\t" << *C << ": SONO UNA COSTANTE INTERA DI VALORE " << C->getValue()
                 << "\n";
        }
      }

      outs() << "LA LISTA DEI MIEI USERS:\n";
      for (auto Iter = Inst1st.user_begin(); Iter != Inst1st.user_end(); ++Iter)
      {
        outs() << "\t" << *(dyn_cast<Instruction>(*Iter)) << "\n";
      }

      outs() << "E DEI MIEI USI (CHE E' LA STESSA):\n";
      for (auto Iter = Inst1st.use_begin(); Iter != Inst1st.use_end(); ++Iter)
      {
        outs() << "\t" << *(dyn_cast<Instruction>(Iter->getUser())) << "\n";
      }

      // Si possono aggiornare le singole references separatamente?
      // Controlla la documentazione e prova a rispondere.

      // Inst1st.replaceAllUsesWith(NewInst);

      /*
      for (auto Iter = Inst1st.use_begin(); Iter != Inst1st.use_end(); ++Iter)
      {

        Instruction *user = dyn_cast<Instruction>(Iter->getUser());
        user->replaceUsesOfWith(&Inst1st, NewInst);
      }*/

      /*
      Per esercitarci a manipolare la IR modifichiamo adesso il
      passo LocalOpts.cpp perché sostituisca tutte le operazioni di
      moltiplicazione che hanno tra gli operandi una costante che
      è una potenza di 2 con una shift (strength reduction)

      NOTA: Non rimuoviamo la mul, ma la slinkiamo dal programma
      rimpiazzando i suoi usi con gli usi della nuova istruzione.

      */

      bool Transformed = false;
      SmallVector<Instruction *, 16> MulInsts;

      // Fase 1: Raccogli tutte le istruzioni di moltiplicazione
      for (auto &I : B)
      {
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I))
        {
          if (BinOp->getOpcode() == Instruction::Mul)
          {
            MulInsts.push_back(BinOp);
          }
        }
      }

      // Fase 2: Processa ogni moltiplicazione
      for (auto *MulInst : MulInsts)
      {
        for (unsigned i = 0; i < 2; ++i)
        {
          Value *Op = MulInst->getOperand(i);
          if (ConstantInt *C = dyn_cast<ConstantInt>(Op))
          {
            APInt Val = C->getValue();

            // Verifica se la costante è una potenza di due positiva
            if (Val.isPowerOf2())
            {
              int ShiftAmt = Val.exactLogBase2();
              Value *OtherOp = MulInst->getOperand(1 - i);

    
              // Sostituisci la moltiplicazione con uno shift left
              Instruction *Shl =
                  BinaryOperator::CreateShl(OtherOp, ConstantInt::get(OtherOp->getType(), ShiftAmt));
              Shl->insertAfter(MulInst);       // Posiziona dopo la moltiplicazione
              MulInst->replaceAllUsesWith(Shl); // Rimpiazza tutti gli usi

              Transformed = true;
              break; // Gestisci solo un operando per moltiplicazione
            }
          }
        }
      }


      return Transformed;
    }

    bool runOnFunction(Function &F)
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
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &)
    {
      if (!runOnFunction(F))
        return PreservedAnalyses::all();

      return PreservedAnalyses::none();
    }

    // Without isRequired returning true, this pass will be skipped for functions
    // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
    // all functions with optnone.
    static bool isRequired() { return true; }
  };
} // namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getTestPassPluginInfo()
{
  return {LLVM_PLUGIN_API_VERSION, "LocalOpts", LLVM_VERSION_STRING,
          [](PassBuilder &PB)
          {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>)
                {
                  if (Name == "local-opts")
                  {
                    FPM.addPass(LocalOpts());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize TestPass when added to the pass pipeline on the
// command line, i.e. via '-passes=test-pass'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo()
{
  return getTestPassPluginInfo();
}
