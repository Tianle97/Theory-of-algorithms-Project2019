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
    uint64_t s[1];
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
uint32_t rotr(uint32_t n, uint32_t x);
uint32_t shr(uint32_t n, uint32_t x);

//See Section 4.1.2 for definitions
uint32_t SIG0(uint32_t x);
uint32_t SIG1(uint32_t x);

//See Section 4.1.2 for definitions
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);

//Calculates the SHA256 function
void sha256(FILE* f);

//Retriveces the next message block
int nextmsgblock(FILE *funion, msgblock *M,enum status *s, uint64_t nobits);

int main(int argc, char *argv[]) {
    FILE* f;
    f = fopen(argv[1],"r")
    sha256(f);
    
    return 0;
}


//int main(int argc, char *argv[]) {
//    msgblockmethod();
//    char M[Max];
//    printf("please inuput :");
//    scanf("%s",M);
//    int n = strlen(M);
//    printf("M size %d\n",n);
//     //printf("argc: %d\n",argc);
//    if(argc < 2){
//        printf("please input valid:\n");
//        return 0;
//    }else {
//        sha256();
//        return 0;
//    }
//}

// swap byte endian
//more https://songlee24.github.io/2015/05/02/endianess/
//uint32_t swapE32(uint32_t x) {
//    x = (x & 0xffff0000) >> 16 | (x & 0x0000ffff) << 16;
//    x = (x & 0xff00ff00) >>  8 | (x & 0x00ff00ff) <<  8;
//    return x;
//}

//uint64_t swapE64(uint64_t x) {
//    x = (x & 0xffffffff00000000) >> 32 | (x & 0x00000000ffffffff) << 32;
//    x = (x & 0xffff0000ffff0000) >> 16 | (x & 0x0000ffff0000ffff) << 16;
//    x = (x & 0xff00ff00ff00ff00) >>  8 | (x & 0x00ff00ff00ff00ff) <<  8;
//    return x;
//}

void sha256(FILE* f){    
    //The current message block
    union msgblock M;
    
    // The number of bits read from the file
	uint64_t nobytes;
    
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
	for (nextmsgblock(f, M, S, nobits)) {
	
        //From page 22, W[t] = M[t] for 0 <= t <= 15.
        for (t = 0; t < 16; t++)
            W[t] = M.t[t];

        //From page 22 W[t]=...
        for(t = 16; t < 64; t++)
            W[t] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];

        //Initialise a, b, c, ... , h as per Step 2, Page 22
        a = H[0]; b = H[1]; c = H[2]; d = H[3];
        e = H[4]; f = H[5]; g = H[6]; h = H[7];

        //Steps 3.
        for (t = 0; t < 64; t++) {
            T1 = h + SIG1(e) + Ch(e, f, g) + K[t] + W[t];
            T2 = SIG0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        //Step 4.
        H[0] = a + H[0];
        H[1] = b + H[1];
        H[2] = c + H[2];
        H[3] = d + H[3];
        H[4] = e + H[4];
        H[5] = f + H[5];
        H[6] = g + H[6];
        H[7] = h + H[7];
    }  
    printf("%x %x %x %x %x %x %x %x\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
}

//See Sections 3.2 for definitions
uint32_t rotr(uint32_t n, uint32_t x) {
	return (x >> n) | ( x<< (32 - n));
}
uint32_t shr(uint32_t n, uint32_t x) {
	return (x >> n);
}

// See Sections 3.2 and 4.1.2 for definitions
uint32_t sig0(uint32_t x) {
	return (rotr(7, x) ^ rotr(18, x) ^ shr(3, x));
}
// See Sections 3.2 and 4.1.2 for definitions
uint32_t sig1(uint32_t x) {
	return (rotr(17, x) ^ rotr(19, x) ^ shr(10, x));
}

//See Section 4.1.2 for definitions
uint32_t SIG0(uint32_t x) {
	return rotr(2, x) ^ rotr(13, x) ^ rotr(22, x);
}
uint32_t SIG1(uint32_t x) {
	return rotr(6, x) ^ rotr(11, x) ^ rotr(25, x);
}

//See Section 4.1.2 for definitions
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) ^ ((!x) & z);
}
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) ^ (x ^ z) ^ (y ^ z);
}

int nextmsgblock(FILE *funion, msgblock *M,enum status *s, uint64_t nobits) {

    // The number of bytes we get from fread
	uint64_t nobytes;
    
	uint64_t nobits = 0;
    
    //Just for loop use.
	int i;
    
    //If we have finished all the message blocks, then 's' should be FINISH
    if(*s == FINISH){
        return 0;
    }
    
    // Otherwise, Check if we need another block full of padding.
    if(s == PAD0 || s == PAD1){
        //Set the first 56 bytes to all zero bits
		for(i =0; i<56; i++){
			M.e[i] = 0x00;
		}
        // Set the last 64 bits to the number of bits in the file (should be big-endien)
		M.s[7] = nobits;
        // Tell 's' we are finish
        *s = FINISH;
	}
    // If 's' was PAD1, then set the first bit of M to one
	if(s == PAD1){
		M.e[0] = 0x80;
    //Keep the looo in the sha256 going for one more iteration
    return 1;
	}

    // If we get down here.We have not finished reading the file (s == READ)
    nobytes = fread(M->e, 1, 64, f);
    printf("%llu\n", nobytes);
    //Keep track of the number of bytes we've read
    *nobits = *nobits + (nobytes * 8);
    / If we read less than 56, we can put all padding in this message block
    if(nobytes < 56){
        printf("I've Found a block with less then 55 bytes\n");
        // Add the one bit, as per the standard
        M->e[nobytes] = 0x80;
        while(nobytes < 56){
            nobytes = nobytes + 1;
            M.e[nobytes] = 0x00;
        }

        M.s[7] = nobits;
        s = FINISH;
    }else if (nobytes < 64){
        s = PAD0;
        M.e[nobytes] = 0X80;
        while( nobytes < 64){
            nobytes = nobytes + 1;
            M.e[nobytes] = 0x00;
        }
    }else if(feof(f)){
        s = PAD1;
    }
} // end loop

fclose(f);

for(int i= 0; i < 64; i++)
    printf("%x ", M.e[i]);
    printf("\n");

return 0;
    
}



