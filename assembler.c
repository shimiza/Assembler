#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "macro.h"
#include "first_pass.h"
#include "second_pass.h"
#include "utilities.h"
#include "general.h"


int main(int argc, char *argv[])
{	
	int error = 0;
	char file_name[MAX_FILENAME_LENGTH] = "";
	FILE *ifp;
	struct Macro *mcr_head = NULL;
	struct label_table *label_head = NULL;
	struct codeList *code_head = NULL;
	struct dataList *data_head = NULL;
	struct externList *extern_head = NULL;
	
	
	if(argc == 1) /* no argument has been typed except the name of the program */
	{
		printf("Error: no file name has been entered, exiting program\n");
		exit(1);
	}
	
	else if(argc > 2) /* more the one file name has been typed */
	{
		printf("Error: please enter exactly one file name\n");
		exit(2);
	}	

	mcr_head = (struct Macro*)malloc(sizeof(struct Macro));
	
	label_head = (struct label_table*)malloc(sizeof(struct label_table));
	
	code_head = (struct codeList*)malloc(sizeof(struct codeList));
	
	data_head = (struct dataList*)malloc(sizeof(struct dataList));
	
	extern_head = (struct externList*)malloc(sizeof(struct externList));
	
	if(mcr_head && label_head && code_head && data_head && extern_head)
	{
		if(strlen(argv[1]) > MAX_FILENAME_LENGTH - 5)
		{
			printf("Error: file name is too long, exiting program\n");
			exit(2);
		}
			
		initial_file_read(&mcr_head, argv, 1);
		deploy_macro(mcr_head, argv, 1);

		strcpy(file_name, argv[1]);
		strcat(file_name, ".am");
		
		printf("  File: %s  \n",file_name);
		
		ifp = fopen(file_name, "r");
		if(ifp)
		{
			error = first_pass(ifp, label_head, data_head);
			if(error)
			{
				printf("Errors occured in file %s, therefore output files could not be created.\n",file_name);
				fclose(ifp);
				freeMemory(label_head, code_head, data_head, extern_head, mcr_head);
				return 1;
			}

			rewind(ifp);
			error = second_pass(ifp, label_head, code_head, data_head, extern_head);
			
			if(error)
			{
				printf("Errors occured in file %s, therefore output files could not be created.\n",file_name);
				fclose(ifp);
				freeMemory(label_head, code_head, data_head, extern_head, mcr_head);
				return 2;
			}
				
			if(create_output_files(argv[1], label_head, code_head, extern_head, data_head) != 1)
			{
				printf("Error: failed to create output files to: %s\n",file_name);
				freeMemory(label_head, code_head, data_head, extern_head, mcr_head);
			}
				
			else
			{
				printf("Successfully created all output files for %s\n\n",file_name);
				freeMemory(label_head, code_head, data_head, extern_head, mcr_head);
			}
				
			fclose(ifp);
		}
		
		else
		{
			printf("Error: can not open file %s.am, exiting program\n", argv[1]);
			exit(3);	
		}
	}
	
	else
	{
		printf("Error allocating memory, exiting program\n");
    	exit(4);
	}		
	
	return 0;
}		

