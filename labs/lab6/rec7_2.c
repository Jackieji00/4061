#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char DEFAULT_NAME[10] = "John Doe";

void run(int input) {
    char * name = (char *) malloc(20);
    char namebuf[20];
    char numbuf[3];
    if (input % 10 == 0) {
        strcpy(name, DEFAULT_NAME);
    }
    else {
        strcpy(namebuf, "This is number: ");
        sprintf(numbuf, "%d", input);
        strcat(namebuf, numbuf);
        strcpy(name, namebuf);
    }
    if (input % 3 == 0) {
        printf("Name is: %s\n", name);
    }
    else if (input % 2 == 0) {
        printf("Input is even!");
        free(name);
        return;
    }
}

int main() {
    int i = 0;
    while(1) {
        run(i);
        i++;
    }

}
