#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label_table.h"
#include "general.h"
#include "base_conversion.h"





void add_label_to_table(struct label_table **head, char *name, int address, int type) /* create new label and add it to the label table */
{
	struct label_table *new_node, *last;
	
	/* if the linked list is empty, then make the new node as head */
    if (*head == NULL)
    {
       strcpy((*head)->name, name);
       (*head)->address = address;
       (*head)->type = type;
       (*head)->next = NULL;
       return;
    }
     
    /* else, create new node, go through the list until the last node and set the new node last */
    else
    {
    	new_node = (struct label_table*)malloc(sizeof(struct label_table));
 		if(!new_node)
		{
			printf("Error allocating memory, exiting program\n");
			exit(1);
		}  
		
		strcpy(new_node->name, name);
		new_node->address = address;
		new_node->type = type;	
		new_node->next = NULL;
		
    	last = *head;
    	
    	while(last->next != NULL)
        	last = last->next;
        	
        last->next = new_node;

        return;	
    }    	
 
}


int is_label_exists(struct label_table *head, char *label_name) /* check if the label already exists */
{
  struct label_table *cur;
  cur = head;
  while(cur != NULL)  /* check through the label table */
  { 
    if (!strcmp(cur->name, label_name))
    {
      return 1;
    }
    cur = cur->next;
  }
  return 0;
}



void update_data_labels(struct label_table *head, int IC) /* update value of data labels by adding IC */
{
	struct label_table *temp = head;
	while(temp)
	{
		if(temp->type == DATA || temp->type == STRUCT || temp->type == STRING) /* data label */
			temp->address += IC;
		
		temp = temp->next;
	}
}




int update_entry(struct label_table *head, char *name) /* get name of entry label, search it in the table and update it as entry, return 0 for successful update or positive number if unsuccessful */
{
	struct label_table *temp = head;
	while(temp)
	{
		if(strcmp(temp->name,name) == 0) /* label was found */
		{
			if(temp->type == EXTERN)
				return 1; /* label can't be both extern and entry */
			
			temp->is_ent = ON; /* set the label as entry */
			return 0; /* successful update */
		}
		temp = temp->next;
	}
	
	return 2; /* label not found */
}	
				


struct label_table *search_label(struct label_table *head, char *name) /* search label in table and return it if found, otherwise return NULL */
{
	struct label_table *temp = head;
	while(temp)   /* go through the list */
	{
		if(strcmp(temp->name,name) == 0) /* label found */
			return temp;
		temp = temp->next;		
	}
	
	return NULL;
}



int is_ext_exist(struct label_table *head)   /* return 1 if there is at least one extern label, 0 if not */
{
	struct label_table *temp = head;
	while(temp)  /* go through the list */
	{
		if(temp->type == EXTERN)
			return 1;
		temp = temp->next;
	}
	
	return 0;
}
			


int is_ent_exist(struct label_table *head)   /* return 1 if there is at least one entry label, 0 if not */
{
	struct label_table *temp = head;
	while(temp)  /* go through the list */
	{
		if(temp->is_ent == ON)
			return 1;
		temp = temp->next;
	}
	
	return 0;
}	



void free_label_table(struct label_table *head)   /* free all nodes in label table */
{
    struct label_table *tmp = NULL;

	while(head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}

	return;
}





void add_to_externList(struct externList **head, char *name, int address) /* create new extern label and add it to the extern list */
{
	char *unique = NULL;
	struct externList *new_node, *last;
	unique = decimal_to_unique_base(address);
	if(!unique)
	{
		printf("Error allocating memory, exiting program\n");
		exit(1);
	}

	
	/* if the linked list is empty, then make the new node as head */ 
    if (*head == NULL)
    {
       strcpy((*head)->name, name);
       strcpy((*head)->address, unique);
       (*head)->next = NULL;
       free(unique);
       return;
    }
     
    /* else, create new node, go through the list until the last node and set the new node last */ 
    else
    {
    	new_node = (struct externList*)malloc(sizeof(struct externList));
		if(!new_node)
		{
			printf("Error allocating memory, exiting program\n");
			exit(1);
		}
		
		strcpy(new_node->name, name);
		strcpy(new_node->address, unique);
		new_node->next = NULL;
		free(unique);
		
    	last = *head;    	
    	while(last->next != NULL)
        	last = last->next;
        	
        last->next = new_node;	
        return;
    }
        	
}



void free_externList(struct externList *head)   /* free all nodes in extern list */
{
    struct externList *tmp = NULL;

	while(head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}


