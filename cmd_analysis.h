#ifndef _CMD_ANALYSIS_H
#define _CMD_ANALYSIS_H





/**
* Analyzes text and get the method of addressing by number.
* @param line The line of text
* @param index The current index
* @return the number of the addressing method
*/
int get_addressing_method(char *line, int index);




/**
* Calculates the required number of words for each cmd line and updates variable L.
* @param cmd_num The number of the command
* @param L a pointer to the number of words the machine code needs for a given command line
* @param line The line of text
* @param index The current index
* @return 0 for success, positive number(represented by error type) for failure
*/
int calculate_words(int cmd_num, int *L, char *line, int index);



#endif
