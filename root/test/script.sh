mkdir root
cd root
export ROOT_LABS=${pwd}/root/
export LLVM_DIR=/usr/lib/llvm/bin



cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR #all'interno di root
make
cd test
#Lanciamo opt dalla cartella test/. Essendo un passo di trasformazione la IR verrà modificata, e quindi dobbiamo specificare l’output file
opt -load-pass-plugin ../libLocalOpts.so -p local-opts Foo.ll -o Foo.optimized.bc
#• L’output è in formato bytecode, quindi dobbiamo usare il disassembler per generare la forma .ll leggibile
llvm-dis Foo.optimized.bc -o Foo.optimized.ll


