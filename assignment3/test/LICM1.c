int compute(int a, int b) {
    int sum = 0;
    

    for(int i = 0; i < 100; i++) {
        int invariant1 = a + b;
        sum += invariant1 * i;
        
        
        for(int j = 0; j < 50; j++) {
            int invariant2 = a * b;
            sum += invariant2 + j;
        }
    }
    
    return sum;
}

int main() {
    return compute(5, 3);
}