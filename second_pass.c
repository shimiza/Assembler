#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "second_pass.h"
#include "checks.h"
#include "utilities.h"
#include "encode.h"
#include "general.h"


#define ErrorJump \
error_flag = ON;\
continue;



int second_pass(FILE *fp, struct label_table *label_head, struct codeList *code_head, struct dataList *data_head, struct externList *extern_head)
{
	int index, label_end, res, cmd_num, instruction, IC = 100, line_num = 0;
	char *word = NULL, *line, *cmd_name = NULL;
	short error_flag = OFF;
	line = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
	if (!line)
	{
		printf("Error allocating memory, exiting program.\n");
    	exit(1);
    }
    
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
    	index = 0;
    	line_num++;
    	index = skip_white_chars(line, index);
    	if(line[index] == ';' || line[index] == '\n') /* ignore comment line or empty line */
    		continue;
    		
    	label_end = is_label_declaration(line, index);
    	if(label_end > 0)   /* if label - skip it */
    		index = label_end + 1;   /* set the index to point after label name */
    		
    	index = skip_white_chars(line, index);
    	if(line[index] == '.') /* start of instruction */	
    	{
    		char *inst_name;
    		index++;
    		inst_name = get_word(line,index);
    		if (!inst_name)
			{
				printf("Error allocating memory, exiting program.\n");
    			exit(1);
    		}
    		
    		instruction = is_instruction(inst_name);
    		if((instruction >= 1 && instruction <= 3) || instruction == 5) /* data/string/struct/extern instruction */ 
    		{
    			free(inst_name);
    			continue;
    		}	
    			
    		if(instruction == 4)   /* entry instruction */
    		{
    			index += strlen(inst_name);	/* set index to point after extern instruction */
				index = skip_white_chars(line, index);
					
				if(line[index] == '\n') /* no more text */
				{
					printf("Error:(line:%d) missing label name after entry instruction.\n",line_num);
					free(inst_name);
					ErrorJump
				}
				
				word = get_word(line,index); /* get the label name */ 
				if (!word)
				{
					printf("Error allocating memory, exiting program.\n");
    				exit(1);
    			}
    			
				if(strlen(word) > MAX_LABEL_LENGTH)
    			{
    				printf("Error:(line:%d) label name can't be longer than %d.\n",line_num,MAX_LABEL_LENGTH);
    				free(inst_name);
    				free(word);
					ErrorJump
    			}
    			
    			if(!is_valid_label_name(word))
				{
					printf("Error:(line:%d) invalid label name.\n",line_num);	
					free(inst_name);
					free(word);					
					ErrorJump
				}
				
				index += strlen(word); /* set index to point after label name */
				index = skip_white_chars(line, index);
				if(line[index] != '\n') /* there is additional text */
				{
					printf("Error:(line:%d) extraneous text after label name.\n",line_num);
					free(inst_name);
					free(word);
					ErrorJump
				}
				
				res = update_entry(label_head, word); /* set the label as entry */
				if(res == 1) /* label was extern */
				{
					printf("Error:(line:%d) label can't be both entry and extern.\n",line_num);
					free(inst_name);
					free(word);
					ErrorJump
				}
				
				if(res == 2) /* label not found */
				{
					printf("Error:(line:%d) label %s not found.\n",line_num, word);
					free(inst_name);
					free(word);
					ErrorJump
				}
			}
			if(word)
				free(word);
			free(inst_name);	
			continue;
		}	
			
			
		/* if not label nor instruction, than cmd name expected */
		cmd_name = get_word(line,index);
		if(!cmd_name)
		{
			printf("Error allocating memory, exiting program.\n");
    		exit(1);
    	}
    	
		cmd_num = is_cmd(cmd_name);
		if(cmd_num == -1  || !isspace(line[index+3]))  /* if cmd name is unknown or if there is no space right after it */
		{
			printf("Error:(line:%d) unknown command name.\n",line_num);
			free(cmd_name);
			ErrorJump
		}
		
		/* legal cmd name has been entered */
		res = convert_cmd_line_to_code(label_head, extern_head, code_head, cmd_num, line, index, &IC);   /* encoding cmd line */
			
		if(res == EXTRANEOUS_TEXT)
		{
			printf("Error:(line:%d) extraneous text.\n",line_num);
			free(cmd_name);
			ErrorJump
		}
						
		if(res == MISSING_OPERAND)
		{
			printf("Error:(line:%d) missing operand.\n",line_num);
			free(cmd_name);
			ErrorJump
		}
						
		if(res == ILLEGAL_NUM)
		{
			printf("Error:(line:%d) expecting a legal number.\n",line_num);
			free(cmd_name);
			ErrorJump
		}
						
		if(res == NUM_OUT_OF_RANGE)
		{
			printf("Error:(line:%d) number is out of range(-128-127).\n",line_num);
			ErrorJump
		}
						
		if(res == ILLEGAL_METHOD)
		{
			printf("Error:(line:%d) illegal addressing method for cmd name.\n",line_num);
			free(cmd_name);
			ErrorJump
		}
			
		if(res == LABEL_NOT_EXIST)
		{
			printf("Error:(line:%d) label does not exist.\n",line_num);
			free(cmd_name);
			ErrorJump
		}
			
		if(res == WRONG_LABEL_TYPE)
		{
			printf("Error:(line:%d) expected label of type struct .\n",line_num);
			free(cmd_name);
			ErrorJump
		}
			
		if(res == ILLEGAL_FIELD_NUM)
		{
			printf("Error:(line:%d) illegal field number of struct.\n",line_num);
			free(cmd_name);
			ErrorJump
		}
			
		if(res == MISSING_COMMA)
		{
			printf("Error:(line:%d) missing comma between operands.\n",line_num);
			free(cmd_name);
			ErrorJump
		}
		free(cmd_name);
	}
	
	data_to_code(IC, code_head, data_head);    /* encoding data list */
	free(line);
	if(error_flag == ON)
		return 1;
	else
		return 0;	
}	
						
