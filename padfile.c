#include <stdio.h>
#include <stdint.h>

union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

int main(int argc,char *argv[]) {
    
    union msgblock M;  
    uint64_t nobytes;
    
    FILE* f;
    //Open the file
    
    f = fopen(argv[1],"r");
    
    while (!feof(f)) {
        // get file size
        nobytes = fread(M.e,1, 64, f);
        if(nobytes < 56) {
            printf("I've found a with less than 55 bytes!\n");
            M.e[nobytes] = 0x01;
        } 
        
        
        printf("%llu\n",nobytes);
        
    }
    
    fclose(f);
    
    return 0;
}