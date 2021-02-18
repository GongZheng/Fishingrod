/*
 *  Fishingrod.h
 *
 *  Description: Fishingrod-128 with a clear structure.
 *  Created on: 2020-12-28
 *  Coding: Zheng Gong, Jinhai Chen.
 */


#include <stdio.h>
#include <inttypes.h>

#ifndef FISHINGROD_H_
#define FISHINGROD_H_

//comment this out if this is used on PC
//#define __UINT_T__
// #ifndef __UINT_T__
// #define __UINT_T__
// typedef unsigned char uint8_t;
// typedef short int int8_t;
// typedef unsigned short uint16_t;
// typedef unsigned long uint32_t;
// typedef unsigned long long uint64_t;
// #endif /*__UINT_T__*/

//rounds of Fishingrod;
#define ROUNDS 18

//The 8-bit sbox for Fishingrod, same as AES
static const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

//Multiply * 2 without using LUT
uint8_t xtime(uint8_t input)
{
    return (input<<1) ^ (((input>>7) & 1) * 0x1b);
	
	//if((input & 0x80) == 0)
	//   return (input << 1) ^ 0x00;
	//else
	//   return (input << 1) ^ 0x1B;
}

//The LFSR function D used in the key schedule;
uint32_t lfsr(uint32_t round_key_value)
{
    return (round_key_value ^ 
    ((round_key_value << 2) ^ (round_key_value >> 30)) ^ 
    ((round_key_value << 11) ^ (round_key_value >> 21)));
}

#define fishingrod_encrypt(plain, key, cipher) fishingrod_encrypt_rounds((plain), (key), ROUNDS, (cipher))
#define fishingrod_decrypt(cipher, key, plain) fishingrod_decrypt_rounds((cipher), (key), ROUNDS, (plain))

//Precompute all the subkeys in advance;
//void fishingrod_key_expansion(uint32_t *round_key, const uint32_t *key, const uint8_t rounds)
//{
//    uint8_t i;
//
//    round_key[0] = key[0];
//    round_key[1] = key[1];
//    round_key[2] = key[2];
//    round_key[3] = key[3];
//    
//    for(i=1; i<=rounds; i++)
//    {
//        round_key[(i-1)+4] = lfsr(round_key[(i-1)+3] ^ round_key[i-1]);
//    };
//
//
//}

//Update the subkey on the fly; keystate is the u
void fishingrod_key_update(uint32_t *keystate, uint8_t *round_key)
{
    uint32_t temp1, temp2;

    uint64_t * ptr_keystate, * ptr_round_key;

    ptr_keystate = (uint64_t * )  keystate;
    ptr_round_key = (uint64_t * ) round_key;
    ptr_round_key[0] = ptr_keystate[0];

    temp1 = lfsr(keystate[3] ^ keystate[0]);
    temp2 = lfsr(temp1 ^ keystate[1]);

    keystate[0] = keystate[2];
    keystate[1] = keystate[3];
    keystate[2] = temp1;
    keystate[3] = temp2;

}

void fishingrod_key_schedule(const uint32_t *master_key, uint32_t *round_key) {
    int i,j;
    round_key[0] = master_key[0];
    round_key[1] = master_key[1];
    round_key[2] = ~round_key[0];
    round_key[3] = ~round_key[1];

    round_key[4] = master_key[2];
    round_key[5] = master_key[3];
    round_key[6] = ~round_key[4];
    round_key[7] = ~round_key[5];

    round_key[8] = lfsr(round_key[5]^round_key[0] );
    round_key[9] = lfsr(round_key[8]^round_key[1] );
    round_key[10] = ~round_key[8];
    round_key[11] = ~round_key[9];
    round_key[12] = lfsr(round_key[9]^round_key[4] );
    round_key[13] = lfsr(round_key[12]^round_key[5] );
    round_key[14] = ~round_key[12];
    round_key[15] = ~round_key[13];
    round_key[16] = lfsr(round_key[13]^round_key[8] );
    round_key[17] = lfsr(round_key[16]^round_key[9] );
    round_key[18] = ~round_key[16];
    round_key[19] = ~round_key[17];
    round_key[20] = lfsr(round_key[17]^round_key[12] );
    round_key[21] = lfsr(round_key[20]^round_key[13] );
    round_key[22] = ~round_key[20];
    round_key[23] = ~round_key[21];
    round_key[24] = lfsr(round_key[21]^round_key[16] );
    round_key[25] = lfsr(round_key[24]^round_key[17] );
    round_key[26] = ~round_key[24];
    round_key[27] = ~round_key[25];
    round_key[28] = lfsr(round_key[25]^round_key[20] );
    round_key[29] = lfsr(round_key[28]^round_key[21] );
    round_key[30] = ~round_key[28];
    round_key[31] = ~round_key[29];
    round_key[32] = lfsr(round_key[29]^round_key[24] );
    round_key[33] = lfsr(round_key[32]^round_key[25] );
    round_key[34] = ~round_key[32];
    round_key[35] = ~round_key[33];
    
}

