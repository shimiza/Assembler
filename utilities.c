#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utilities.h"
#include "base_conversion.h"
#include "general.h"






int skip_white_chars(char *str, int index) /* skip spaces and tabs */
{
	while(str[index] == ' ' || str[index] == '\t')
		index++;
	return index;
}



char *get_word(char *line, int index) /* get line and starting index and return the next word */
{
	int i = 0;
	char *word;
	word = (char*)malloc((MAX_LINE_LENGTH*2) * sizeof(char));
	if(!word)
	{
		printf("Error allocating memory, exit program.\n");
		exit(1);
	}
	
	word[0] = '\0';
	index = skip_white_chars(line, index);
	while(!isspace(line[index]) && line[index] != ',' && line[index] != ':' && line[index] != '.')
		word[i++] = line[index++];
		
	word[i] = '\0';
	return word;
}		



int create_output_files(char *file_name, struct label_table *label_head, struct codeList *code_head, struct externList *extern_head, struct dataList *data_head) /* receive file name and create the 3 required output files (if necessary), return 1 if ok, -1 for error */
{
	int dc = 0, ic, count = 0;
	char fname_ob[MAX_FILENAME_LENGTH] = {'\0'}, fname_ent[MAX_FILENAME_LENGTH] = {'\0'},fname_ext[MAX_FILENAME_LENGTH] = {'\0'};
	FILE *fp;
	struct codeList *code_tmp = code_head->next;
	struct label_table *label_tmp;
	struct externList *ext_tmp;
	struct dataList *data_tmp = data_head->next;
	
	/* get the number of DC and IC */
	while(data_tmp)
	{
		data_tmp = data_tmp->next;
		dc++;
	}	
	data_tmp = NULL;

	while(code_tmp)   
	{
		code_tmp = code_tmp->next;
		count++;
	}	
	code_tmp = NULL;
	ic = count - dc;
	
	
	if(code_head != NULL) /* code list isn't empty */
	{
		strcpy(fname_ob, file_name);
		strcat(fname_ob, ".ob");
		fp = fopen(fname_ob,"w");
		if(fp)
		{
			char *ic_unique = NULL, *dc_unique = NULL;
			ic_unique = decimal_to_unique_base(ic);
			if(!ic_unique)
			{
				printf("Error allocating memory, exit program.\n");
				exit(1);
			}	
			
			dc_unique = decimal_to_unique_base(dc);
			if(!dc_unique)
			{
				printf("Error allocating memory, exit program.\n");
				exit(1);
			}
			
			fprintf(fp,"%s\t%s\n", ic_unique, dc_unique);
			free(ic_unique);
			free(dc_unique);
			code_tmp = code_head->next;
			while(code_tmp) /* as long as the code list isn't empty we keep writing the output in the ob file */
			{
				char *text_to_print = NULL;
				text_to_print = get_code(&code_tmp);
				if(!text_to_print)
				{
					fclose(fp);
					return -1;
				}
				
				fprintf(fp,"%s\n",text_to_print);
				free(text_to_print);
			}
			fclose(fp);
		}
		
		else
			return -1;
	}
	
	if(is_ext_exist(label_head))  /* there is at least one extern label */
	{
		strcpy(fname_ext, file_name);
		strcat(fname_ext, ".ext");
		if(fp = fopen(fname_ext,"w"))
		{
			ext_tmp = extern_head->next;
			while(ext_tmp)   /* go through extern list */
			{
					fprintf(fp, "%s\t%s\n",ext_tmp->name, ext_tmp->address);  /* print the extern label to .ext file */
					ext_tmp = ext_tmp->next;
			}
			
			fclose(fp);
		}
		
		else
			return -1;
	}
	
	if(is_ent_exist(label_head))  /* there is at least one entry label */
	{
		strcpy(fname_ent, file_name);
		strcat(fname_ent, ".ent");
		if(fp = fopen(fname_ent,"w"))
		{
			char *unique = NULL;
			label_tmp = label_head;
			while(label_tmp)   /* go through label list */
			{
				if(label_tmp->is_ent == ON)   /* entry label */
				{
					unique = decimal_to_unique_base(label_tmp->address);
					if(!unique)
					{
						printf("Error allocating memory, exit program.\n");
						exit(1);
					}
					
					fprintf(fp, "%s\t%s\n",label_tmp->name, unique);  /* print the entry label to .ent file */
					free(unique);
				}	
				
				label_tmp = label_tmp->next;
			}
			
			fclose(fp);
		}
		
		else
			return -1;
	}
	
	return 1;
}						
							
				

void freeMemory(struct label_table *label_head, struct codeList *code_head, struct dataList *data_head, struct externList *extern_head, struct Macro *Mhead) /* free all dynamic memory allocated */
{
	free_data_list(data_head);
	free_label_table(label_head);
	free_code_list(code_head);
	free_externList(extern_head);
	free_macro_list(Mhead);
}



int find_last_quotation_mark(char *line, int index) /* find the last quotation mark in line from the starting index and return its index. return -1 if not found */
{
	int last = -1;
	while(line[index] != '\n')
	{
		if(line[index] == '\"') /* quotation mark found */
			last = index;
		
		index++;
	}

return last;			
}


