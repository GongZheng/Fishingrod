/*
 *  LUT_Fishingrod.h
 *
 *  Description: Fishingrod-128 with a look-up table (LUT) structure.
 *  Created on: 2020-12-28
 *  Coding: Zheng Gong, Jinhai Chen
 */

#ifndef LUT_FISHINGROD_H_
#define LUT_FISHINGROD_H_

//comment this out if this is used on PC
//#define __UINT_T__
#ifndef __UINT_T__
#define __UINT_T__
typedef unsigned char uint8_t;
typedef short int int8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
#endif /*__UINT_T__*/

//rounds of Fishingrod;
#define ROUNDS 18

//The LFSR function D used in the key schedule;
uint32_t lfsr(uint32_t round_key_value)
{
    return (round_key_value ^ 
    ((round_key_value << 2) ^ (round_key_value >> 30)) ^ 
    ((round_key_value << 11) ^ (round_key_value >> 21)));
}

void fishingrod_key_schedule(const uint32_t *master_key, uint64_t *round_key) {
    uint32_t k[ROUNDS];
    int i;
    k[0] = master_key[0];
    k[1] = master_key[1];
    k[2] = master_key[2];
    k[3] = master_key[3];

    for(i=0;i+4<ROUNDS;i++) {
        k[i+4] = lfsr(k[i+3]^k[i]);
    }

    for(i=0;i<ROUNDS;i+=2) {
        round_key[i] = k[i+1];
        round_key[i] <<= 32;
        round_key[i] |= k[i];
        round_key[i+1] = ~round_key[i];
    }
}


void fishingrod_encrypt_rounds(const uint8_t *plain, const uint8_t *key, const uint8_t rounds, uint8_t *cipher)
{


}

#endif /*LUT_FISHINGROD_H_*/