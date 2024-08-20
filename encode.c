#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "encode.h"
#include "utilities.h"
#include "base_conversion.h"
#include "cmd_analysis.h"
#include "general.h"






int convert_cmd_line_to_code(struct label_table *label_head, struct externList *extern_head, struct codeList *code_head, int cmd_num, char *line, int index, int *IC) /* encode cmd line and add the encoding to code list, return 0 if ok, positive number for error */
{
	int num_of_words = 0, addressing1, i = 0, number, num_len = 0;
	char num[MAX_LINE_LENGTH] =	{'\0'}, *label_name = NULL;
	unsigned int next_word[5] = {0};/* max 5 words, according to addressing method */
	struct label_table *temp = NULL;
	
	/* first word */
	num_of_words++;
	next_word[0] |= (cmd_num<<6);
	index = index + 3; /* set index to point after cmd name */
	index = skip_white_chars(line, index);
	if(cmd_num > 13) /* no operands for this cmd */
	{
		if(line[index] != '\n') /* there is additional text */
			return EXTRANEOUS_TEXT; /* extraneous text after cmd name */
			
		else
		{
			add_code_to_DB(code_head, next_word,num_of_words,IC);
			return 0;
		}	
	}
	
	/* second word */
	if(line[index] == '\n') /* no more text */
		return MISSING_OPERAND;
	
	addressing1 = get_addressing_method(line, index);
	switch(addressing1)	
	{
		case 0:   /* immediate addressing */
		
			if(cmd_num < 4 || cmd_num == 12) /* legal addressing method */
			{
				index++;   /* skip '#' */
				if(line[index] == '-' || line[index] == '+')
    				num[i++] = line[index++];
    			
    			while(isdigit(line[index]))
    			{
    				num[i++] = line[index++];
        			num_len++;
    			}
    			
    			if((!isspace(line[index]) && line[index]!=',') || num_len == 0)/* length of number must be at least 1 and after the number must be space or comma*/
    				return ILLEGAL_NUM; /* illegal number, expected a number after '#' */
    		
    			num[i] = '\0';
    			number = atoi(num);
    			
    			if(number > 127 || number < -128)/* the machine have only 8 bits to represent the number */
    				return NUM_OUT_OF_RANGE;	
    				
    			next_word[num_of_words] |= ((number<<2)&(-4));   /* A,R,E = A */
    			num_of_words++;
    		}
    		
    		else
    			return ILLEGAL_METHOD;   /* illegal addressing method for this cmd */
    		
    		break;
    		
    	case 1:   /* direct addressing */
    		
    		label_name = get_word(line, index);
    		temp = search_label(label_head, label_name);
    		if(temp == NULL)
    			return LABEL_NOT_EXIST; /* label doesn't exist */		
    			
    		next_word[num_of_words] |= (temp->address)<<2;
    		if(temp->type == EXTERN) /* external label */ 
    		{	
    			next_word[num_of_words] |=	 1;   /* A,R,E = E */
    			add_to_externList(&extern_head, label_name, (*IC) + num_of_words); /* add the label to extern list */
    		}	
    			
    		else /* not external */
    			next_word[num_of_words] |=	 2;   /* A,R,E = R */
    		
    		if(cmd_num < 4 || cmd_num == 6)  /* source operand */
    			next_word[0] |= 1<<4;   /* addressing method for source operand */
    			
    		else   /* target operand */
    			next_word[0] |= 1<<2;   /* addressing method for target operand */
    			
    		index += strlen(label_name);
    		num_of_words++;
    		if(label_name)
    			free(label_name);
    		if(temp)
    			temp = NULL;
    		
    		break;
    		
    	case 2:	  /* struct access addressing */
    		
    		label_name = get_word(line, index);
    		temp = search_label(label_head, label_name);
    		if(temp == NULL)
    			return LABEL_NOT_EXIST; /* label doesn't exist */		
    			
    		if(temp->type != STRUCT)
    		{	
    			free(temp);
    			return WRONG_LABEL_TYPE;   /* expected label of type struct */
    		}	
    			
    		index += strlen(label_name) + 1;   /* set index to point after label name and '.' */
    		next_word[num_of_words] |= (temp->address)<<2;
    		num_of_words++;
    		
    		if(line[index] == '1' || line[index] == '2')
    			next_word[num_of_words] |= (line[index] - '0')<<2;
    			
    		else
    			return ILLEGAL_FIELD_NUM;  /* there are only two fields in struct: 1 or 2 */
    			
    		if(temp->type == EXTERN) /* external label */ 
    		{
    			next_word[num_of_words-1] |= 1;		/*  A,R,E = E */
    			next_word[num_of_words] |= 0;
    		}
    		
    		else   /* not external */
    		{
    			next_word[num_of_words-1] |= 2;		/*  A,R,E = R */
    			next_word[num_of_words] |= 0;
    		}	
    		
    		if(cmd_num < 4 || cmd_num == 6)  /* source operand */
    			next_word[0] |= 2<<4;   /* addressing method for source operand */
    			
    		else   /* target operand */
    			next_word[0] |= 2<<2;   /* addressing method for target operand */
    			
    		index++;   /* set index to point after field number */
    		num_of_words++;
    		if(label_name)
    			free(label_name);
    		if(temp)
    			temp = NULL;		
    		
    		break;
    		
    	case 3:	  /* direct register addressing */
    		
    		if(cmd_num == 6)
    			return ILLEGAL_METHOD;   /* illegal addressing method for this cmd for first operand */
    			
    		index++;   /* set index to point after 'r' */
    		if(cmd_num < 4)  /* source operand */
    		{
    			next_word[0] |= 3<<4;	/* addressing method for source operand */
    			next_word[num_of_words] |= (line[index] - '0')<<6;
    		}
    		
    		else   /* target operand */
    		{
    			next_word[0] |= 3<<2;   /* addressing method for target operand */
    			next_word[num_of_words] |= (line[index] - '0')<<6;
    		}
    		
    		index++; /* set index to point after register number */
    		num_of_words++;
    		
    		break;	
    }
    	
    /* third word */
    
    if(cmd_num < 4 || cmd_num == 6)  /* second operand */
    {
    	int addressing2;
    	index = skip_white_chars(line, index);
    	if(line[index] != ',')
    		return MISSING_COMMA;   /* missing comma between operands */
    		
    	index++;   /* set index to point after comma */	
    	addressing2 = get_addressing_method(line,index);
    	switch(addressing2)
    	{
    		case 0:   /* immediate addressing */
    			
    			if(cmd_num != 1)
    				return ILLEGAL_METHOD;   /* illegal addressing method for this cmd for second operand */
    				
    			index++;   /* set index to point after '#' */
    			num_len = 0;
    			i = 0;	
    			if((line[index] == '-' || line[index] == '+'))
    				num[i++] = line[index++];
    			
    			while(isdigit(line[index]))
    			{
    				num[i++] = line[index++];
        			num_len++;
    			}
    			
    			if(!isspace(line[index]) || num_len == 0) /* length of number must be at least 1 and after the number can't be text */
    				return ILLEGAL_NUM; /* illegal number, expected a number after '#' */
    		
    			num[i] = '\0';
    			number = atoi(num);
    			
    			if(number > 127 || number < -128)/* the machine have only 8 bits to represent the number */
    				return NUM_OUT_OF_RANGE;
    				
    			next_word[num_of_words] |= ((number<<2)&(-4));   /* A,R,E = A */
    			num_of_words++;
    			
    			break;
    			
    		case 1:   /* direct addressing */
    			
    			label_name = get_word(line, index);
    			temp = search_label(label_head, label_name);
    			if(temp == NULL)
    				return LABEL_NOT_EXIST; /* label doesn't exist */
    				
    			next_word[num_of_words] |= (temp->address)<<2;
    			if(temp->type == EXTERN) /* external label */ 	
    			{
    				next_word[num_of_words] |=	 1;   /* A,R,E = E */
    				add_to_externList(&extern_head, label_name, (*IC) + num_of_words); /* add the label to extern list */
    			}	
    			
    			else /* not external */
    				next_word[num_of_words] |=	 2;   /* A,R,E = R */
    				
    			next_word[0] |= 1<<2;   /* addressing method for target operand */	
    			index = skip_white_chars(line, index);
    			index += strlen(label_name);
    			num_of_words++;
    			if(label_name)
    				free(label_name);
    			if(temp)
    				temp = NULL;	
    			
    			break;
    			
    		case 2:	  /* struct access addressing */
    			
    			label_name = get_word(line, index);
    			temp = search_label(label_head, label_name);
    			if(temp == NULL)
    				return LABEL_NOT_EXIST; /* label doesn't exist */		
    			
    			if(temp->type != STRUCT)
    			{
    				free(temp);
    				return WRONG_LABEL_TYPE;   /* expected label of type struct */
    			}
    				
    			index += strlen(label_name) + 1;   /* set index to point after label name and '.' */
    			next_word[num_of_words] |= (temp->address)<<2;
    			num_of_words++;
    			
    			if(line[index] == '1' || line[index] == '2')
    				next_word[num_of_words] |= (line[index] - '0')<<2;
    			
    			else
    			{
    				free(temp);
    				return ILLEGAL_FIELD_NUM;  /* there are only two fields in struct: 1 or 2 */
    			}	
    				
    			if(temp->type == EXTERN) /* external label */ 
    			{
    				next_word[num_of_words-1] |= 1;		/*  A,R,E = E */
    				next_word[num_of_words] |= 0;
    			}
    		
    			else   /* not external */
    			{
    				next_word[num_of_words-1] |= 2;		/*  A,R,E = R */
    				next_word[num_of_words] |= 0;
    			}
    			
    			next_word[0] |= 2<<2;   /* addressing method for target operand */
    			index++;   /* set index to point after field number */
    			num_of_words++;
    			if(label_name)
    				free(label_name);
    			if(temp)
    				temp = NULL;
    			
    			break;
    			
    		case 3:	  /* direct register addressing */
    		
    			index = skip_white_chars(line, index);
    			index++;   /* set index to point after 'r' */
    			next_word[0] |= 3<<2;   /* addressing method for target operand */
    			if(addressing1 == 3)   /* check if another word is required */
    				next_word[num_of_words-1] |= (line[index] - '0')<<2;   /* two registers will be in 1 word */
    				
    			else
    			{
    				next_word[num_of_words] |= (line[index] - '0')<<2;
    				num_of_words++;
    			}
    			
    			index++; /* set index to point after register number */
    			
    			break;
    	}
    	
    }
    
    else  /* only target operand */
    {
    	index = skip_white_chars(line, index);
    	if(line[index] != '\n')  /* there is additional text */
    		return EXTRANEOUS_TEXT; /* extraneous text after operand */			
    }
    
    index = skip_white_chars(line, index);
    if(line[index] != '\n')  /* there is additional text */
    	return EXTRANEOUS_TEXT;	

    add_code_to_DB(code_head, next_word,num_of_words,IC);
    return 0;
}    	
    				
    		
    				
    			
    		
