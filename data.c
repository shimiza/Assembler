#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "data.h"
#include "utilities.h"
#include "general.h"




void add_to_data_list(struct dataList **head, int number, int *DC)   /* add data information to data list */
{
	struct dataList *tmp, *new;

    if(*head == NULL)   /* list is empty */
    {
        (*head)->data = number;
        (*head)->DC = *DC;
        (*head)->next = NULL;
        (*DC)++;
        return;
    }
    
    /* else, create new node, go through the list until the last node and set the new node last */ 
    else
    {
    	new = (struct dataList*)malloc(sizeof(struct dataList));
    	if(!new)
    	{
    		printf("Error allocating memory\n");
    		exit(1);
    	}
    	
    	new->data = number;
    	new->DC = *DC;
    	new->next = NULL;
    	(*DC)++;
    	
    	tmp = *head;
    	while(tmp->next != NULL) 
        	tmp = tmp->next;
        
    	tmp->next = new;
    	return;
    }
}




int add_data(char *line, int index, struct dataList *head, int *DC) /* after .data instruction, check for legal numbers and syntax and add the numbers to the data list. return 0 if legal, positive number for errors */
{
    int number, num_len, num_cnt = 0, i;
    char num[MAX_LINE_LENGTH] =	{'\0'};
    
    index = skip_white_chars(line, index);
    if(line[index] == '\n') /* no text after .data */
    	return MISSING_NUM;
    
    while(line[index] != '\n') /* run until the end of line */
    {
    	i = 0;
    	num_len = 0;	
    	index = skip_white_chars(line, index);
    	if((line[index] == '-' || line[index] == '+'))
    		num[i++] = line[index++];
    		
    	while(isdigit(line[index]))
    	{
    		num[i++] = line[index++];
        	num_len++;
    	}
    	
    	if(num_len == 0) /* length of number must be at least 1 */
    		return ILLEGAL_NUM; /* not a number */
    		
    	num[i] = '\0';
    	number = atoi(num);

    	if(number > 511 || number < -512) /* signed int range for 10 bits machine */
        	return NUM_OUT_OF_RANGE;
        	
    	add_to_data_list(&head, number, DC); /* add the number to the data list */
    	num_cnt++;
    	index = skip_white_chars(line, index);
    	if(line[index] != '\n') /* if not end of line than expecting a comma */
    	{
    		if(line[index] != ',') /* no comma */
    			return MISSING_COMMA;
    		else
    			index++; /* skip the comma */	
    	}		
    }
    return 0;	
}



int add_string(char *line, int index, struct dataList *head, int *DC) /* after .string instruction, check for legal string and add it to the data list. return 0 if legal, positive number for errors */
{
	int last;
	index = skip_white_chars(line, index);
	
	if(line[index] != '\"') /* string must start with " */
		return ILLEGAL_STRING;
		
	index++;
	last = find_last_quotation_mark(line, index);
	if(last == -1)
		return ILLEGAL_STRING; /* string must end with " */
	
	while(index != last)
	{
		add_to_data_list(&head, line[index], DC); /* add the char to the data list */	
		index++;
	}	
		
	add_to_data_list(&head, '\0', DC);
	index++; /* set index to point after the last "  */
	index = skip_white_chars(line, index);
	
	if(line[index] != '\n') /* there should be no text after end of string */
		return EXTRANEOUS_TEXT;
		
	return 0;
}



int add_struct(char *line, int index, struct dataList *head, int *DC) /* after .struct instruction, check for legal struct and add it to the data list. return 0 if legal, positive number for errors */
{
	int number, num_len = 0, i = 0, res;
    char num[MAX_LINE_LENGTH] = {'\0'};
    
	index = skip_white_chars(line, index);
	
	if((line[index] == '-' || line[index] == '+'))
    	num[i++] = line[index++];
    	
	while(isdigit(line[index]))
	{
		num[i++] = line[index++];
		num_len++;
	}
	
	if(num_len == 0) /* length of number must be at least 1 */
    	return ILLEGAL_NUM; 
    	
    num[i] = '\0';
    number = atoi(num);
    
    if(number > 511 || number < -512) /* signed int range for 10 bits machine */
        return NUM_OUT_OF_RANGE;
        
	add_to_data_list(&head, number, DC); /* add the number to the data list */
	index = skip_white_chars(line, index);
	
    if(line[index] != ',') /* expecting a comma after number */
        return MISSING_COMMA;
        
    index++;
    res = add_string(line, index, head, DC); /* add the string to the data list */
    	return res;
}    	



void free_data_list(struct dataList *head)   /* free all nodes in data list */
{
    struct dataList *tmp = NULL;

	while(head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}


