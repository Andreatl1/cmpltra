//=============================================================================
// FILE:
//    StrengthReduction.cpp
//
// DESCRIPTION:
//  Implementare il seguente passo LLVM che realizza la seguente ottimizzazione:
//  2. Strength Reduction (più avanzato)
//  15 * X = X * 15 -> (x << 4) – X
//  y = X / 8 -> y = X >> 3
//
//
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

//-----------------------------------------------------------------------------
// StrengthReduction implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
namespace assignment1
{

    bool StrengthReduction::runOnBasicBlock(BasicBlock &B)
    {
        bool Transformed = false;                    // Flag per tracciare modifiche al BB
        SmallVector<BinaryOperator *, 16> ToReplace; // Contiene le istruzioni candidate, in modo che prima capiamo le istruzioni da modificare e solo dopo andiamo a modificarle, garantendo una corretta iterazione del blocco

        // 1: Scansiona tutte le istruzioni nel BB
        for (auto &I : B)
        {
            // Verifica se l'istruzione è un'operazione binaria
            if (auto *BinOp = dyn_cast<BinaryOperator>(&I))
            {
                // Salva le operazioni MUL/DIV per elaborazione successiva
                ToReplace.push_back(BinOp);
            }
        }

        // 2: Elabora le istruzioni raccolte
        for (auto *BinOp : ToReplace)
        {
            switch (BinOp->getOpcode())
            {                      // Controlla il tipo di operazione
            case Instruction::Mul: // Moltiplicazione
                Transformed |= handleMultiplication(BinOp);
                break;
            case Instruction::SDiv: // Divisione con segno
            case Instruction::UDiv: // Divisione senza segno
                Transformed |= handleDivision(BinOp);
                break;
            }
        }

        return Transformed; // true se il blocco è stato modificato
    }

    bool StrengthReduction::handleMultiplication(BinaryOperator *Mul)
    {
        Value *Op0 = Mul->getOperand(0); // Primo operando
        Value *Op1 = Mul->getOperand(1); // Secondo operando
        ConstantInt *CI = nullptr;       // Puntatore a costante intera

        // Verifica la presenza di una costante in uno dei due operandi
        if (!(CI = dyn_cast<ConstantInt>(Op0))) // Controlla Op0
            CI = dyn_cast<ConstantInt>(Op1);    // Altrimenti Op1

        if (!CI)
            return false; // Nessuna costante trovata -> esci

        APInt Val = CI->getValue();

        if (Val.isNonNegative())
        {
            int ShiftAmt = Val.nearestLogBase2(); // Trova la potenza di due più vicina

            if (ShiftAmt < 0)
                return false;
            APInt NearestPow = APInt(Val.getBitWidth(), 1) << ShiftAmt; // Crea un APInt con la stesso numero di bit di Val ed il primo bit è settato ad 1, inseguito lo shifta a sx di ShiftAmt
            APInt Diff = Val - NearestPow;                              // 18-16 = 2

            Value *OtherOp = (CI == Op0) ? Op1 : Op0; // Seleziona l'altro operando

            // Crea istruzione SHL (shift left)
            Instruction *Shl = BinaryOperator::CreateShl(
                OtherOp,                                        // Operando da shiftare
                ConstantInt::get(OtherOp->getType(), ShiftAmt), // Quantità shift
                "shl",
                Mul // Posiziona prima della mul
            );

            // Aggiustamenti per differenze positive/negative
            Value *Current = Shl;
            for (int i = 0; i < Diff.abs().getZExtValue(); ++i)
            { // getZExtValue() fornisce una conversione senza segno sicura ad uint64_t
                Instruction *Adjustment;
                if (Diff.isNegative())
                {
                    Adjustment = BinaryOperator::CreateSub(
                        Current,
                        OtherOp,
                        "sub");
                }
                else
                {
                    Adjustment = BinaryOperator::CreateAdd(
                        Current,
                        OtherOp,
                        "add");
                }
                // Inserisci l'istruzione nel BasicBlock prima della moltiplicazione
                Adjustment->insertBefore(Mul);
                Current = Adjustment; // Aggiorna il valore corrente
            }

            // Sostituisci tutti gli usi della moltiplicazione con il risultato finale
            Mul->replaceAllUsesWith(Current); // Aggiorna tutte le dipendenze

            return true; // modifica effettuata
        }
        return false; // nessuna modifica
    }

    /*
     * Funzione che trasforma un'istruzione di divisione in un'istruzione di shift a destra,
     * se il secondo operando è una costante intera ed è una potenza di due precisa.
     */
    bool StrengthReduction::handleDivision(BinaryOperator *Div)
    {
        Value *Divisor = Div->getOperand(1); // Divisore
        ConstantInt *CI = dyn_cast<ConstantInt>(Divisor);

        // Verifica se il divisore è una potenza di 2
        if (CI && CI->getValue().isPowerOf2())
        {
            int ShiftAmt = CI->getValue().exactLogBase2(); // Calcola log2
            Value *Dividend = Div->getOperand(0);          // Dividendo

            // Scegli tipo di shift in base alla divisione
            Instruction::BinaryOps Op =
                (Div->getOpcode() == Instruction::SDiv) ? Instruction::AShr : // Shift aritmetico per divisioni signed
                    Instruction::LShr;                                        // Shift logico per divisioni unsigned

            // Crea istruzione di shift
            Instruction *Shr = BinaryOperator::Create(
                Op,                                              // Tipo di operazione (AShr/LShr)
                Dividend,                                        // Valore da shiftare
                ConstantInt::get(Dividend->getType(), ShiftAmt), // Quantità shift
                "shr",
                Div // Posiziona prima della divisione
            );

            // Sostituisci la divisione
            Div->replaceAllUsesWith(Shr); // Aggiorna tutte le dipendenze

            return true; // modifica effettuata
        }
        return false; // nessuna modifica
    }

    bool StrengthReduction::runOnFunction(Function &F)
    {
        bool Transformed = false;

        // Itera su tutti i BasicBlock della funzione
        for (auto Iter = F.begin(); Iter != F.end(); ++Iter)
        {
            // Aggiorna il flag se almeno un BB è modificato
            if (runOnBasicBlock(*Iter))
            {
                Transformed = true;
            }
        }

        return Transformed; // True se funzione modificata
    }

    // Main entry point, takes IR unit to run the pass on (&F) and the
    // corresponding pass manager (to be queried if need be)
    PreservedAnalyses StrengthReduction::run(Function &F, FunctionAnalysisManager &)
    {
        // Esegui l'ottimizzazione e restituisci lo stato delle analisi
        return runOnFunction(F) ? PreservedAnalyses::none() : // modifiche effettuate
                   PreservedAnalyses::all();                  // analisi preservate
    }

    // Without isRequired returning true, this pass will be skipped for functions
    // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
    // all functions with optnone.
    static bool isRequired() { return true; }
}