void add_code_to_DB(struct codeList *code_head, unsigned int arr[],int size,int *IC) /* convert word codes from decimal to base32 and add them to code list */
{
	int i;
	char *unique = NULL;
	for(i=0; i<size; i++)
	{
		unique = decimal_to_unique_base(arr[i]);
		if(!unique)
		{
			printf("Error allocating memory, exiting program\n");
			exit(1);
		}
				
		add_to_code_list(&code_head,*IC,unique);
		(*IC)++;
		free(unique);
	}
	
	return;	
}   






void data_to_code(int IC, struct codeList *code_head, struct dataList *data_head)   /* add data list to code list */
{
	struct codeList *last = code_head, *tmp;
	struct dataList *cur = data_head->next;
	char *unique;
	
	if(last)   /* code list is not empty */
		while(last->next)   /* go through the code list and set last to be the last node of the list */
			last = last->next;
			
	else   /* code list is empty */
	{
		if(cur)   /* data list is not empty */
		{
			tmp = (struct codeList *)malloc(sizeof(struct codeList));
			if(tmp)
			{
				unique = decimal_to_unique_base(cur->data);
				if(!unique)
				{
					printf("Error allocating memory, exiting program\n");
					exit(1);
				}
				
				tmp->address = IC + cur->DC;
				strcpy(tmp->base32, unique);
				code_head = tmp;    /* set tmp as the head of code list */
				last = tmp;
				cur = cur->next;
				free(unique);
			}
			
			else
			{
				printf("Error allocating memory, exiting program\n");
    			exit(2);
			}
		}
	}
	
	while(cur)  /* go through the whole data list */
	{
		tmp = (struct codeList *)malloc(sizeof(struct codeList));
		if(tmp)
		{
			unique = decimal_to_unique_base(cur->data);
			if(!unique)
			{
				printf("Error allocating memory, exiting program\n");
				exit(1);
			}
			
			tmp->address = IC + cur->DC;
			strcpy(tmp->base32, unique);
			last->next = tmp;
			last = last->next;
			free(unique);
		}
		else
		{
			printf("Error allocating memory, exiting program\n");
    		exit(2);
		}
		cur = cur->next;
	}
	return;
}						
    				