void fishingrod_encrypt_rounds(const uint8_t *plain, uint32_t *round_key64, uint8_t *cipher)
{
	uint8_t lstate[8];
    uint8_t rstate[8];
	uint8_t temp_state[8];
	uint8_t u,v,w,x,y;
	uint8_t *round_key;
	uint8_t i;

    //initialize the L and R states;
    lstate[0] = plain[0];
	lstate[1] = plain[1];
	lstate[2] = plain[2];
	lstate[3] = plain[3];
	lstate[4] = plain[4];
	lstate[5] = plain[5];
	lstate[6] = plain[6];
	lstate[7] = plain[7];

    rstate[0] = plain[8];
	rstate[1] = plain[9];
	rstate[2] = plain[10];
	rstate[3] = plain[11];
	rstate[4] = plain[12];
	rstate[5] = plain[13];
	rstate[6] = plain[14];
	rstate[7] = plain[15];

    round_key = (uint8_t *)(round_key64);
    
    for(i=0; i<ROUNDS; i+=2)
    {
        //Add roundkey K_i;
        temp_state[0] = lstate[0] & round_key[0];
        temp_state[1] = lstate[1] & round_key[1];
        temp_state[2] = lstate[2] & round_key[2];
        temp_state[3] = lstate[3] & round_key[3];
        temp_state[4] = lstate[4] & round_key[4];
        temp_state[5] = lstate[5] & round_key[5];
        temp_state[6] = lstate[6] & round_key[6];
        temp_state[7] = lstate[7] & round_key[7];

        //Xor with R_i;
        temp_state[0] = temp_state[0] ^ rstate[0];
        temp_state[1] = temp_state[1] ^ rstate[1];
        temp_state[2] = temp_state[2] ^ rstate[2];
        temp_state[3] = temp_state[3] ^ rstate[3];
        temp_state[4] = temp_state[4] ^ rstate[4];
        temp_state[5] = temp_state[5] ^ rstate[5];
        temp_state[6] = temp_state[6] ^ rstate[6];
        temp_state[7] = temp_state[7] ^ rstate[7];

        //subbytes with AES 8-bit sbox, combine with cyclic left rotating two bytes;
        u = sbox[temp_state[0]];
        v = sbox[temp_state[1]];
        temp_state[0] = sbox[temp_state[2]];
        temp_state[1] = sbox[temp_state[3]];
        temp_state[2] = sbox[temp_state[4]];
        temp_state[3] = sbox[temp_state[5]];
        temp_state[4] = sbox[temp_state[6]];
        temp_state[5] = sbox[temp_state[7]];
        temp_state[6] = u;
        temp_state[7] = v;

        //MDS permutation with AES MixColumns, without using LUT;
        //MDS permutation with AES MixColumns, without using LUT;
        u = temp_state[0];
        v = temp_state[1];
        w = temp_state[2];
        x = temp_state[3];
        y = u^v^w^x;

        temp_state[0] = xtime(u) ^ xtime(v) ^ y ^ u;
        temp_state[1] = xtime(v) ^ xtime(w) ^ y ^ v;
        temp_state[2] = xtime(w) ^ xtime(x) ^ y ^ w;
        temp_state[3] = xtime(u) ^ xtime(x) ^ y ^ x;


        u = temp_state[4];
        v = temp_state[5];
        w = temp_state[6];
        x = temp_state[7];
        y = u^v^w^x;

        temp_state[4] = xtime(u) ^ xtime(v) ^ y ^ u;
        temp_state[5] = xtime(v) ^ xtime(w) ^ y ^ v;
        temp_state[6] = xtime(w) ^ xtime(x) ^ y ^ w;
        temp_state[7] = xtime(u) ^ xtime(x) ^ y ^ x;

        //output L_i+1 of round function (before the exchange with R_i+1)
        lstate[0] = lstate[0] ^ temp_state[0];
        lstate[1] = lstate[1] ^ temp_state[1];
        lstate[2] = lstate[2] ^ temp_state[2];
        lstate[3] = lstate[3] ^ temp_state[3];
        lstate[4] = lstate[4] ^ temp_state[4];
        lstate[5] = lstate[5] ^ temp_state[5];
        lstate[6] = lstate[6] ^ temp_state[6];
        lstate[7] = lstate[7] ^ temp_state[7];

        //output R_i+1 of round function (before the exchange with R_i+1)
        rstate[0] = (temp_state[0] & round_key[0]) ^ rstate[0];
	    rstate[1] = (temp_state[1] & round_key[1]) ^ rstate[1];
        rstate[2] = (temp_state[2] & round_key[2]) ^ rstate[2];
	    rstate[3] = (temp_state[3] & round_key[3]) ^ rstate[3];
        rstate[4] = (temp_state[4] & round_key[4]) ^ rstate[4];
	    rstate[5] = (temp_state[5] & round_key[5]) ^ rstate[5];
	    rstate[6] = (temp_state[6] & round_key[6]) ^ rstate[6];
        rstate[7] = (temp_state[7] & round_key[7]) ^ rstate[7];

        round_key += 8;

        //Add roundkey K_i;
        temp_state[0] = rstate[0] & round_key[0];
        temp_state[1] = rstate[1] & round_key[1];
        temp_state[2] = rstate[2] & round_key[2];
        temp_state[3] = rstate[3] & round_key[3];
        temp_state[4] = rstate[4] & round_key[4];
        temp_state[5] = rstate[5] & round_key[5];
        temp_state[6] = rstate[6] & round_key[6];
        temp_state[7] = rstate[7] & round_key[7];

        //Xor with R_i;
        temp_state[0] = temp_state[0] ^ lstate[0];
        temp_state[1] = temp_state[1] ^ lstate[1];
        temp_state[2] = temp_state[2] ^ lstate[2];
        temp_state[3] = temp_state[3] ^ lstate[3];
        temp_state[4] = temp_state[4] ^ lstate[4];
        temp_state[5] = temp_state[5] ^ lstate[5];
        temp_state[6] = temp_state[6] ^ lstate[6];
        temp_state[7] = temp_state[7] ^ lstate[7];

        //subbytes with AES 8-bit sbox, combine with cyclic left rotating two bytes;
        u = sbox[temp_state[0]];
        v = sbox[temp_state[1]];
        temp_state[0] = sbox[temp_state[2]];
        temp_state[1] = sbox[temp_state[3]];
        temp_state[2] = sbox[temp_state[4]];
        temp_state[3] = sbox[temp_state[5]];
        temp_state[4] = sbox[temp_state[6]];
        temp_state[5] = sbox[temp_state[7]];
        temp_state[6] = u;
        temp_state[7] = v;

        //MDS permutation with AES MixColumns, without using LUT;
        //MDS permutation with AES MixColumns, without using LUT;
        u = temp_state[0];
        v = temp_state[1];
        w = temp_state[2];
        x = temp_state[3];
        y = u^v^w^x;

        temp_state[0] = xtime(u) ^ xtime(v) ^ y ^ u;
        temp_state[1] = xtime(v) ^ xtime(w) ^ y ^ v;
        temp_state[2] = xtime(w) ^ xtime(x) ^ y ^ w;
        temp_state[3] = xtime(u) ^ xtime(x) ^ y ^ x;


        u = temp_state[4];
        v = temp_state[5];
        w = temp_state[6];
        x = temp_state[7];
        y = u^v^w^x;

        temp_state[4] = xtime(u) ^ xtime(v) ^ y ^ u;
        temp_state[5] = xtime(v) ^ xtime(w) ^ y ^ v;
        temp_state[6] = xtime(w) ^ xtime(x) ^ y ^ w;
        temp_state[7] = xtime(u) ^ xtime(x) ^ y ^ x;


        //output L_i+1 of round function (before the exchange with R_i+1)
        rstate[0] = rstate[0] ^ temp_state[0];
        rstate[1] = rstate[1] ^ temp_state[1];
        rstate[2] = rstate[2] ^ temp_state[2];
        rstate[3] = rstate[3] ^ temp_state[3];
        rstate[4] = rstate[4] ^ temp_state[4];
        rstate[5] = rstate[5] ^ temp_state[5];
        rstate[6] = rstate[6] ^ temp_state[6];
        rstate[7] = rstate[7] ^ temp_state[7];

        //output R_i+1 of round function (before the exchange with R_i+1)
        lstate[0] = (temp_state[0] & round_key[0]) ^ lstate[0];
	    lstate[1] = (temp_state[1] & round_key[1]) ^ lstate[1];
        lstate[2] = (temp_state[2] & round_key[2]) ^ lstate[2];
	    lstate[3] = (temp_state[3] & round_key[3]) ^ lstate[3];
        lstate[4] = (temp_state[4] & round_key[4]) ^ lstate[4];
	    lstate[5] = (temp_state[5] & round_key[5]) ^ lstate[5];
	    lstate[6] = (temp_state[6] & round_key[6]) ^ lstate[6];
        lstate[7] = (temp_state[7] & round_key[7]) ^ lstate[7];
        round_key += 8;

    }

	//final output of full round fishingrod
    cipher[0] = lstate[0];
	cipher[1] = lstate[1];
    cipher[2] = lstate[2];
	cipher[3] = lstate[3];
    cipher[4] = lstate[4];
	cipher[5] = lstate[5];
	cipher[6] = lstate[6];
    cipher[7] = lstate[7];

	cipher[8] = rstate[0];
    cipher[9] = rstate[1];
    cipher[10] = rstate[2];
    cipher[11] = rstate[3];
    cipher[12] = rstate[4];
    cipher[13] = rstate[5];
    cipher[14] = rstate[6];
    cipher[15] = rstate[7];

}

