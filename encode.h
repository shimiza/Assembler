#ifndef _ENCODE_H
#define _ENCODE_H


#include "label_table.h"
#include "code.h"
#include "data.h"




 /**
 * Encodes cmd line and adds the encoding to the code list.
 * @param label_head A pointer to the head of the label table
 * @param extern_head A pointer to the head of the extern list
 * @param code_head A pointer to the head of the code list
 * @param cmd_num The number of the command
 * @param line The line of text
 * @param index The current index
 * @param IC A pointer to the instruction counter
 * @return 0 for success, positive number(represented by error type) for failure
 */	
int convert_cmd_line_to_code(struct label_table *label_head, struct externList *extern_head, struct codeList *code_head, int cmd_num, char *line, int index, int *IC);



 /**
 * converts codes of words from decimal to binary and adds them to the code list.
 * @param code_head A pointer to the head of the code list
 * @param arr An unsigned int array of size 5(the max number of words for each machine order) that stores the binary code of every word of machine order 
 * @param size The number of words(of the machine order)
 * @param IC A pointer to the instruction counter
 */	
void add_code_to_DB(struct codeList *code_head, unsigned int arr[],int size,int *IC);



 /**
 * Adds the data list to the code list.
 * @param IC The instruction counter
 * @param code_head A pointer to the head of the code list
 * @param data_head A pointer to the head of the data list
 */	
void data_to_code(int IC, struct codeList *code_head, struct dataList *data_head);



#endif
