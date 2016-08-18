#include <stdio.h>

int main() {
    int c;
    while((c = getchar()) != EOF) { 
        putchar(c);
    }    
    printf("%d  at EOF\n", c);
}

