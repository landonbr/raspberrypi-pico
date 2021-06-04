#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "crc32.h"

//  usage-- ./makeuf2 <input file> <output file>
//                        {elf}        {uf2}

#if 0
static_assert(sizeof(struct uf2_block) == 512, "uf2_block not sector sized");

#endif
// part of UF2 block
#define UF2_MAGIC_0     0x0A324655u
#define UF2_MAGIC_1     0x9E5D5157u
#define UF2_MAGIC_END   0x0AB16F30u
#define UF2_FLAG_FAMILY_ID   0x00002000u

#define RP2040_FAMILY_ID    0xE48BFF56
#define PAYLOAD_SIZE        0x100
#define BLOCK_NO            0x0
#define NUM_BLOCKS          0x1
#define TARGET              0x10000000

typedef uint32_t crc;
unsigned char data[512];
unsigned int write_data[128];   // this will be filled with 512 bytes of header/data

// read elf file into data array
int elfuf2(FILE *fin, FILE *fout) {

    unsigned int length;
    unsigned int addr;

    /* initialize data memory */
    memset(data, 0, sizeof(data));
    length = fread(data, 1, sizeof(data), fin);

    // last four bytes reserved for checksum
    if(length > 252) {
        fprintf(stderr, "Memory is outside of range: %08x -> %08lx\n", length, sizeof(data));
        return 1;
    }
    printf("Bytes of data read: %u\n", length);

    /* checksum */
    crc remainder = 0xFFFFFFFF; // initial value
    for(addr=0; addr<252; addr++) {
        uint8_t table;

        table = data[addr] ^ (remainder >> 24);
        remainder = crc32Table[table] ^ (remainder << 8);
    }
    printf("Checksum: %u\n", remainder);

    // set last 4 bytes to checksum
    data[addr++] = (remainder >> 0)&0xFF;   // 255
    data[addr++] = (remainder >> 8)&0xFF;   // 254
    data[addr++] = (remainder >> 16)&0xFF;  // 253
    data[addr++] = (remainder >> 24)&0xFF;  // 252

    /* UF2 BLOCK */
    memset(write_data, 0, sizeof(write_data));
    // 32 bytes of header
    write_data[0] = UF2_MAGIC_0;
    write_data[1] = UF2_MAGIC_1;
    write_data[2] = UF2_FLAG_FAMILY_ID;
    write_data[3] = TARGET;
    write_data[4] = PAYLOAD_SIZE;
    write_data[5] = BLOCK_NO;
    write_data[6] = NUM_BLOCKS;
    write_data[7] = RP2040_FAMILY_ID;
    // now load in data including the last 4 bytes of the checksum
    memcpy(&write_data[8], data, sizeof(data));
    write_data[8] = UF2_MAGIC_END;

    //fwrite(write_data, 1, sizeof(write_data), fout);
    if(1 != fwrite(write_data, 1, sizeof(write_data), fout)) {
        fprintf(stderr, "Error writing uf2 block...\n");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {

    const char *in_file = argv[1];
    const char *out_file = argv[2];

    if(argc < 3) {
        fprintf(stderr, "*ERROR* Usage: ./exe <input elf file> <output uf2 file>\n");
    }

    FILE *fin;
    fin = fopen(in_file, "rb");
    if(fin == NULL) {
        fprintf(stderr, "Error opening input file %s\n", in_file);
        return 1;
    }

    FILE *fout;
    fout = fopen(out_file, "wb");
    if(fout == NULL) {
        fprintf(stderr, "Error opening input file %s\n", out_file);
        return 1;
    }

    int rc = elfuf2(fin, fout);
    fclose(fin);
    fclose(fout);

    return rc;
}
