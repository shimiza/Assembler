#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include "label_table.h"
#include "code.h"
#include "data.h"
#include "macro.h"





/**
* Skips spaces and tabs in a string.
* @param str The string
* @param index The current index
* @return the index after skipping spaces and tabs
*/
int skip_white_chars(char *str, int index); /* skip spaces and tabs in a string */



/**
* Gets line and starting index and returns the next word(every character from starting index until ':' or '.' or ',' or white car).
* @param line The line of string
* @param index The starting index
* @return the word
*/
char *get_word(char *line, int index);



/**
* Receives file name and create the 3 required output files(if necessary).
* @param file_name The name of the file. 
* @param label_head The head of the lable table
* @param code_head The head of the code list
* @param extern_head The head of the extern list
* @param data_head The head of the data list
* @return 1 for success, -1 for failure
*/
int create_output_files(char *file_name, struct label_table *label_head, struct codeList *code_head, struct externList *extern_head, struct dataList *data_head);



/**
* Free all dynamic memory allocated .
* @param label_head The head of the lable table
* @param code_head The head of the code list
* @param data_head The head of the data list
* @param extern_head The head of the extern list
* @param Mhead The head of the macro table
*/
void freeMemory(struct label_table *label_head, struct codeList *code_head, struct dataList *data_head, struct externList *extern_head, struct Macro *Mhead);



/**
* Finds the last quotation mark in line from the given starting index and return its index.
* @param line The line of string
* @param index The starting index
* @return the number of the index of the last quotation mark, -1 if not found
*/
int find_last_quotation_mark(char *line, int index);



#endif
