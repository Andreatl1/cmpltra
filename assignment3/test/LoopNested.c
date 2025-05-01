int g = 0;


void nested_loop(int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            g += i * j;
        }
    }
}


int main() {
    nested_loop(4);
    return g;
}