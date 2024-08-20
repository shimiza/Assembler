#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "first_pass.h"
#include "checks.h"
#include "utilities.h"
#include "cmd_analysis.h"
#include "general.h"

#define MAX_LENGTH 10000

#define ErrorJump \
error_flag = ON;\
continue;


int first_pass(FILE *fp, struct label_table *label_head, struct dataList *data_head)
{
	int DC = 0, IC = 100, index, line_num = 0, L = 0, instruction, res, res2;
	short error_flag = OFF, label_flag = OFF;
	char *line, *labelName = NULL, *cmd_name = NULL;
	line = (char*)malloc((MAX_LENGTH) * sizeof(char));
	if(!line)
	{
		printf("Error allocating memory, exiting program.\n");
    	exit(1);
    }

    while(fgets(line, MAX_LENGTH, fp) != NULL)
    {
    	index = 0;
    	L = 0;
    	line_num++;
    	if(strlen(line) > MAX_LINE_LENGTH)
    	{
    		printf("Error:(line:%d) line can't be longer than %d characters.\n",line_num,MAX_LINE_LENGTH-1);
			ErrorJump
    	}
    	
    	else
    	{
			index = skip_white_chars(line, index);
			
			if(line[index] == ';' || line[index] == '\n') /* ignore comment line or empty line */
				continue;

			if(is_label_declaration(line, index)) /* label has been declared */
			{
				labelName = get_word(line,index);
				if(!labelName)
				{
					printf("Error allocating memory, exiting program.\n");
    				exit(1);
    			}
    			
				if(strlen(labelName) > MAX_LABEL_LENGTH) /* label name is longer than max */
				{
					printf("Error:(line:%d) label name can't be longer than %d characters.\n",line_num,MAX_LABEL_LENGTH);
					free(labelName);
					ErrorJump
				}
				
				if(!is_valid_label_name(labelName))
				{
					printf("Error:(line:%d) invalid label name.\n",line_num);
					free(labelName);						
					ErrorJump
				}

				if(is_label_exists(label_head, labelName))
				{
					printf("Error:(line:%d) label has already been declared.\n",line_num);
					free(labelName);						
					ErrorJump
				}
			
				label_flag = ON;
				index = index + strlen(labelName) + 1; /* set the index to point after label name */
				index = skip_white_chars(line, index);
				
				if(line[index] == '\n') /* no more text */
				{
					printf("Error:(line:%d) expected a method name or instruction after label.\n",line_num);
					free(labelName);
					ErrorJump
				}
			}
		
			if(line[index] == '.') /* start of instruction name */
			{
				char *inst_name;
				index++;
				inst_name = get_word(line,index);
				if(!inst_name)
				{
					printf("Error allocating memory, exiting program.\n");
    				exit(1);
    			}
    			
				instruction = is_instruction(inst_name);
			
				if(instruction == 0) /* not an instruction name */
				{
					printf("Error:(line:%d) invalid instruction name.\n",line_num);
					free(inst_name);
					ErrorJump
				}
			
				if(instruction <= 3) /* storage instruction */
				{
					if(label_flag)
					{
						if(labelName)
						{
							label_flag = OFF;
							if(instruction == 1)
								add_label_to_table(&label_head, labelName, DC, DATA);
							else if(instruction == 2)
								add_label_to_table(&label_head, labelName, DC, STRUCT);
							else if(instruction == 3)
								add_label_to_table(&label_head, labelName, DC, STRING);
						}		
					}
					switch(instruction)
					{
						case 1: /* data */
					
							index += strlen(inst_name); /* set index to point after data instruction */
							res = add_data(line, index, data_head, &DC);
						
							if(res == MISSING_COMMA)
							{
								printf("Error:(line:%d) expecting a comma after number.\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							if(res == ILLEGAL_NUM)
							{
								printf("Error:(line:%d) expecting a legal number.\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							if(res == NUM_OUT_OF_RANGE)
							{
								printf("Error:(line:%d) number is out of range (-512-511).\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							if(res == MISSING_NUM)
							{
								printf("Error:(line:%d) expected at least one number.\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							break;
						
						case 2: /* struct */
					
							index += strlen(inst_name);	/* set index to point after struct instruction */
							res = add_struct(line, index, data_head, &DC);
						
							if(res == ILLEGAL_STRING)
							{
								printf("Error:(line:%d) illegal string, missing inverted commas at start or end of string.\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							if(res == EXTRANEOUS_TEXT)
							{
								printf("Error:(line:%d) extraneous text after end of string.\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							if(res == ILLEGAL_NUM)
							{
								printf("Error:(line:%d) expecting a legal number.\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							if(res == NUM_OUT_OF_RANGE)
							{
								printf("Error:(line:%d) number is out of range(-512-511).\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							if(res == MISSING_COMMA)
							{
								printf("Error:(line:%d) missing comma after number.\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							break;	
						
						case 3: /* string */
					
							index += strlen(inst_name);	/* set index to point after string instruction */
							res = add_string(line, index, data_head, &DC);
						
							if(res == ILLEGAL_STRING)
							{
								printf("Error:(line:%d) illegal string, missing inverted commas at start or end of string.\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							if(res == EXTRANEOUS_TEXT)
							{
								printf("Error:(line:%d) extraneous text after end of string.\n",line_num);
								free(inst_name);
								ErrorJump
							}
						
							break;
						
					}
				
					if(labelName)
						free(labelName);
					if(inst_name)	
						free(inst_name);
					continue;
				}
			
				else /* entry or extern instruction */
				{
					if(label_flag)
					{
						label_flag = OFF;
						printf("Warning:(line:%d) meaningless label defined before entry/extern instruction.\n",line_num);	
					}
				
					if(instruction == 5) /* extern instruction */
					{
						char *lab_name;
						index += strlen(inst_name);	/* set index to point after extern instruction */
						index = skip_white_chars(line, index);
					
						if(line[index] == '\n') /* no more text */
						{
							printf("Error:(line:%d) missing label name after extern instruction.\n",line_num);
							ErrorJump
						}
					
						lab_name = get_word(line,index);
						if(!lab_name)
						{
							printf("Error allocating memory, exiting program.\n");
    						exit(1);
    					}	
    					
						if(strlen(lab_name) > MAX_LABEL_LENGTH) /* check if label too long */
						{
							printf("Error:(line:%d) label name can't be longer than %d characters.\n",line_num,MAX_LABEL_LENGTH);
							free(lab_name);
							ErrorJump
						}
						
						if(!is_valid_label_name(lab_name)) /* check if label is valid */
						{
							printf("Error:(line:%d) invalid label name.\n",line_num);
							free(lab_name);						
							ErrorJump
						}
					
						if(is_label_exists(label_head, lab_name))
						{
							printf("Error:(line:%d) label has already been declared.\n",line_num);
							free(lab_name);						
							ErrorJump
						}
					
						index = index + strlen(lab_name); /* set the index to point after label name */
						index = skip_white_chars(line, index);
					
						if(line[index] != '\n') /* check if there is additional text */
						{
							printf("Error:(line:%d) extraneous text after end of label.\n",line_num);
							free(lab_name);
							ErrorJump
						}
					
						add_label_to_table(&label_head, lab_name, 0, EXTERN);
						free(lab_name);
					}
				}
				if(inst_name)
					free(inst_name);
				continue;
			}
			
		
			if(label_flag)
			{
				if(labelName)
				{
					label_flag = OFF;
			
					if(is_label_exists(label_head, labelName))
					{
						printf("Error:(line:%d) label has already been declared.\n",line_num);
						free(labelName);				
						ErrorJump
					}
			
					add_label_to_table(&label_head, labelName, IC, CODE);
					free(labelName);
				}	
			}
		
			/* get the next word and check if it is a cmd name */
			index = skip_white_chars(line, index);
			cmd_name = get_word(line,index);
			if(!cmd_name)
			{
				printf("Error allocating memory, exiting program.\n");
    			exit(1);
    		}
    		
			res = is_cmd(cmd_name);
			if(res == -1)
			{
				printf("Error:(line:%d) unknown command name.\n",line_num);
				free(cmd_name);
				ErrorJump
			}
		
			/* legal cmd name has been entered */
			index += strlen(cmd_name); /* set index to point after cmd name */

			/* calculate number of words and update IC */
			res2 = calculate_words(res, &L, line, index); 
			if (res2 == EXTRANEOUS_TEXT)
			{
				printf("Error:(line:%d) extraneous text at end of cmd line.\n",line_num);
				free(cmd_name);
				ErrorJump
			}
		
			if (res2 == MISSING_OPERAND)
			{
				printf("Error:(line:%d) missing operand name.\n",line_num);
				free(cmd_name);
				ErrorJump
			}
		
			if (res2 == ILLEGAL_METHOD)
			{
				printf("Error:(line:%d) illegal addressing method for command name.\n",line_num);
				free(cmd_name);
				ErrorJump
			}
		
			if (res2 == MISSING_COMMA)
			{
				printf("Error:(line:%d) missing comma between operands.\n",line_num);
				free(cmd_name);
				ErrorJump
			}

			IC += L;
			free(cmd_name);
		}	
	}
	
	free(line);
	update_data_labels(label_head, IC);
	if(error_flag == ON)
		return 1;
	else
		return 0;	
}	
    		
    		
