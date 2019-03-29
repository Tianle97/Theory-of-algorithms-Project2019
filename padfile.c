#include <stdio.h>
#include <stdint.h>

union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

enum status {
    READ,
    PAD0,
    PAD1,
    FINISH  
};

int main(int argc,char *argv[]) {
    
    union msgblock M;  
    
    uint64_t nobits = 0;
    
    uint64_t nobytes;
    
    FILE* f;
    //Open the file and it is a only read model
    //https://blog.csdn.net/libing403/article/details/73136244
    f = fopen(argv[1],"r");
    
    enum status s = READ;
    
    int i;
    
    //feof()是函数，用来检测文件的结束；
    while (s == READ) {
        // get file size
        nobytes = fread(M.e, 1, 64, f);
        printf("Ready %2llu \n bytes",nobytes);
        nobits = nobits + (nobytes * 8);
        if(nobytes < 56) {
            printf("I've found a with less than 55 bytes!\n");
            //put 8 zeros
            M.e[nobytes] = 0x80;
            while(nobytes < 56) {
                
                nobytes = nobytes + 1;
                //8 bytes at the end,write 64 base bit endien integer with the number of the bites
                M.e[nobytes] = 0x00;
            }
            //All of the padding into the final block
            M.s[7] = nobits;
            //Set 's' for finish point
            s = FINISH;
        } else if (nobytes < 64) {
            s = PAD0;
            M.e[nobytes] = 0x80;
            while (nobytes < 64) {
                M.e[nobytes] = 0x00;
            }
        }
        // feof() function check file result
        else if(feof(f)) {
            //Set 's' for PAD1 point
            s = PAD1;
        }
        //printf("%llu\n", nobytes);
    }
    if(s == PAD0 || s == PAD1) {
        for(i=0; i<56; i++) 
            M.e[i] = 0x00;
        M.s[7] = nobits;
    }
    if(s == PAD1)
        // 0x80 = 10000000
        M.e[0] = 0x80;
    
    fclose(f);
    
    for(int i = 0; i < 64; i++)
        printf("%x ",M.e[i]);
    printf("\n");
    
    return 0;
}