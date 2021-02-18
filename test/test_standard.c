#include <stdio.h>
#include <string.h>
#include "../Standard_C/Fishingrod.h"

#ifdef __GNUC__
#include <x86intrin.h>
#endif
#ifdef _MSC_VER
#include <intrin.h>
#endif
#pragma intrinsic(__rdtsc)

//Repeat test times and calculate on average for accuracy
#define TEST 1000000

//CPU cycles set start;
uint64_t start_rdtsc()
{
	return __rdtsc();
}

//CPU cycles set end;
uint64_t end_rdtsc()
{
    return __rdtsc();
}

int main() {
    uint32_t master_key[4] = {0x00000001, 0x00000111, 0x00000000, 0x000000000};
    uint32_t round_key[ROUNDS*2];
    fishingrod_key_schedule(master_key, round_key);


    uint8_t plain[16] = {
        0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f 
    };
    uint8_t cipher[16];
    uint8_t plain2[16];


    fishingrod_encrypt_rounds(plain, round_key, cipher);
    for(int i=0;i<16;i++) printf("%02x ", cipher[i]); printf("\n\n");
    // c9 96 9d 8e ea bb d6 fa a7 c6 5a 4c f6 8b fa 3f

    fishingrod_decrypt_rounds(cipher, round_key, plain2);
    for(int i=0;i<16;i++) printf("%02x ", plain2[i]); printf("\n");


    uint64_t begin, end, ans;

    begin = start_rdtsc();
    for(int i=0;i<TEST;i++) 
        fishingrod_encrypt_rounds(plain, round_key, cipher);
    
    end = end_rdtsc();
    ans = end - begin;
    printf("cost %lu CPU cycles\n", ans/TEST);


    begin = start_rdtsc();
    for(int i=0;i<TEST;i++) 
        fishingrod_key_schedule(master_key, round_key);

    
    end = end_rdtsc();
    ans = end - begin;
    printf("KS cost %lu CPU cycles\n", ans/TEST);

    return 0;
}