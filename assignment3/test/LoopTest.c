int g = 0;

// Loop sequenziale semplice
void sequential_loops(int a, int b) {
    // Primo loop
    for(int i = 0; i < a; i++) { g++; }
    
    // Secondo loop
    for(int j = 0; j < b; j++) { g--; }
}

void nested_loop(int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            g += i * j;
        }
    }
}

void complex_loops(int x) {
    // Loop esterno
    for(int a = 0; a < x; a++) {
        // Primo loop interno
        for(int b = 0; b < 5; b++) {
            g += b;
        }
        
        // Secondo loop interno
        for(int c = 0; c < 10; c++) {
            g -= c;
        }
    }
}

int main() {
    sequential_loops(3, 5);
    nested_loop(4);
    complex_loops(2);
    return g;
}