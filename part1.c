#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1], "rb"); 
    unsigned char bytes[2]; //Each byte consists of two hexadecimal digits. 2x2--45;00
    while(fread(bytes, 1, 2, file)){ 
        printf("%#04x%02x\n", bytes[0], bytes[1]);
    }
    fclose(file);
    return 0;
}
