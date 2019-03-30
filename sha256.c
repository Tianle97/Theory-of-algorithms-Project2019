//Tianle Shu , 2019 Theory Of Algorithms Project
//The Secure Hash Algorithm 256 bit version

/* How to paste and Copy in vi:
** esc -> V 键 = visual model.  **
** 选中所需copy的行 按下 y 键   **
** Cursor the need copy line to **
**   highlight and click Y      **
** 将光标移至需paste的地方 按下 p 键 **
** Move to paste line click P **/

#include <stdlib.h>
//The usual inpurt and output header file
#include <stdio.h>
#include <string.h>
//For using fixed bit length integerry
#include <stdint.h>

//Rpresents a message block.
union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

// A flag for where we are reading the file
enum status {
    READ,
    PAD0,
    PAD1,
    FINISH  
};

//See Section 4.1.2 and 4.2.2 for definitions.
uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

//See Sections 3.2 for definitions
uint32_t rotr(uint32_t x, uint32_t n);
uint32_t shr(uint32_t x, uint32_t n);

//See Section 4.1.2 for definitions
uint32_t EP0(uint32_t x);
uint32_t EP1(uint32_t x);

//See Section 4.1.2 for definitions
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);

//Calculates the SHA256 function
void sha256(FILE* f);

//Retriveces the next message block
int nextmsgblock(FILE *funion, union msgblock *M,enum status *s, uint64_t *nobits);

// swap byte endian
//more https://songlee24.github.io/2015/05/02/endianess/
//Unsigned 32-bit conversion:
uint32_t swapE32(uint32_t x) {
    x = (x & 0xffff0000) >> 16 | (x & 0x0000ffff) << 16;
    x = (x & 0xff00ff00) >>  8 | (x & 0x00ff00ff) <<  8;
    return x;
}

//Unsigned 64-bit conversion:
uint64_t swapE64(uint64_t nobits) {
    uint64_t x = nobits;
    x = (x & 0xffffffff00000000) >> 32 | (x & 0x00000000ffffffff) << 32;
    x = (x & 0xffff0000ffff0000) >> 16 | (x & 0x0000ffff0000ffff) << 16;
    x = (x & 0xff00ff00ff00ff00) >>  8 | (x & 0x00ff00ff00ff00ff) <<  8;
    return x;
}

int main(int argc, char *argv[]) {
    
    FILE* msgf;
    //Open the file given us first command line argument
    msgf = fopen(argv[1],"r");
    //should do error checking here
    if (argc < 2){
        //if do not input file name
        printf("please input file name.");
    }
    
    //Run the secure hash algorithm on the file
    sha256(msgf);
    
    //Close the file
    fclose(msgf);
    printf("\n");
    return 0;
}

void sha256(FILE *msgf){    
    //The current message block
    union msgblock M;
    
    // The number of bits read from the file
	uint64_t nobits = 0;
    
    //The dtatus of message blocks, in terms of padding
    enum status s = READ;
    
	//The K constants, defined in Section 4.2.2
	uint32_t K[] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 
		0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 
		0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
		0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
		0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
		0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
		0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
		0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

	//Message Schedule (Section 6.2).
	//Explain uintN_t in Chinese: https://baike.baidu.com/item/stdint.h/2439217?fr=aladdin
	uint32_t W[64];
    
	//Working variables (Section 6.2)
	uint32_t a, b, c, d, e, f, g, h;

	//The Hash Value (Section 6.2)
	//The temporary variables (Section 6.2)
	uint32_t T1, T2;

	//The Hash value (Section 6.2)
	//The Values come from section 5.3.3
	uint32_t H[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372, 
        0xa54ff53a, 
        0x510e527f, 
        0x9b05688c, 
        0x1f83d9ab, 
        0x5be0cd19
	};

	//The current message black
	//uint32_t M[16] = {0, 0, 0, 0, 0, 0, 0, 0};

	//For looping
	int i, t;
       
  //Loop through message blocks as per page 22
	while (nextmsgblock(msgf, &M, &s, &nobits)) {
	
        //From page 22, W[t] = M[t] for 0 <= t <= 15.
        for (t = 0; t < 16; t++)
            W[t] = swapE32(M.t[t]);

        //From page 22 W[t]=...
        for(t = 16; t < 64; t++)
            W[t] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];

        // Debug
//        for (int h = 0; h < 64; h++) {
//            printf("%08x ", W[h]);
//        }
//        printf("\n\n");
        
        //Initialise a, b, c, ... , h as per Step 2, Page 22
        a = H[0]; b = H[1]; c = H[2]; d = H[3];
        e = H[4]; f = H[5]; g = H[6]; h = H[7];

        // Debug for chck
//        printf("a %08x ", a);
//        printf("%08x ", b);
//        printf("%08x ", c);
//        printf("%08x ", d);
//        printf("%08x ", e);
//        printf("%08x ", f);
//        printf("%08x ", g);
//        printf("%08x ", h);
//        printf("\n\n");
        
        //Steps 3.
        for (t = 0; t < 64; t++) {
            T1 = h + EP1(e) + Ch(e, f, g) + K[t] + W[t];
            T2 = EP0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        // Debug for check
//        printf("b %08x ", a);
//        printf("%08x ", b);
//        printf("%08x ", c);
//        printf("%08x ", d);
//        printf("%08x ", e);
//        printf("%08x ", f);
//        printf("%08x ", g);
//        printf("%08x ", h);
//        printf("\n\n");
        
        //Step 4.
        H[0] = a + H[0]; H[1] = b + H[1];
        H[2] = c + H[2]; H[3] = d + H[3];
        H[4] = e + H[4]; H[5] = f + H[5];
        H[6] = g + H[6]; H[7] = h + H[7];
    }  

    printf("%08x%08x%08x%08x%08x%08x%08x%08x\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
}

//See Sections 3.2 for definitions
uint32_t rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32-n));
}
uint32_t shr(uint32_t x, uint32_t n) {
    return x >> n;
}

