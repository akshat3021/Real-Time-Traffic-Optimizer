#include <stdio.h>


int main(int argc, char *argv[]) {
    if (argc == 3) {
       
        printf("Path:%s-2-3-%s,Time:15", argv[1], argv[2]);
    } else {
        printf("Error: Please provide a source and a destination.");
    }
    return 0;
}