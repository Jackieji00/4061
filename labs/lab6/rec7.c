#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void func2(char* input) {
    char buffer[100];
    strcpy(buffer, input);
    printf("%s\n", buffer);
}

void func1(char* input) {
    char *longstr = (char *) malloc(1000);
    char shortstr[20];
    strcpy(longstr, input);
    strcpy(shortstr, "Small String");
    func2(longstr);
    free(longstr);
    func2(shortstr);
}

int main() {
    char inp[1000];
    printf("Input string: ");
    fgets(inp, 1000, stdin);
    while (1) {
        func1(inp);
    }
    return 0;
}
