// TODO: read 2.8.4.2. UF2 format for file implementation
// more crc code implementation
// also what is going into write data[]

#include <stdio.h>
#include "crc32.h"

//  this file will work as follows after it is compiled
//  ./makeuf2 <input file> <output file>
//              {bin}           {uf2}


// NOTE: check docs buffer for reading the input file


int bin_uf2(FILE *in, FILE *out) {

}

int main(int argc, char **argv) {

    const char *in_file = argv[1];
    const char *out_file = argv[2];

    if(argc < 3) {
        fprintf(stderr, "*ERROR* Usage: ./exe <input bin file> <output uf2 file>\n");
    }

    FILE *fin;
    in = fopen(in_file, "rb"); 
    if(fin == NULL) {
        fprintf(stderr, "Error opening input file %s\n", in_file);
    }
    // TODO: read input data - make sure it is not bigger than 252 bytes bc 4 bytes is needed by the checksum at the end



    return 0;
}
