#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macro.h"
#include "checks.h"




int is_macro_or_endmacro(char line[]) /* check if the line is the start or end of a macro definition */
{
  int i = 0 , mcr_index = 0;
  char macro[MAX_LINE_LENGTH];
  while(isspace(line[i])) /* skip white characters */
    i++;
  while(!isspace(line[i]))
    macro[mcr_index++] = line[i++];
  macro[mcr_index] = '\0';
  if (strcmp(macro, "macro") == 0) /* start of marco */
    return 1;
  if (strcmp(macro, "endmacro") == 0) /* end of macro */
    return -1;
  return 0;
}



int add_name(struct Macro *node, char line[]) /* add the macro name to the macro table, return 1 for success, 0 if name was illegal*/
{
  int i = 0 , name_index = 0;
  char name[MAX_LINE_LENGTH];
  while(isspace(line[i])) /* skip white chars until we reach start of macro */
    i++;
    
  while (!isspace(line[i])) /* skip the word 'macro' */
    i++;
    
  while(isspace(line[i])) /* skip white chars until we reach the name of the macro */
    i++;
    
  while(!isspace(line[i]))
    name[name_index++] = line[i++];
    
  name[name_index] = '\0';
  
  if(is_instruction(name)) /* macro name is an instruction name - illegal */
  	return 0;
  	
  if(is_cmd(name) != -1)  /* macro name is a cmd name - illegal */
  	return 0;
  	
  if(is_register(name))   /* macro name is a register name - illegal */
  	return 0;	
  
  strcpy(node->mcr_name,name);   /* if name is legal - copy the macro name to macro node */
  return 1;
}



void add_content(struct Macro *node, FILE *fp) /* add the macro content to the macro table */
{
  char line[MAX_LINE_LENGTH] = {'\0'};
  char content[MAX_LENGTH] = {'\0'};
  fgets(line, MAX_LINE_LENGTH, fp);
  while(is_macro_or_endmacro(line) >= 0) /* as long as it's not the end of the macro */
  {
    strncat(content, line , MAX_LINE_LENGTH);   
    fgets(line, MAX_LINE_LENGTH, fp);
  }
  
  strcpy(node->mcr_content, content);
}



void initial_file_read(struct Macro **head, char *argv[], int i) /* read the file and add the macros to the macro table */
{
  struct  Macro *last = *head;
  char line[MAX_LINE_LENGTH] = {'\0'}, file_name[MAX_LINE_LENGTH] = {'\0'};
  FILE *fp;
  strcpy(file_name, argv[i]);
  strcat(file_name, ".as");
  fp = fopen(file_name,"r");
  if(!fp)
  {
     printf("can not open file: %s, exiting program\n" , argv[i]);  
     exit(1);
  }
  
   while(fgets(line, MAX_LINE_LENGTH, fp))
   {
    if(is_macro_or_endmacro(line) > 0) /* start of macro */
    { 
      	struct  Macro *new_node;
    	new_node = (struct Macro*)malloc(sizeof(struct Macro));
    	if(!new_node)
		{
			printf("Error allocating memory, exiting program\n");
			exit(2);
		}
		
        if(add_name(new_node, line)); /* legal name, then add its content and add to macro table */
        {
        	add_content(new_node, fp);
        	new_node->next = NULL;
        
        	/* if the macro table is empty, then make the new node as head */
   	 		if (*head == NULL)
    		{
       			*head = new_node;
   			}
   		
   			else  /* macro table is not empty */
   			{ 
    			while(last->next != NULL) /* go through the table until the last node */
        			last = last->next;
        	
        		last->next = new_node;
        	}
        }		
    }
   }  
  
  fclose(fp);
  return;
}



int is_macro_name(char line[], FILE *fp, struct Macro *head) /* copy the content of the corresponding macro to a file when a macro is called */
{
  int i = 0, mcr_index = 0, flag = 0;
  char name[MAX_LINE_LENGTH] = {'\0'};
  struct Macro *node = head;

  while(isspace(line[i])) /* skip white chars */
    i++;
    
  while(!isspace(line[i]))
    name[mcr_index++] = line[i++];   /* store the first word of the line in array name */
    
  while(node)
  {
    if (strcmp(node->mcr_name, name) == 0) /* macro name found */
    {
    	while(line[i] != '\n' && flag == 0) /* make sure there is no more text in line */
    	{
    		if(!isspace(line[i])) /* there is additional text beside macro name */
    			flag = 1;
    		else	
    			i++;
    	}
    	
    	if(flag == 0) /* only macro name in line - legal macro call */
    	{
    		fprintf(fp, "%s", node->mcr_content);
    		return 1;
    	}			
    }
    
    flag = 0;			
    node = node->next;
  }
  
  return 0;
}



void deploy_macro(struct Macro *head, char *argv[], int i) /* deploy the macros from the initial .as file to the .am file */
{
  FILE *wfp;
  FILE *rfp;
  int flag = 0;  /* flag that represents whether we are inside(1) or outside(0) macro definition */
  char line[MAX_LINE_LENGTH] = {'\0'};
  char file_name[MAX_LINE_LENGTH] = {'\0'};
  char file_name2[MAX_LINE_LENGTH] = {'\0'};
  strcpy(file_name, argv[i]);
  strcat(file_name, ".as");
  strcpy(file_name2, argv[i]);
  strcat(file_name2, ".am");
  rfp = fopen(file_name,"r");
  wfp = fopen(file_name2,  "w");

  if(rfp && wfp)
  {	 
  	while(fgets(line, MAX_LINE_LENGTH, rfp))
  	{
    	if(!flag) /* outside macro */
    	{   
       		if(is_macro_name(line, wfp, head) == 0) /* not a macro call */
       		{
         		if(is_macro_or_endmacro(line) == 0) /* not a start or end of macro */
            		fprintf(wfp,"%s",line); /* copy the line */
         		
         		else /* start or end of macro */
        		{
            		flag = 1;
         		}
       		}
    	}
    
    	else /* inside macro */
    	{
      		if(is_macro_or_endmacro(line) < 0) /* end of macro definition */
        		flag = 0;
    	}        
  	}
  }
  
  else
  {
 	printf("can not open files, exiting program\n");
  	exit(1);
  } 
  
  fclose(rfp);
  fclose(wfp);
  return;
}


void free_macro_list(struct Macro *head)   /* free all nodes in macro list */
{
    struct Macro *tmp = NULL;

	while(head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

