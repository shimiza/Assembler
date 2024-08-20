#ifndef _GENERAL_H
#define _GENERAL_H

#include <stdio.h>
#define MAX_LABEL_LENGTH 30
#define MAX_LINE_LENGTH 81
#define MAX_FILENAME_LENGTH 2048


extern char digits[32];

extern char *cmd[17];


extern char *reg[9]; 


extern char *instructions[6];



/* common error types */
enum errors { MISSING_COMMA = 1, ILLEGAL_NUM, NUM_OUT_OF_RANGE, MISSING_NUM, ILLEGAL_STRING, EXTRANEOUS_TEXT, MISSING_OPERAND, ILLEGAL_METHOD, LABEL_NOT_EXIST, WRONG_LABEL_TYPE, ILLEGAL_FIELD_NUM };


enum types { DATA = 1, STRUCT, STRING, ENTRY, EXTERN, CODE };

enum flags { OFF = 0, ON };


#endif
