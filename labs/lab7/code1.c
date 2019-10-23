#include <stdlib.h>
#include <stdint.h>

int main() {
    int i;
    char* alphabet = malloc(26*sizeof(char));
    char* destination=malloc(25*sizeof(char));

    for(i = 0; i < 26; i++) {
        *(alphabet + i) = 'A' + i;
    }
    *(alphabet + 26) = '\0';

    for(i=0; i<26; i++){
      destination[i]=alphabet[i];
    }

    return 0;
}
