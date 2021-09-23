#include <stdio.h>

#define INPUT_LENGTH 503

unsigned long h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
unsigned long k[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                       0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                       0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                       0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                       0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                       0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                       0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                       0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void sha265() {
    unsigned char input[INPUT_LENGTH + 1];
    int pointer = INPUT_LENGTH;
    unsigned long leng;
    printf("Please input the text to encrypt:\n");
    fgets(input, INPUT_LENGTH, stdin);
    for (int i = 0; i < INPUT_LENGTH; i++) {
        if (input[i] == '\n') {
            pointer = i;
            break;
        }
    }
    leng = pointer << 3;
    input[pointer] = 0x80;
    while ((pointer + 4) % 64 != 0) {
        pointer++;
        input[pointer] = 0;
    }
    int temp = 24;
    for (int i = 0; i < 4; i++) {
        input[pointer] = (leng >> temp) & 0xff;
        temp -= 8;
        pointer++;
    }
    int wordlistSize = pointer >> 2;//pointer/4
    unsigned long wordList[wordlistSize];
    for (int i = 0; i < wordlistSize; ++i) {
        int index = i << 2;
        wordList[i] = (input[index] << 24) ^ (input[index + 1] << 16) ^ (input[index + 2] << 8) ^ (input[index + 3]);
    }
    unsigned long outWords[8];
    for (int i = 0; i < 8; i++) {
        outWords[i] = h[i];
    }
    int start = 0;
    int end = 16;
    while (start < wordlistSize) {
        unsigned long chunk[64];
        for (int i = 0; i < 16; i++) {
            chunk[i] = wordList[start + i];
        }
        for (int i = 16; i < 64; i++) {
            chunk[i] = 0;
        }
        for (int i = 16; i < 64; i++) {
            unsigned long s0 =
                    ((chunk[i - 15] >> 7) ^ (chunk[i - 15] << 25)) ^ ((chunk[i - 15] >> 18) ^ (chunk[i - 15] << 14)) ^
                    (chunk[i - 15] >> 3);
            unsigned long s1 =
                    ((chunk[i - 2] >> 17) ^ (chunk[i - 2] << 15)) ^ ((chunk[i - 2] >> 19) ^ (chunk[i - 2] << 13)) ^
                    (chunk[i - 2] >> 10);
            chunk[i] = chunk[i - 16] + s0 + chunk[i - 7] + s1;
        }
        unsigned long lett[8];
        for (int i = 0; i < 8; i++) {
            lett[i] = h[i];
        }
        for (int i = 0; i < 64; i++) {
            unsigned long s1 = ((lett[4] >> 6) ^ (lett[4] << 26)) ^ ((lett[4] >> 11) ^ (lett[4] << 21)) ^
                               ((lett[4] >> 25) ^ (lett[4] << 7));
            unsigned long ch = (lett[4] & lett[5]) ^ ((~lett[4]) & lett[6]);
            unsigned long temp1 = lett[7] + s1 + ch + k[i] + chunk[i];
            unsigned long s0 = ((lett[0] >> 2) ^ (lett[0] << 30)) ^ ((lett[0] >> 13) ^ (lett[0] << 19)) ^
                               ((lett[0] >> 22) ^ (lett[0] << 10));
            unsigned long maj = (lett[0] & lett[1]) ^ (lett[0] & lett[2]) ^ (lett[1] & lett[2]);
            unsigned long temp2 = s0 + maj;
            lett[7] = lett[6];
            lett[6] = lett[5];
            lett[5] = lett[4];
            lett[4] = lett[3] + temp1;
            lett[3] = lett[2];
            lett[2] = lett[1];
            lett[1] = lett[0];
            lett[0] = temp1 + temp2;
        }
        for (int i = 0; i < 8; i++) {
            outWords[i] += lett[i];
        }
        start += 16;
        end += 16;
    }
    printf("\n%X%X%X%X%X%X%X%X\n", outWords[0], outWords[1], outWords[2], outWords[3], outWords[4], outWords[5],
           outWords[6], outWords[7]);
}

int main(int argc, char *argv[]) {
    sha265();
}
