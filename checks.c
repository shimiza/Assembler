#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "checks.h"
#include "utilities.h"
#include "general.h"

char *cmd[17] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt",NULL};


char *reg[9] = {"r0","r1","r2","r3","r4","r5","r6","r7",NULL};   


char *instructions[6] = {"data","struct","string","entry","extern",NULL};


int is_label_declaration(char *line ,int index)/* check whether a label has been declared, if true returns end index, if false returns 0 */
{
  int end_index;	
  end_index = skip_white_chars(line, index);
  if(line[end_index] == '\n') /* empty line */
     return 0;
     
  while(!isspace(line[end_index]) && line[end_index] != ':')
  	end_index++;
  
  if(line[end_index] == ':') /* label declaration must end with ':' */
   	return end_index;
   	
  return 0;
}





int is_register(char *name) /* check if a given word is a register name (1-true, 0-false) */
{
	int i = 0;
	while(reg[i])
	{
		if(!strcmp(reg[i], name))
			return 1;
		i++;
	}
	return 0;
}			





int is_instruction(char *name) /* check if a given word is an instruction name, if true return number of instruction, or 0 if false) */
{
	int i = 0;

	while(instructions[i])/* run through instructions names */
	{
		if(!strcmp(instructions[i], name))
			return i+1;
		i++;
	}
	return 0;
}			



int is_cmd(char *name) /* check if a given word is a command name, return number of command if true or -1 if false */
{
	int i;
	for(i=0; i<16; i++)
	{
		if(!strcmp(cmd[i], name))
			return i;
	}
	return -1;
}		







int is_reserved_word(char *name) /* check if a given word is a reserved word(register/command/instruction) returns 1 if true, 0 if false */
{
	if(is_register(name) || is_instruction(name) || (is_cmd(name) >= 0))
		return 1;
	else
		return 0;	
}




int is_alphanumeric_str(char *string) /* check if every char in a string is alphanumeric (1-true, 0-false) */
{
	int i;
	for (i=0; string[i]; i++)
	{
		if(!isalpha(string[i]) && !isdigit(string[i]))
			return 0;
	}
	return 1;
}




int is_valid_label_name(char *name) /* check if the string is a valid label name (1-true, 0-false) */
{
	if(isalpha(name[0]) && is_alphanumeric_str(name) &&
	       !is_reserved_word(name))
		return 1;
	else
		return 0;   
}

