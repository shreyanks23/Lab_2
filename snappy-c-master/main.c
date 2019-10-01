#include "snappy.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
    struct snappy_env en;
    snappy_init_env(&en);
    if(snappy_init_env(&en)==0){
        printf("Success");
    }

    char * in = "/home/shreyank/Documents/AOS/Project_2/project_2-master/input/Tiny.txt";
    char *out;
    FILE* fp = fopen(in, "r");
    fseek(fp, 0L, SEEK_END);
    size_t res = ftell(fp);
    fseek(fp,0L,SEEK_SET);

    char*buffer;
    char*uncompressed;
    buffer = (char*)calloc(res, sizeof(char));	
    out = (char*)calloc(res, sizeof(char));	
    uncompressed = (char*)calloc(res, sizeof(char));	
    fread(buffer, sizeof(char), res, fp);
    fclose(fp);
    size_t max_comp_len = snappy_max_compressed_length(sizeof(buffer));
    snappy_compress(&en,buffer, res, out, &max_comp_len);
    size_t uncompressedLength;
    snappy_uncompress(out,max_comp_len,uncompressed);
    printf("The file called test.dat contains this text\n\n%s", uncompressed);

    free(buffer);
    free(out);
    free(uncompressed);

    
    return 0;

}