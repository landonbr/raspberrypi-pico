#include <stdio.h>
#include <stdint.h>

#define POLY 0xD8 // 11011...0000

uint8_t crc8(uint8_t const message)
{
    uint8_t remainder;

    remainder = message;

    for(uint8_t bit=8; bit > 0; bit--) {

        // if msb is 1
        if(remainder & 0x80) {
            remainder ^= POLY;    // xor previous remainder w/ divisor
        }

        remainder = (remainder << 1);
    }

    //return (remainder >> 4);
    return remainder;
}

int main()
{

    uint8_t message = 0xE8;
    
    if(crc8(message) == 0x000000C0) // 0xc -> 000000c0
        printf("%x\n", crc8(message));  // c is the checksum
    else
        printf("checksum failed\n");
    return 0;
}