void fishingrod_decrypt_rounds(const uint8_t *cipher, uint32_t *round_key64, uint8_t *plain)
{
	uint8_t lstate[8];
    uint8_t rstate[8];
	uint8_t temp_state[8];
	uint8_t u,v,w= 0;
	uint8_t *round_key;
	int i;

    //initialize the L and R states;
    lstate[0] = cipher[0];
	lstate[1] = cipher[1];
	lstate[2] = cipher[2];
	lstate[3] = cipher[3];
	lstate[4] = cipher[4];
	lstate[5] = cipher[5];
	lstate[6] = cipher[6];
	lstate[7] = cipher[7];

    rstate[0] = cipher[8];
	rstate[1] = cipher[9];
	rstate[2] = cipher[10];
	rstate[3] = cipher[11];
	rstate[4] = cipher[12];
	rstate[5] = cipher[13];
	rstate[6] = cipher[14];
	rstate[7] = cipher[15];

    for(i=ROUNDS-1; i>=0; i--)
    {
        round_key = (uint8_t *)(round_key64+i);
        //Add roundkey K_i;
        temp_state[0] = rstate[0] & round_key[0];
        temp_state[1] = rstate[1] & round_key[1];
        temp_state[2] = rstate[2] & round_key[2];
        temp_state[3] = rstate[3] & round_key[3];
        temp_state[4] = rstate[4] & round_key[4];
        temp_state[5] = rstate[5] & round_key[5];
        temp_state[6] = rstate[6] & round_key[6];
        temp_state[7] = rstate[7] & round_key[7];

        //Xor with R_i;
        temp_state[0] = temp_state[0] ^ lstate[0];
        temp_state[1] = temp_state[1] ^ lstate[1];
        temp_state[2] = temp_state[2] ^ lstate[2];
        temp_state[3] = temp_state[3] ^ lstate[3];
        temp_state[4] = temp_state[4] ^ lstate[4];
        temp_state[5] = temp_state[5] ^ lstate[5];
        temp_state[6] = temp_state[6] ^ lstate[6];
        temp_state[7] = temp_state[7] ^ lstate[7];

        //subbytes with AES 8-bit sbox, combine with cyclic left rotating two bytes;
        u = sbox[temp_state[0]];
        v = sbox[temp_state[1]];
        temp_state[0] = sbox[temp_state[2]];
        temp_state[1] = sbox[temp_state[3]];
        temp_state[2] = sbox[temp_state[4]];
        temp_state[3] = sbox[temp_state[5]];
        temp_state[4] = sbox[temp_state[6]];
        temp_state[5] = sbox[temp_state[7]];
        temp_state[6] = u;
        temp_state[7] = v;

        //MDS permutation with AES MixColumns, without using LUT;
        u = temp_state[0] ^ temp_state[1] ^ temp_state[2] ^ temp_state[3];
        v = temp_state[0] ^ temp_state[1];
        v = xtime(v);
        w = temp_state[0];
        temp_state[0] = temp_state[0] ^ v ^ u;

        v = temp_state[1] ^ temp_state[2];
        v = xtime(v);
        temp_state[1] = temp_state[1] ^ v ^ u;

        v = temp_state[2] ^ temp_state[3];
        v = xtime(v);
        temp_state[2] = temp_state[2] ^ v ^ u;

        v = temp_state[3] ^ w;
        v = xtime(v);
        temp_state[3] = temp_state[3] ^ v ^ u;

        u = temp_state[4] ^ temp_state[5] ^ temp_state[6] ^ temp_state[7];
        v = temp_state[4] ^ temp_state[5];
        v = xtime(v);
        w = temp_state[4];
        temp_state[4] = temp_state[4] ^ v ^ u;

        v = temp_state[5] ^ temp_state[6];
        v = xtime(v);
        temp_state[5] = temp_state[5] ^ v ^ u;

        v = temp_state[6] ^ temp_state[7];
        v = xtime(v);
        temp_state[6] = temp_state[6] ^ v ^ u;

        v = temp_state[7] ^ w;
        v = xtime(v);
        temp_state[7] = temp_state[7] ^ v ^ u;

        //output L_i+1 of round function (before the exchange with R_i+1)
        rstate[0] = rstate[0] ^ temp_state[0];
        rstate[1] = rstate[1] ^ temp_state[1];
        rstate[2] = rstate[2] ^ temp_state[2];
        rstate[3] = rstate[3] ^ temp_state[3];
        rstate[4] = rstate[4] ^ temp_state[4];
        rstate[5] = rstate[5] ^ temp_state[5];
        rstate[6] = rstate[6] ^ temp_state[6];
        rstate[7] = rstate[7] ^ temp_state[7];

        //output R_i+1 of round function (before the exchange with R_i+1)
        lstate[0] = (temp_state[0] & round_key[0]) ^ lstate[0];
	    lstate[1] = (temp_state[1] & round_key[1]) ^ lstate[1];
        lstate[2] = (temp_state[2] & round_key[2]) ^ lstate[2];
	    lstate[3] = (temp_state[3] & round_key[3]) ^ lstate[3];
        lstate[4] = (temp_state[4] & round_key[4]) ^ lstate[4];
	    lstate[5] = (temp_state[5] & round_key[5]) ^ lstate[5];
	    lstate[6] = (temp_state[6] & round_key[6]) ^ lstate[6];
        lstate[7] = (temp_state[7] & round_key[7]) ^ lstate[7];

        //exchange the L and R;
        u = lstate[0];
        lstate[0] = rstate[0];
        rstate[0] = u;

        u = lstate[1];
        lstate[1] = rstate[1];
        rstate[1] = u;

        u = lstate[2];
        lstate[2] = rstate[2];
        rstate[2] = u;
        
        u = lstate[3];
        lstate[3] = rstate[3];
        rstate[3] = u;

        u = lstate[4];
        lstate[4] = rstate[4];
        rstate[4] = u;

        u = lstate[5];
        lstate[5] = rstate[5];
        rstate[5] = u;

        u = lstate[6];
        lstate[6] = rstate[6];
        rstate[6] = u;

        u = lstate[7];
        lstate[7] = rstate[7];
        rstate[7] = u;

    }

	//final output of full round fishingrod
    plain[0] = lstate[0];
	plain[1] = lstate[1];
    plain[2] = lstate[2];
	plain[3] = lstate[3];
    plain[4] = lstate[4];
	plain[5] = lstate[5];
	plain[6] = lstate[6];
    plain[7] = lstate[7];

	plain[8] = rstate[0];
    plain[9] = rstate[1];
    plain[10] = rstate[2];
    plain[11] = rstate[3];
    plain[12] = rstate[4];
    plain[13] = rstate[5];
    plain[14] = rstate[6];
    plain[15] = rstate[7];

}

#endif /*FISHINGROD_H_*/