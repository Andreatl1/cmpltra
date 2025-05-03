# Esecuzione automatica
Esecuzione automatica tramite l'utilizzo di uno script:
```bash
cd assignment3
chmod +x script.sh
./script.sh
```
**Segui le istruzioni interattive**, lo script ti guiderà passo dopo passo:
1. Inserisci il nome del file da ottimizzare (senza estensione)
2. Scegli se il file è un file C o LLVM IR (.ll)
3. Seleziona i passi di ottimizzazione da applicare (puoi scegliere tra: **loop-pass**, **custom-LICM-pass** oppure "**tutti**")

Lo script compila il progetto e applica le ottimizzazioni al file scelto, alla fine, otterrai un file ottimizzato che si troverà nella cartella **test/** con estensione **.optimized.ll**


# Esecuzione manuale

### 1. Spostiamoci nella cartella dell'assignment
```bash
cd assignment3
```
### 2. Impostare le variabili di ambiente:
```bash
export ROOT_LABS=$(pwd)
export LLVM_DIR=$(llvm-config --libdir)
export TEST_FILE="nome_file" #Nome file .ll o .c da ottimizzare, senza estensione
```
### 3. Creare la directory di compilazione
```bash
mkdir -p ${ROOT_LABS}/build
```

### 4. Compilazione del progetto
```bash
cd ${ROOT_LABS}/build
cmake -DLT_LLVM_INSTALL_DIR="${LLVM_DIR}" "${ROOT_LABS}"
make
```

### 5. Generazione del file LLVM IR (se necessario)
Se il file da ottimizzare è in C, esegui i seguenti comandi per generare il file LLVM IR:
```bash
clang -Xclang -disable-O0-optnone -O0 -S -emit-llvm -c ${ROOT_LABS}/test/${TEST_FILE}.c -o ${ROOT_LABS}/test/${TEST_FILE}.ll
opt -p mem2reg ${ROOT_LABS}/test/${TEST_FILE}.ll -o ${ROOT_LABS}/test/${TEST_FILE}.bc
llvm-dis ${ROOT_LABS}/test/${TEST_FILE}.bc -o ${ROOT_LABS}/test/${TEST_FILE}.ll
```

### 6. Applicare i passi di ottimizzazione
Scegli i passi di ottimizzazione da applicare (**loop-pass**, **custom-LICM-pass** oppure "**tutti**") e applicali al file:
```bash
opt -load-pass-plugin ${ROOT_LABS}/build/libAssignment3.so \
  -passes="custom-LICM-pass" \
  ${ROOT_LABS}/test/${TEST_FILE}.ll -o ${ROOT_LABS}/test/${TEST_FILE}.optimized.bc
llvm-dis ${ROOT_LABS}/test/${TEST_FILE}.optimized.bc -o ${ROOT_LABS}/test/${TEST_FILE}.optimized.ll
```
### 7. Verifica il risultato
Alla fine, dovresti trovare il file ottimizzato con estensione **.optimized.ll** nella directory **test/**
