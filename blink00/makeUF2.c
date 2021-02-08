#include <stdio.h>
#include "crc32.h"

//  usage-- ./makeuf2 <input file> <output file>
//                        {elf}        {uf2}

const char write_data[128];
const char data[512];


#if 0
int elf_uf2(FILE *in, FILE *out) {

}
#endif

// setting up data to be read into uf2
int elfIn(FILE *fin, char *in_file) {

    unsigned int length;

    fin = fopen(in_file, "rb"); 
    if(fin == NULL) {
        fprintf(stderr, "Error opening input file %s\n", in_file);
    }

    // initialize data memory
    memset(data, 0, sizeof(data));
    length = fread(data, 1, sizeof(data), fin);
    fclose(fin);

    // last four bytes reserved for checksum
    if(length > 252) {
        fprintf(stderr, "Memory is outside of range: %08x -> %08x\n", data, sizeof(data));
    }

    return data;
}

// TODO: checksum function

//TODO: uf2Out function

int main(int argc, char **argv) {

    const char *in_file = argv[1];
    const char *out_file = argv[2];

    if(argc < 3) {
        fprintf(stderr, "*ERROR* Usage: ./exe <input elf file> <output uf2 file>\n");
    }

    return 0;
}
