# Theory-of-Algorithms-Project2019
#### Lecturer: *Dr.Ian McLoughlin*
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
## *hash result*
This assignment finish main follow videos uploaded by Dr.Ian McLoughlin.
And this is my result hash:
![result](https://github.com/Tianle97/Theory-of-algorithms-Project2019/blob/master/Elements/test.png)
## *How to Run*
 * 1 Git clone or download this repository
 * 2 Open cmder and go to this project path
 * 3 Type this code in cmder: <br/>
   Compile >  `gcc -o sha256 sha256.c` </br>
   Execute >  `./sha256 test txt` </br>
 Or</br>
   Compile >  `gcc -o hash hash.c`</br>
   Execute >  `./hash "input what you want"` </br>
 
 * 4 Then you will get the hash result

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

#### *64 constants*:
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

#### *pre-processin*
The preprocessing in the SHA256 algorithm is to supplement the required information after the message that wants the Hash so that the entire message satisfies the specified structure.</br>
The preprocessing of information is divided into two steps: `additional padding bits` and `additional length`
</br>
</br>
*STEP1:* Additional Padding Bits</br>
The padding is done by first filling the first bit to 1 and then adding 0 until the length is equal to 448 and the remainder is 448.
It should be noted that the information must be filled, that is, even if the length is already satisfied, the remainder of the 512 modulo is 448, and the complement must be performed, at which time 512 bits are filled.
Therefore, the padding is at least one bit and up to 512 bits.
</br>
</br>
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

`Why is 448?`
Because after the first step of preprocessing, the second step will add a 64-bit data to indicate the length of the original message. And 448+64=512, just spelled a complete structure.
</br>

*STEP2:* Additional Length Value
Additional Length Value is append length of message (before pre-processing), in bits, as 64-bit big-endian integer.
SHA256 uses a 64-bit data to represent the length of the original message.</br>
Therefore, the message length calculated by SHA256 must be less than `2^64`, which is of course large enough in most cases.
The length information is encoded as 64-bit big-endian integer.</br>
Back to the example just now, the message `abc`, 3 characters, so occupies is 24 bits.
Therefore, after the complement length operation, the entire message becomes as follows (hexadecimal format)
```C
61626380 00000000 00000000 00000000
00000000 00000000 00000000 00000000
00000000 00000000 00000000 00000000
00000000 00000000 00000000 00000018
```

#### Logic Calculation
The operations involved in the SHA256 hash function are all logical bit operations.</br>
SHA-256 use six logical functions, where each function operates on 32-bit
words, which are represented as x, y, and z. The result of each function is a new 32-bit word.
Includes the following logic functions:</br>
```C
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
```

#### *Calculate message digest*
Firstly break message into 512-bit chunks: break chunk into sixteen 32-bit big-endian words w[0..15].Extend the sixteen 32-bit words into sixty-four 32-bit words:
```C
for(t = 16; t < 64; t++)
    W[t] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];
 
```

Initialize hash value for this chunk( a, b, c, ... , h) as per Step 2 Page 22
```C
a = H[0]; b = H[1]; c = H[2]; d = H[3];
e = H[4]; f = H[5]; g = H[6]; h = H[7];
```

step3 
64 encryption cycles
That is, an iteration of 64 encryption cycles is completed.
Each encryption cycle can be described by the following figure:</br>
图中，ABCDEFGH这8个字（word）在按照一定的规则进行更新，其中深蓝色方块是事先定义好的非线性逻辑函数，上文已经做过铺垫。</br>
红色田字方块代表 `mod 2^{32} addition`，即将两个数字加在一起，如果结果大于`2^{32}` ，你必须除以 ，你必须除以，你必须除以 `2^{32}` 并找到余数。
ABCDEFGH一开始的初始值分别为`H_{i-1}(0),H_{i-1}(1),…,H_{i-1}(7)`</br>
Kt是第t个密钥，对应我们上文提到的64个常量。</br>
Wt是本区块产生第t个word。原消息被切成固定长度512-bit的区块，对每一个区块，产生64个word，通过重复运行循环n次对ABCDEFGH这八个字循环加密。</br>
最后一次循环所产生的八个字合起来即是第i个块对应到的散列字符串`H_{i}` </br>
![ll](https://upload.wikimedia.org/wikipedia/commons/thumb/7/7d/SHA-2.svg/400px-SHA-2.svg.png)
```C
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
```
step4</br>
Add this chunk's hash to result so far:
```C
	H[0] = a + H[0]; H[1] = b + H[1];
        H[2] = c + H[2]; H[3] = d + H[3];
        H[4] = e + H[4]; H[5] = f + H[5];
        H[6] = g + H[6]; H[7] = h + H[7];
```
last step is print the value


## *References*
* C: https://en.wikipedia.org/wiki/C_(programming_language) <br/>
* Sha256: https://en.wikipedia.org/wiki/SHA-2 <br/>
* fread():http://www.runoob.com/cprogramming/c-function-fread.html</br>
* calloc(): http://c.biancheng.net/cpp/html/134.html
* memcpy(): https://blog.csdn.net/tigerjibo/article/details/6841531</br>
* swap_uint32() and swap_uint64(): https://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func</br>
