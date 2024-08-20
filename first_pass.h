#ifndef _FIRST_PASS_H
#define _FIRST_PASS_H

#include <stdio.h>
#include "label_table.h"
#include "data.h"





/**
* Performs the first scan on the file according to the algorithm given in the task.
* @param label_head The head of the label table
* @param data_head The head of the data list
* @return 0 for success, 1 if errors occured
*/
int first_pass(FILE *fp, struct label_table *label_head, struct dataList *data_head);





#endif
