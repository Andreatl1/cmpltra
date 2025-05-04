
int licm_test(int a, int b) {
    int sum = 0;
    
    for (int i = 0; i < 1000; i++) {
        int x = a * b;   // Istruzione Loop Invariant 
        int y = x + 10;  // Istruzione Loop Invariant
        
        for (int j = 0; j < 100; j++) {
            sum += y * j;  
        }
        

    }
    
    return sum;
}

int main() {
    return licm_test(5, 3);
}