#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "code.h"
#include "base_conversion.h"


void add_to_code_list(struct codeList **head, int address, char *unique) /* add a code to the code list */
{
	struct codeList *tmp, *new;
    
    if(*head == NULL)   /* list is empty */
    {
        (*head)->address = address;
        strcpy((*head)->base32, unique);
        (*head)->next = NULL;
        return;
    }
    
    /* else, create new node, go through the list until the last node and set the new node last */ 
    else
    {
    	new = (struct codeList*)malloc(sizeof(struct codeList));
    	if(!new)
    	{
    		printf("Error allocating memory, exiting program\n");
    		exit(1);
    	}
    	
    	new->address = address;
    	strcpy(new->base32, unique);
    	new->next = NULL;
    	
    	tmp = *head;
    	while(tmp->next != NULL) 
        	tmp = tmp->next;
        
    	tmp->next = new;
    	return;
    }
}





char *get_code(struct codeList **head) /* convert address from the head of code list to base32, update the head to be the next node and return char array consisted of 2 chars of base32 address, tab, and 2 chars of base32 code */
{
    char *converted, *str_temp, *unique;
    struct codeList *tmp = *head;
    
    converted = (char *)malloc(6*sizeof(char));
    if(!converted)
        return NULL;
    
    unique = decimal_to_unique_base((tmp)->address);  
    if(!unique)
        return NULL; 
     
    (*head) = (*head)->next;   /* remove the first node in code list */
    memcpy(converted, unique, 2);
    converted[2]='\t';
    str_temp = converted + 3;   /* set str_temp to point on the third char in converted */
    memcpy(str_temp ,tmp->base32, 2);
    converted[5]='\0';
    free(unique);
    return converted;
}




void free_code_list(struct codeList *head)   /* free all nodes in code list */
{
    struct codeList *tmp = NULL;

	while(head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}


