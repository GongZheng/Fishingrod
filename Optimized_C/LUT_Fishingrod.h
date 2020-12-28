/*
 *  LUT_Fishingrod.h
 *
 *  Description: Fishingrod-128 with a look-up table (LUT) structure.
 *  Created on: 2020-12-28
 *  Last modified: 2020-12-28
 *  Coding: Zheng Gong
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
#define ROUNDS 12

//Multiply * 2 without using LUT
uint8_t xtime(uint8_t input)
{
    return (input<<1) ^ (((input>>7) & 1) * 0x1b);
	
	//if((input & 0x80) == 0)
	//   return (input << 1) ^ 0x00;
	//else
	//   return (input << 1) ^ 0x1B;
}

#define fishingrod_encrypt(plain, key, cipher) fishingrod_encrypt_rounds((plain), (key), ROUNDS, (cipher))
#define fishingrod_decrypt(cipher, key, plain) fishingrod_decrypt_rounds((cipher), (key), ROUNDS, (plain))
