#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DATA_ARRAY_LEN 10000
#define END_WITH_ZERO 0

#define NO_FILEPTR_RESET 0
#define DO_FILEPTR_RESET 1


#define DEBUG_STATEMENT_CNT 1
#define DEBUG_MOREINFO 1

uint8_t init();
char* statementArrayInit(FILE* file);

char getInput();
char getNextFileChar(uint8_t reset, FILE* file);

// Debug Var
#if DEBUG_MOREINFO
int64_t maxloop = 0;
#endif

// Global Var
FILE* stateInput;
FILE* IOInputFile;


int main(int argc, char** argv) {

    if (!init()) {
        puts("Init failure");
        return 1;
    }

    char* const statementArray  = statementArrayInit(stateInput);
    char*       statementPtr    = statementArray;
    int8_t*     dataArray       = calloc(DATA_ARRAY_LEN, sizeof(int8_t));
    int8_t*     dataPtr         = dataArray;


    while (*statementPtr != '\0') {

        #if DEBUG_STATEMENT_CNT
        printf("statement: %c\n", *statementPtr);
        #endif

        switch (*statementPtr) {
            case '>':
                dataPtr = (void*)dataPtr + 1 * sizeof(int8_t);
                if (dataPtr - dataArray >= DATA_ARRAY_LEN) {
                    puts("\nError: Out of data array.\n");
                    return 1;
                }
            break;

            case '<':
                dataPtr = (void*)dataPtr - 1 * sizeof(int8_t);
                if (dataPtr < dataArray) {
                    puts("\nError: Out of data array.\n");
                    return 1;
                }
            break;

            case '+':
                ++(*dataPtr);

                #if DEBUG_MOREINFO
                printf("dataIndex, data: %ld, %d\n", dataPtr - dataArray, *dataPtr);
                #endif
            break;

            case '-':
                --(*dataPtr);

                #if DEBUG_MOREINFO
                printf("dataIndex, data: %ld, %d\n", dataPtr - dataArray, *dataPtr);
                #endif
            break;

            case '.':
                putc(*dataPtr, stdout);

                #if DEBUG_MOREINFO || DEBUG_STATEMENT_CNT
                putc('\n', stdout);
                #endif
            break;

            case ',':
                *dataPtr = getInput();

                #if DEBUG_MOREINFO
                printf("storedataIndex, data: %ld, %d\n", dataPtr - dataArray, *dataPtr);
                #endif
            break;

            case '[':
                if (*dataPtr == 0) {
                    while (*statementPtr != ']') {
                        statementPtr++;
                        if (*statementPtr == '\0') {
                            puts("\nError: Out of statement array.\n");
                            return 1;
                        }
                    }
                }
            break;

            case ']':
                if (*dataPtr != 0) {

                    #if DEBUG_MOREINFO
                    maxloop++;
                    printf("looping dataIdx: %d\n", *dataPtr);
                    #endif

                    while (*statementPtr != '[') {
                        statementPtr--;
                        
                        if (statementPtr < statementArray) {
                            puts("\nError: Out of statement array.\n");
                            return 1;
                        }
                    }
                } 
                
                #if DEBUG_MOREINFO
                else {
                    printf("maxloop: %lld\n", maxloop);
                    maxloop = 0;
                }
                #endif

            break;

            default:
                printf("\nError: Unknown statement at index %ld: \'%c\'\nchar code: \'%u\'\n", statementPtr - statementArray, *statementPtr, (uint32_t)*statementPtr);
                return 1;
        }

        statementPtr++;

        /*if ((*(statementPtr - 1) == '+' || *(statementPtr - 1) == '-') && (*(statementPtr) != '+' && *(statementPtr)!= '-')) {
            printf("dataIndex, data: %ld, %d\n", dataPtr - dataArray, *dataPtr);
        }*/
    }

    puts("\nFinish execution.\n");
    return 0;
}

char* statementArrayInit(FILE* file) {
    uint32_t arrayLen = 0;
    for (char buffer = getNextFileChar(DO_FILEPTR_RESET, file); buffer != EOF; buffer = getNextFileChar(NO_FILEPTR_RESET, file)) {
        if (buffer != '\n' && buffer != ' ') arrayLen++;
    }

    char* array = malloc((arrayLen + 1) * sizeof(char));
    char* arrayPtr = array;
    
    for (char buffer = getNextFileChar(DO_FILEPTR_RESET, file); buffer != EOF; buffer = getNextFileChar(NO_FILEPTR_RESET, file)) {
        if (buffer != '\n' && buffer != ' ') *(arrayPtr++) = buffer;
    }

    array[arrayLen] = '\0';
    return array;
}

uint8_t init() {
    stateInput = fopen("input.txt", "r");
    if (stateInput == NULL) return 0;
    IOInputFile = fopen("ioinput.txt", "r");
    if (IOInputFile == NULL) return 0;
    return 1;
}

char getInput() {

    #if END_WITH_ZERO
    char buffer = fgetc(IOInputFile);
    if (buffer == '\n') return 0;
    else return buffer;

    #else
    return fgetc(IOInputFile);
    #endif
}

char getNextFileChar(uint8_t reset, FILE* file) {
    if (reset) fseek(file, 0, SEEK_SET);
    return (char)fgetc(file);
}