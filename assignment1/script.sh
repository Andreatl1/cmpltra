export ROOT_LABS=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd ) #serve per ottenere il path assoluto di dove si trova lo script
export LLVM_DIR=$(llvm-config --libdir)

mkdir -p build 
BUILD_DIR=${ROOT_LABS}"/build"
TEST_DIR=${ROOT_LABS}"/test"
SRC_DIR=${ROOT_LABS}"/src"

ASSIGNMENT=Assignment1 #nome del file .so che contiene i passi
TEST_FILE=strength_reduction
TEST_FILE_EXTENSION=c
TEST_FILE_PATH="${TEST_DIR}/${TEST_FILE}" 
C_EXEC=false # this need to be changed to true if you want to run the C code 

echo "ROOT_LABS: ${ROOT_LABS}";
echo "LLVM_DIR: ${LLVM_DIR}";

cd ${BUILD_DIR}
cmake -DLT_LLVM_INSTALL_DIR="${LLVM_DIR}" "${ROOT_LABS}" #all'interno di assignment1/
make
cd ..


if [ ${C_EXEC} == true ]
then

    # Running clang with optnone flag disabled for .ll generation
    clang -Xclang -disable-O0-optnone -O0 -S -emit-llvm -c "${TEST_FILE_PATH}.${TEST_FILE_EXTENSION}" -o "${TEST_FILE_PATH}.ll"

    # Converting LLVM IR to SSA form using the mem2reg pass
    opt -p mem2reg "${TEST_FILE_PATH}.ll" -o "${TEST_FILE_PATH}.bc"

    # Running llvm-dis to convert .bc to .ll
    llvm-dis "${TEST_FILE_PATH}.bc" -o "${TEST_FILE_PATH}.ll"
fi


execute_passes() {
    # Running opt to apply passes
    opt -load-pass-plugin "${BUILD_DIR}/lib${ASSIGNMENT}.so" \
        -passes="$1" "${TEST_FILE_PATH}.ll" \
        -o "${TEST_FILE_PATH}.optimized.bc"
    
    # Running llvm-dis to convert .bc to .ll
    llvm-dis "${TEST_FILE_PATH}.optimized.bc" -o "${TEST_FILE_PATH}.optimized.ll"
}

echo "Running opt to apply pass..."; echo

#execute_passes "algebraic-identity"
execute_passes "strength-reduction"
#execute_passes "multi-instruction-optimization"
#execute_passes "algebraic-identity,multi-instruction-optimization,strength-reduction"
