#ifndef _MACRO_H
#define _MACRO_H

#define MAX_LENGTH 10000 /* according to instructions, we assume program's max length */
#define MAX_LINE_LENGTH 81

struct Macro {
    char mcr_name[MAX_LINE_LENGTH];
    char mcr_content[MAX_LENGTH];
    struct Macro* next;
};



 /**
 * Checks if the line is the start or end of a macro definition.
 * @param line The line of text
 * @return 1 for start of macro, -1 for end of macro, 0 otherwise
 */
int is_macro_or_endmacro(char line[]);





 /**
 * Adds the macro name to the macro table.
 * @param node A pointer to a macro node
 * @param line The line of text
 * @return 1 for legal name and successfull addition, 0 for illegal macro name
 */
int add_name(struct Macro *node, char line[]);






 /**
 * Adds the macro content to the macro table.
 * @param node A pointer to a macro node
 * @param fp The file pointer to read the text from
 */
void add_content(struct Macro *node, FILE *fp);






 /**
 * Reads the file and replace macro with its content.
 * @param head A pointer to a pointer to the head of the macro table
 * @param argv The arguments array containing the names of the files
 * @param i The index of argv array
 */
void initial_file_read(struct Macro **head, char *argv[], int i);








 /**
 * Copies the content of the corresponding macro to a file when a macro is called.
 * @param line The line of text
 * @param fp The file pointer to print the text to
 * @param head A pointer to the head of the macro table
 */
int is_macro_name(char line[], FILE *fp, struct Macro *head);







 /**
 * Deploys the macros from the initial .as file to the .am file.
 * @param head A pointer to the head of the macro table
 * @param argv The arguments array containing the names of the files
 * @param i The index of argv array
 */
void deploy_macro(struct Macro *head, char *argv[], int i);



 /**
 * Frees all nodes in macro table.
 * @param head A pointer to the head of the macro table
 */  
void free_macro_list(struct Macro *head);


#endif
