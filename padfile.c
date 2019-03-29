#include <stdio.h>
#include <stdint.h>

union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

int main(int argc,char *argv[]) {
    
    union msgblock M;  
    
    uint64_t nobits = 0;
    
    uint64_t nobytes;
    
    FILE* f;
    //Open the file
    f = fopen(argv[1],"r");
    
    //feof()是函数，用来检测文件的结束；
    while (!feof(f)) {
        // get file size
        nobytes = fread(M.e, 1, 64, f);
        printf("Ready %2llu \n bytes",nobytes);
        nobits = nobits + (nobytes * 8);
        if(nobytes < 56) {
            printf("I've found a with less than 55 bytes!\n");
            M.e[nobytes] = 0x80;
            while(nobytes < 56) {
                nobytes = nobytes + 1;
                M.e[nobytes] = 0x00;
            }
            M.s[7] = nobits;
        } 
    }
    
    fclose(f);
    
    for(int i = 0; i < 64; i++)
        printf("%x ",M.e[i]);
    printf("\n");
    
    return 0;
}