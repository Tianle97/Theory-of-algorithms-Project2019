#include <stdio.h>
#include <stdint.h>

struct point {
    uint8_t x[64];
    uint32_t y[16];
    uint64_t s[8];
};

int main(int argc,char *argv[]) {
    
    union msgblock M;
    
    uint64_t nobytes;
    
    FILE* f;
    
    char c;
    
    //Open the file
    f = fopen(argv[1],"r");
    
    while (!feof(f)) {
        fread(M.e,1, 64, f);
        printf("%llu",nobytes);
    }
    
    print("%c\n", fread(&c, 1, 1, f));
    
    //Close the file
    fclose(f);
    
    return 0;
}
