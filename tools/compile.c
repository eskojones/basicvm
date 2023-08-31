#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "vm.h"


typedef struct {
    uint16_t entry;
    uint8_t bytes[65535];          //bytes compiled
    int bytes_length;           //length of bytes
    char label_names[1024][32]; //names of labels defined
    uint16_t label_addr[1024];  //address of the label
    int label_count;            //number of labels defined
} CompilerState;

CompilerState state;


int strtrim(char *str) {
    unsigned int i = 0, k = 0, l = strlen(str);
    char *trimmed = (char *)malloc(l + 1);
    memset(trimmed, 0, l + 1);
    while (str[i] == ' ' || str[i] == '\t') i++;
    for (int j = i; j < l; j++) trimmed[k++] = str[j];
    memcpy(str, trimmed, k + 1);
    free(trimmed);
    return k;
}

unsigned int strfilter(char *str, char *filters) {
    unsigned int l = strlen(str), kl = strlen(filters);
    char *filtered = (char *)malloc(l + 1);
    memset(filtered, 0, l + 1);
    int j = 0;
    for (int i = 0; i < l; i++) {
        char ch = str[i];
        int invalid = 0;
        for (int k = 0; k < kl; k++) {
            if (ch == filters[k]) {
                invalid = 1;
                break;
            }
        }
        if (invalid == 0) filtered[j++] = ch;
    }
    filtered[j] = 0;
    for (int i = 0; i < j; i++) str[i] = filtered[i];
    str[j] = 0;
    free(filtered);
    return j;
}

