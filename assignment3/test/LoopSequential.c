int g = 0;

void sequential_loops(int a, int b) {

    for(int i = 0; i < a; i++) { g++; }

    for(int j = 0; j < b; j++) { g--; }
}


int main() {
    sequential_loops(3, 5);
    return g;
}