int g = 0;

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

    complex_loops(2);
    return g;
}