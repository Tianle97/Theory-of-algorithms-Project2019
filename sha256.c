//Tianle Shu , 2019 Theory Of Algorithms Project
//The Secure Hash Algorithm 256 bit version

/* How to paste and Copy in vi:
** esc -> V 键 = visual model.  **
** 选中所需copy的行 按下 y 键   **
** Cursor the need copy line to **
**   highlight and click Y      **
** 将光标移至需paste的地方 按下 p 键 **
** Move to paste line click P **/


//The usual inpurt and output header file
#include <stdio.h>
//For using fixed bit length integerry
#include <stdint.h>

void sha256();

//See Section 4.1.2 and 4.2.2 for definitions.
uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

int main(int argc, char *argv[]) {

		sha256();

		return 0;
}

void sha256(){

		//Message Schedule (Section 6.2).
		//Explain uintN_t in Chinese: https://baike.baidu.com/item/stdint.h/2439217?fr=aladdin
		uint32_t W[64];
		//Working variables (Section 6.2)
		uint32_t a, b, c, d, e, f, g, h;
		//The Hash Vamue.
		uint32_t H[0];
		//The temporary variables (Section 6.2)
		uint32_t T1, T2;

		//The Hash value (Section 6.2)
		//The Values come from section 5.3.3
		uint32_t H[8] = {
				0x6a09e667
			, 0xbb67ae85
			,	0x3c6ef372
			,	0xa54ff53a
			,	0x510e527f
			, 0x9b05688c
			,	0x1f83d9ab
			,	0x5be0cd19
		};

		//The current message black
		uint32_t M[16];

		//For looping
		int i;

		for (i = 0; i < 16; i++)
				W[i] = M[i];

		//From page 22 W[t]=...
		for(i = 16; i < 64; i++)
				sig_1(W[t-2]) + W[t-7] + sig_0(W[t-15]) + W[t-16];
		
}

uint32_t sig0(uint32_t x) {

}

uint32_t sig1(uint32_t x) {

}


