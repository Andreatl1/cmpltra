#include <stdio.h>

void f()
{

    int x = 0;

    // A
    x = 1;
    if (x > 0)
    {
        // B
        x += 1;
    }
    else
    {
        // C
        x += 2;
        if (x % 2 == 0)
        {
            // D

            x += 3;
        }
        else
        {
            // E
            x += 4;
        }
        // F
        x += 5;
    }
    // G
    x += 10;
}

int main()
{
    f();
    return 0;
}
