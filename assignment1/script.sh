#!/bin/bash

export ROOT_LABS=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export LLVM_DIR=$(llvm-config --libdir)

# Configurazione iniziale
BUILD_DIR="${ROOT_LABS}/build"
TEST_DIR="${ROOT_LABS}/test"
SRC_DIR="${ROOT_LABS}/src"
ASSIGNMENT="Assignment1"

rm -f ${TEST_DIR}/*.bc
rm -f ${TEST_DIR}/*.optimized*

# Funzione per la selezione interattiva
select_with_default() {
    local prompt="$1"
    local -n options="$2"
    local default="$3"

    PS3="$prompt "
    select opt in "${options[@]}"; do
        if [[ -n $opt ]]; then
            echo "$opt"
            break
        elif [[ -z $opt && -n $default ]]; then
            echo "$default"
            break
        else
            echo "Opzione non valida"
        fi
    done
}

# Input interattivo
read -p "Inserisci il nome del file di test (senza estensione): " TEST_FILE
read -p "È un file C? (y/n): " IS_C

# Imposta il flag C_EXEC
if [[ $IS_C =~ ^[Yy]$ ]]; then
    C_EXEC=true
    TEST_FILE_EXTENSION="c"
else
    C_EXEC=false
    TEST_FILE_EXTENSION="ll"
fi

# Verifica esistenza file
TEST_FILE_PATH="${TEST_DIR}/${TEST_FILE}"
if [ ! -f "${TEST_FILE_PATH}.${TEST_FILE_EXTENSION}" ]; then
    echo "File non trovato: ${TEST_FILE_PATH}.${TEST_FILE_EXTENSION}"
    exit 1
fi

# Selezione passi
PASS_OPTIONS=("algebraic-identity" "strength-reduction" "multi-instruction-optimization" "tutti")
echo "Seleziona i passi da applicare:"
selected_pass=$(select_with_default "Scegli un'opzione (default: tutti):" PASS_OPTIONS "tutti")

case $selected_pass in
    "tutti") PASSES="algebraic-identity,strength-reduction,multi-instruction-optimization" ;;
    *) PASSES="$selected_pass" ;;
esac

# Stampa configurazione
echo -e "\nConfigurazione:"
echo "----------------------------------------"
echo "File di test:      ${TEST_FILE}.${TEST_FILE_EXTENSION}"
echo "Tipo file:         $([ $C_EXEC == true ] && echo "C" || echo "LLVM IR")"
echo "Passi selezionati: ${PASSES}"
echo "----------------------------------------"

# Creazione directory build
mkdir -p ${BUILD_DIR}

echo -e "\nCompilazione...\n"
cd ${BUILD_DIR}
cmake -DLT_LLVM_INSTALL_DIR="${LLVM_DIR}" "${ROOT_LABS}"
make
cd ..

# Generazione IR se necessario
if [ ${C_EXEC} == true ]; then
    echo -e "\nGenerazione IR...\n"
    clang -Xclang -disable-O0-optnone -O0 -S -emit-llvm -c "${TEST_FILE_PATH}.${TEST_FILE_EXTENSION}" -o "${TEST_FILE_PATH}.ll"
    opt -p mem2reg "${TEST_FILE_PATH}.ll" -o "${TEST_FILE_PATH}.bc"
    llvm-dis "${TEST_FILE_PATH}.bc" -o "${TEST_FILE_PATH}.ll"
fi

# Funzione per l'esecuzione dei passi
execute_passes() {
    echo -e "\nApplicazione passi...\n"
    opt -load-pass-plugin "${BUILD_DIR}/lib${ASSIGNMENT}.so" \
        -passes="$1" "${TEST_FILE_PATH}.ll" \
        -o "${TEST_FILE_PATH}.optimized.bc"
    
    llvm-dis "${TEST_FILE_PATH}.optimized.bc" -o "${TEST_FILE_PATH}.optimized.ll"
    echo -e "\nOttimizzazione completata!"
    echo "File ottimizzato: ${TEST_FILE_PATH}.optimized.ll"
}

# Esecuzione
execute_passes "${PASSES}"