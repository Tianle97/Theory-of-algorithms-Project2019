# Theory-of-Algorithms-Project2019
Theory-of-Algorithms-Project2019

#### Lecturer: *Ian McLoughlin*
#### Student Name: *Tianle Shu*
#### Student ID: *G00341565*

## *Introduction for SHA256*
SHA-256 stands for Secure Hash Algorithm – 256 bit and is a type of hash function commonly used in Blockchain. A hash function is a type of mathematical function which turns data into a fingerprint of that data called a hash. It’s like a formula or algorithm which takes the input data and turns it into an output of a fixed length, which represents the fingerprint of the data.</br>
The input data can literally be any data, whether it’s the entire Encyclopedia Britannica, or just the number ‘1’. A hash function will give the same hash for the same input always no matter when, where and how you run the algorithm. Equally interestingly, if even one character in the input text or data is changed, the output hash will change. Also, a hash function is a one-way function, thus it is impossible to generate back the input data from its hash. So, you can go from the input data to the hash but not from the hash to the input data.</br>
![Sha256](https://github.com/Tianle97/Theory-of-algorithms-Project2019/blob/master/Elements/sha256.jpg)


## *Introduction for C*
C (/siː/, as in the letter c) is a general-purpose, imperative computer programming language, supporting structured programming, lexical variable scope and recursion, while a static type system prevents many unintended operations. By design, C provides constructs that map efficiently to typical machine instructions, and it has therefore found lasting use in applications that were previously coded in assembly language. Such applications include operating systems, as well as various application software for computers ranging from supercomputers to embedded systems.</br>
C is an imperative procedural language. It was designed to be compiled using a relatively straightforward compiler, to provide low-level access to memory, to provide language constructs that map efficiently to machine instructions, and to require minimal runtime support. Despite its low-level capabilities, the language was designed to encourage cross-platform programming. A standards-compliant C program that is written with portability in mind can be compiled for a wide variety of computer platforms and operating systems with few changes to its source code; the language has become available on various platforms, from embedded microcontrollers to supercomputers.
![C](https://github.com/Tianle97/Theory-of-algorithms-Project2019/blob/master/Elements/C.jpg)

## *Explain the principle of SHA256 (Step Explain)*
In order to better understand the principle of SHA256, we first introduce the modules that can be extracted separately in the algorithm, including `the initialization of constants`, `the preprocessing of information`, and `the logical operations used`, and then explore these obstacles and explore them together. The main part of the SHA256 algorithm: `how the message digest is calculated`.

+ *Constant initialization*</br>
Eight hash initial values and 64 hash constants are used in the SHA256 algorithm.</br>
Among them, the initial hash values of the SHA256 algorithm are as follows:
``` C
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
```
These initial values are the first 32 bits of the fractional part of the square root of the first 8 prime numbers (2,3,5,7,11,13,17,19) in the natural number.</br>
For Example:</br>
the `sqrt{2}` fractional part is about `0.414213562373095048`, so:</br>
0.414213562373095048 ≈ 6∗16+a∗16+0∗16+...</br>
Therefore, the fractional part of the square root of the prime number `2 `takes the first `32 bits` and corresponds to `0x6a09e667`.</br>

+ *64 constants*:</br>
In the SHA256 algorithm, the 64 constants used are as follows:</br>
``` C
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
  ```
 Similar to the initial values of 8 hashes, these constants are the first 64 prime numbers in the natural number (2,3,5,7,11,13,17,19,23,29,31,37,41,43,47, The fractional part of the cube root of 53,59,61,67,71,73,79,83,89,97...) takes the first 32 bits.</br>

+ *pre-processin*</br>
The preprocessing in the SHA256 algorithm is to supplement the required information after the message that wants the Hash so that the entire message satisfies the specified structure.</br>
The preprocessing of information is divided into two steps: `additional padding bits` and `additional length`</br>

*STEP1:* Additional Padding Bits</br>
The padding is done by first filling the first bit to 1 and then adding 0 until the length is equal to 448 and the remainder is 448.
It should be noted that the information must be filled, that is, even if the length is already satisfied, the remainder of the 512 modulo is 448, and the complement must be performed, at which time 512 bits are filled.
Therefore, the padding is at least one bit and up to 512 bits.</br>
For Example: The information `abc` is taken as an example to show the process of complementing.

The [ASCII](http://ascii.911cha.com/) codes corresponding to a, b, and c are 97, 98, and 99, respectively.</br>
The binary code of the original information is: 01100001 01100010 01100011</br>
Firstly make up a "1": 0110000101100010 01100011 1</br>
Secondly complement 423 “0”: 01100001 01100010 01100011 10000000 00000000 ... 00000000</br>
The data after the completion of the complement is as follows (in order to introduce the hexadecimal notation):</br>
```C
61626380 00000000 00000000 00000000
00000000 00000000 00000000 00000000
00000000 00000000 00000000 00000000
00000000 00000000
```
`Why is 448?`</br>
Because after the first step of preprocessing, the second step will add a 64-bit data to indicate the length of the original message. And 448+64=512, just spelled a complete structure.

*STEP2:* Additional Length Value
Additional Length Value is append length of message (before pre-processing), in bits, as 64-bit big-endian integer.</br>
SHA256 uses a 64-bit data to represent the length of the original message.
Therefore, the message length calculated by SHA256 must be less than `2^64`, which is of course large enough in most cases.
The length information is encoded as 64-bit big-endian integer


## *Reference*
* C: https://en.wikipedia.org/wiki/C_(programming_language) <br/>
* Sha256: https://en.wikipedia.org/wiki/SHA-2 <br/>