void compile (struct VM *vm, char *input, bool labels_only) {
    int word_count = 0;
    const char space[3] = { ' ', '\t', '\0' };
    char *words = (char *)malloc(1024);
    memset(words, 0, 1024);
    char *word = strtok(input, space);
    while (word != NULL) {
        if (strlen(word ) == 0 || word[0] == ';') break;
        strfilter(word, ",\t\n\r\b");
        snprintf((char *)(words + ((word_count) * 32)), 31, "%s", word);
        word = strtok(NULL, space);
        word_count++;
    }

    char *instruction = (word_count > 0 ? words : NULL);
    char *arg1 = " ", *arg2 = " ",
         *arg1_original = (char *)malloc(32),
         *arg2_original = (char *)malloc(32);
    char smode = ' ', dmode = ' ';
    int sbase = 10, dbase = 10;

    if (word_count > 1) {
        arg1 = (char *)(words + 32);
        arg2 = (char *)(words + 64);
        if (arg1[0] == '.') {
            //replace arg with label
            int label_idx = -1;
            for (int i = 0; i < state.label_count; i++) {
                if (strcmp(state.label_names[i], arg1) == 0) label_idx = i;
            }
            sprintf(arg1, "$%04xh", state.label_addr[label_idx]);
        }
        if (arg2[0] == '.') {
            //replace arg with label
            int label_idx = -1;
            for (int i = 0; i < state.label_count; i++) {
                if (strcmp(state.label_names[i], arg2) == 0) label_idx = i;
            }
            sprintf(arg2, "$%04xh", state.label_addr[label_idx]);
        }
        memcpy(arg1_original, arg1, 32);
        memcpy(arg2_original, arg2, 32);


        int len = strlen(arg1);
        bool arg_ptr = (strfilter(arg1, "[]") != len);
        len = strlen(arg1);
        bool arg_imm = (strfilter(arg1, "$") != len);
        len = strlen(arg1);
        if (strfilter(arg1, "h") != len) dbase = 16;
        len = strlen(arg1);
        bool arg_reg = (strfilter(arg1, "rR") != len);
        len = strlen(arg1);
        bool arg_mem = (strfilter(arg1, "()") != len);
        if      (arg_mem) dmode = 'm';
        else if (arg_imm) dmode = 'i';
        else if (arg_reg) dmode = 'r';
        else if (arg_ptr) dmode = 'p';
        else dmode = ' ';
    }
    if (word_count == 3) {
        int len = strlen(arg2);
        bool arg_ptr = (strfilter(arg2, "[]") != len);
        len = strlen(arg2);
        bool arg_imm = (strfilter(arg2, "$") != len);
        len = strlen(arg2);
        if (strfilter(arg2, "h") != len) sbase = 16;
        len = strlen(arg2);
        bool arg_reg = (strfilter(arg2, "rR") != len);
        len = strlen(arg2);
        bool arg_mem = (strfilter(arg2, "()") != len);
        if      (arg_mem) smode = 'm';
        else if (arg_imm) smode = 'i';
        else if (arg_reg) smode = 'r';
        else if (arg_ptr) smode = 'p';
        else smode = ' ';
    }

    //if the line was anything useful
    if (word_count > 0 && strlen(instruction)) {
        //if there is no 2nd argument
        if (word_count == 2) {
            smode = dmode;
            dmode = ' ';
            char *tmp = arg2;
            arg2 = arg1;
            arg1 = tmp;
        }
        if (labels_only && instruction[0] == '.') {
            //define a label
            strfilter(instruction, ".:");
            sprintf(state.label_names[state.label_count], "%s", instruction);
            state.label_addr[state.label_count] = state.bytes_length;
            printf("Created label '%s' at 0x%04x\n",
                   state.label_names[state.label_count],
                   state.label_addr[state.label_count]);
            state.label_count++;
        } else if (instruction[0] != '.') {
            int opcode = OPCODE(vm, instruction, smode, dmode);
            //printf("'%s' '%c' '%c' '%s' '%s'\n", instruction, smode, dmode, arg1, arg2);
            int src = 0, dst = 0;
            char *end;
            dst = strtol(arg1, &end, dbase);
            if (word_count > 2) src = strtol(arg2, &end, sbase);
            int dsize = 0, ssize = 0;
            if (dmode == 'r') dsize += 1;
            else if (dmode == 'i') dsize += 2;
            else if (dmode == 'm') dsize += 2;
            else if (dmode == 'p') dsize += 2;

            if (smode == 'r') ssize += 1;
            else if (smode == 'i') ssize += 2;
            else if (smode == 'm') ssize += 2;
            else if (smode == 'p') ssize += 2;

            if (!labels_only) {
                int offset = state.bytes_length;
                state.bytes[offset++] = opcode;
                if (dsize == 2) {
                    state.bytes[offset++] = HBYTE(dst);
                    state.bytes[offset++] = LBYTE(dst);
                } else if (dsize == 1) {
                    state.bytes[offset++] = LBYTE(dst);
                }
                if (ssize == 2) {
                    state.bytes[offset++] = HBYTE(src);
                    state.bytes[offset++] = LBYTE(src);
                } else if (ssize == 1) {
                    state.bytes[offset++] = LBYTE(src);
                }
            }
            printf("0x%04x %s(%c%c)=%02x dst(%02x %02x) src(%02x %02x) size(%d %d)\n",
                   state.bytes_length, instruction, smode, dmode, opcode,
                   HBYTE(dst), LBYTE(dst), HBYTE(src), LBYTE(src), ssize, dsize);
            state.bytes_length += ssize + dsize + 1;
        }
    }

    free(words);
    free(arg1_original);
    free(arg2_original);

    if (labels_only == true) {
        state.bytes_length = 0;
    }
}


int main (int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s <input.file>\n", argv[0]);
        return 0;
    }
    struct VM vm;

    printf("compiling...\n");
    memset(&state, 0, sizeof(CompilerState));
    vm_init(&vm);

    char *line = (char *)malloc(256);
    FILE *src_fp;

    //read the file first just to grab labels
    src_fp = fopen(argv[1], "r");
    while (feof(src_fp) == 0) {
        size_t len = 256;
        memset(line, 0, 256);
        getline(&line, &len, src_fp);
        compile(&vm, line, true);
    }
    fclose(src_fp);

    //read the file again to compile
    src_fp = fopen(argv[1], "r");
    while (feof(src_fp) == 0) {
        size_t len = 256;
        memset(line, 0, 256);
        getline(&line, &len, src_fp);
        compile(&vm, line, false);
    }
    fclose(src_fp);

    printf("const char program = {\n    ");
    for (int i = 0; i < state.bytes_length; i++) {
        printf("0x%02x", state.bytes[i]);
        if (i < state.bytes_length - 1) printf(", ");
        if (i % 8 == 7) printf("\n    ");
    }
    printf("\n};");

//    FILE *dst_fp = fopen(argv[2], "wb+");
//    fwrite(state.bytes, state.bytes_length, 1, dst_fp);
//    fclose(dst_fp);
    return 0;
}