// See Sections 3.2 and 4.1.2 for definitions
uint32_t sig0(uint32_t x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3);
}
uint32_t sig1(uint32_t x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10);
}

//See Section 4.1.2 for definitions
uint32_t EP0(uint32_t x) {
	return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}
uint32_t EP1(uint32_t x) {
	return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

//See Section 4.1.2 for definitions
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) ^ ((~x) & z);
}
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) ^ (x & z) ^ (y & z);
}

int nextmsgblock(FILE *msgf, union msgblock *M,enum status *s, uint64_t *nobits) {

    // The number of bytes we get from fread
	uint64_t nobytes;
    
    //Just for loop use.
	int i;
    
    //If we have finished all the message blocks, then 's' should be FINISH
    if(*s == FINISH){
        return 0;
    }
    
    // Otherwise, Check if we need another block full of padding.
    if(*s == PAD0 || *s == PAD1){
        
        //Set the first 56 bytes to all zero bits
		for(i =0; i<56; i++){
			M->e[i] = 0x00;
		}
        
        // Set the last 64 bits to the number of bits in the file (should be big-endien)
		M->s[7] = swapE64(*nobits);
        
        // Tell 's' we are finish
        *s = FINISH;
	}
    
    // If 's' was PAD1, then set the first bit of M to one
	if(*s == PAD1){
        
		M->e[0] = 0x80;
        
    //Keep the looo in the sha256 going for one more iteration
    return 1;
	}

    // If we get down here.We have not finished reading the file (s == READ)
    // C 库函数 size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) 从给定流 stream 读取数据到 ptr 所指向的数组中。
    nobytes = fread(M->e, 1, 64, msgf);
    
    //Just for check
    //  printf("fread1: %llu\n",nobytes);
    
    //Keep track of the number of bytes we've read
    *nobits = *nobits + (nobytes * 8);
    //*nobits = swapE64(*nobits);
    
    // If we read less than 56, we can put all padding in this message block
    if(nobytes < 56){
        
        // Add the one bit, as per the standard
        M->e[nobytes] = 0x80;
        
        //Add 0 bits until the last 64 bits
        while(nobytes < 56){
            nobytes = nobytes + 1;
            M->e[nobytes] = 0x00;
        }
        
        //Append the file size in bits as a (should be big endian unsigned 64 bit int)
        M->s[7] = swapE64(*nobits);
        
        //Tell us FINISH
        *s = FINISH;
    }
    //Otherwise, check if we can put some padding into this message block
    else if (nobytes < 64){
        
        //Tekk 's' we need another message block, with padding but no one bit
        *s = PAD0;
        
        //Put the one bit into the current block
        M->e[nobytes] = 0X80;
        
        //Pad the rest of the block with 0 bits
        while( nobytes < 64){
            nobytes = nobytes + 1;
            M->e[nobytes] = 0x00;
        }
    }
    
    //Otherwise, check if we're just at the end of the file
    else if(feof(msgf)){
        
        //Tell 's' that we need another message block with all the padding
        *s = PAD1;
    }
    
    //If we get this far, then return 1 so that this function called again
    return 1;
}



