//Tianle Shu , 2019 Theory Of Algorithms Project
//The Secure Hash Algorithm 256 bit version

//The usual inpurt and output header file
#include <stdio.h>
//For using fixed bit length integerry
#include <stdint.h>

void sha 256();

int main(int argc, char *argv[]) {

		sha256();

		return 0;
}

void sha256(){

		//Message Schedule (Section 6.2).
		uint32_t W[64];
		//Working variables (Section 6.2)
		uint32_t a, b, c, d, e, f, g, h;
		//The Hash Vamue.
		uint32_t H[0];
		//The temporary variables (Section 6.2)
		uint32_t T1, T2;

		//The Hash value (Section 6.2)
		//The Values come from section 5.3.3

}

		
