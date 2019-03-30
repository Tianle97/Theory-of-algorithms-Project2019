//assert的作用是现计算表达式 expression ，如果其值为假（即为0），那么它先向stderr打印一条出错信息，然后通过调用 abort 来终止程序运行。
//#include <assert.h>  void assert( int expression );
//if return wrong result it will abort the process
#include <assert.h>	
//The usual inpurt and output header file
#include <stdio.h>
//For using fixed bit length integerry
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void sha256();

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

uint32_t swapE32(uint32_t x) {
    x = (x & 0xffff0000) >> 16 | (x & 0x0000ffff) << 16;
    x = (x & 0xff00ff00) >>  8 | (x & 0x00ff00ff) <<  8;
    return x;
}

uint64_t swapE64(uint64_t nobits) {
    uint64_t x = nobits;
    x = (x & 0xffffffff00000000) >> 32 | (x & 0x00000000ffffffff) << 32;
    x = (x & 0xffff0000ffff0000) >> 16 | (x & 0x0000ffff0000ffff) << 16;
    x = (x & 0xff00ff00ff00ff00) >>  8 | (x & 0x00ff00ff00ff00ff) <<  8;
    return x;
}

int main(int argc, char *argv[]) {
    char* msg = argv[1];
    
    sha256(msg);

    return 0;
}

void sha256(char *msg){
    
    //get the size of msg
    size_t len = strlen(msg);
    // 5.1.1
    uint64_t l = len * sizeof(char) * 8;
    size_t k = (448 - l - 1) % 512;
    if(k < 0) 
        k += 512;
    
    //checke the result
    //It can be used to add diagnostics to a C program.
    //More details:http://www.runoob.com/cprogramming/c-macro-assert.html
    assert((l+1+k) % 512 == 448);

    size_t msgSize = l + 1 + k + 64;
    
    //void* calloc (size_t num, size_t size);
    //calloc() 函数用来动态地分配内存空间并初始化为 0
    char* msgPad = (char*)calloc((msgSize / 8), sizeof(char));
    
    //C 库函数 void *memcpy(void *str1, const void *str2, size_t n) 从存储区 str2 复制 n 个字符到存储区 str1。
    memcpy(msgPad, msg, len);
    
    msgPad[len] = 0x80;
    
    l = swapE64(l);
    
    memcpy(msgPad+(msgSize/8)-8, &l, 8);

    // 5.2.1
    size_t N = msgSize / 512;

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
	uint32_t* M = (uint32_t*)msgPad;

	//For looping
	int i, t;

  //Loop through message blocks as per page 22
	for (i = 0; i < 1; i++) {
	
	//From page 22, W[t] = M[t] for 0 <= t <= 15.
	for (t = 0; t < 16; t++) {
        W[t] = swapE32(M[t]);
    }
        
    // Debug
//    printf("a ");
//    for (int h = 0; h < 64; h++) {
//        printf("%08x ", W[h]);
//    }
//    printf("\n\n");
			
	//From page 22 W[t]=...
	for(t = 16; t < 64; t++)
			W[t] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];

	//Initialise a, b, c, ... , h as per Step 2, Page 22
	a = H[0]; b = H[1]; c = H[2]; d = H[3];
	e = H[4]; f = H[5]; g = H[6]; h = H[7];

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