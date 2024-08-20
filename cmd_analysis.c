#include <stdio.h>
#include <ctype.h>
#include "cmd_analysis.h"
#include "utilities.h"
#include "general.h"

int get_addressing_method(char *line, int index) /* analyze text and get the method of addressing by number */
{
	int i = 0;
	index = skip_white_chars(line, index);
	if(line[index] == '#') /* immediate addressing */
		return 0;
		
	if(line[index] == 'r' && line[index+1] >= '0' && line[index+1] <= '7')	/* direct register addressing */
		return 3;
		
	while(!isspace(line[index+i]) && line[index+i] != '.' && line[index+i] != ',')
		i++;
		
	if(line[index+i] == '.')	/* struct access addressing */
		return 2;
	else   /* direct addressing */
		return 1;	
}


int calculate_words(int cmd_num, int *L, char *line, int index) /* calculate the required number of words for each cmd line and update variable L, return positive number for error or 0 if no error occured */
{
	int num_of_words = 1, addressing1;
	if(cmd_num > 13) /* no operands required */
	{
		index = skip_white_chars(line, index);
		if(line[index] != '\n') /* there is additional text after cmd name */
			return EXTRANEOUS_TEXT;
	}
	
	else /* one or more operands required */
	{
		index = skip_white_chars(line, index);
		if(line[index] == '\n') /* no more text */
			return MISSING_OPERAND;
			
		addressing1 = get_addressing_method(line,index);
		switch(addressing1)
		{
			case 0: /* immediate addressing */
				if(cmd_num < 4 || cmd_num == 12) /* legal addressing method */
					num_of_words++;
					
				else /* illegal addressing method */
					return ILLEGAL_METHOD;
				break;
				
			case 1:	 /* direct addressing */
				num_of_words++;	
				break;
			
			case 2: /* struct access addressing */
				num_of_words += 2;
				break;
			
			case 3: /* direct register addressing */
				if(cmd_num != 6)
					num_of_words++;
					
				else /* illegal addressing method for this cmd name, can't be source operand */
					return ILLEGAL_METHOD;
				break;
		}
		
		while(!isspace(line[index]) && line[index] != ',') /* set the index to point after first operand */
			index++;
		
		index = skip_white_chars(line, index);
		if(cmd_num < 4 || cmd_num == 6)	 /* two operands required */
		{
			int addressing2;
			if(line[index] == '\n') /* no more text */
				return MISSING_OPERAND;
			
			if(line[index] != ',') /* no comma */
				return MISSING_COMMA;
			
			else /* there is a comma */
			{
				index++;
				index = skip_white_chars(line, index);
			}
			
			addressing2 = get_addressing_method(line,index);
			switch(addressing2)
			{
				case 0:        		/* immediate addressing */
					if(cmd_num == 1)   /* legal cmd method */
						num_of_words++;
					
					else
						return ILLEGAL_METHOD; /* addressing method does not match command name */	
					break;
					
				case 1:        		/* direct addressing */
					num_of_words++;
					break;
				
				case 2:        		/* struct access addressing */
					num_of_words += 2;
					break;
					
				case 3:        		/* direct register addressing */
					if(addressing1 != 3) /* two registers share one word */
						num_of_words++;
					break;
			}
			
			while(!isspace(line[index])) /* set the index to point after second operand */
				index++;
				
			index = skip_white_chars(line, index);
			if(line[index] != '\n') /* there is additional text after second operand */
				return EXTRANEOUS_TEXT;
		}
		
		else 		/* one operand required */
		{
			if(line[index] != '\n') /* there is additional text after first operand */
				return EXTRANEOUS_TEXT;
		}
	}
	
	*L = num_of_words;
	return 0;
}	

