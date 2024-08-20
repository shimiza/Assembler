#ifndef _CHECKS_H
#define _CHECKS_H


#define MAX_LINE_LENGTH 81







/**
* Checks if a label has been declared.
* @param line The current command line. 
* @param index The current line index.
* @return positive number(represents the index at the end of the label)-true, 0-false
*/
int is_label_declaration(char *line ,int index);




/**
* Checks if a given word is a register name.
* @param name The word. 
* @return 1-true, 0-false
*/
int is_register(char *name);




/**
* Checks if a given word is an instruction name.
* @param name The word to check. 
* @return instruction number(as given in the task) if true, 0-false
*/
int is_instruction(char *name);




/**
* Checks if a given word is a command name.
* @param name The word to check. 
* @return command number if true, -1 if false
*/
int is_cmd(char *name);




/**
* Checks if a given word is a reserved word.
* @param name The word to check. 
* @return 1-true, 0-false
*/
int is_reserved_word(char *name);




/**
* Checks if every char in a given string is alphanumeric.
* @param string The string. 
* @return 1-true, 0-false
*/
int is_alphanumeric_str(char *string);




/**
* Checks if a given string is a valid label name.
* @param name The string. 
* @return 1-true, 0-false
*/
int is_valid_label_name(char *name);


#endif
