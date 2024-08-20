#ifndef _SECOND_PASS_H
#define _SECOND_PASS_H

#include <stdio.h>
#include "label_table.h"
#include "code.h"
#include "data.h"






/**
* Performs the second scan on the file according to the algorithm given in the task.
* @param label_head The head of the label table
* @param code_head The head of the code list
* @param data_head The head of the data list
* @param extern_head The head of the extern list
* @return 0 for success, 1 if errors occured
*/
int second_pass(FILE *fp, struct label_table *label_head, struct codeList *code_head, struct dataList *data_head, struct externList *extern_head);



#endif
