int main() {
    int i = 0;
    int x = 10;
    int y = 0;
    int a = x * 2; 

    while (i < 5) {
        int z = a + 1; // Istruzione Loop Invariant
        y += z;
        i++;
    }

    return 0;
}
