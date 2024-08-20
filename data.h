#ifndef _DATA_H
#define _DATA_H


#define MAX_LINE_LENGTH 81



struct dataList {
    int data;
    int DC;
    struct dataList *next;
    };





 /**
 * Adds data information to the data list and updates DC.
 * @param head A pointer to a pointer to the head of the list
 * @param number The number of the data to insert
 * @param DC a pointer to data counter to insert
 */
void add_to_data_list(struct dataList **head, int number, int *DC);
    









 /**
 * Checks for legal numbers and legal syntax after .data instruction and add them to the data list.
 * @param line The line of text
 * @param index The current index
 * @param head A pointer to the head of the list
 * @param DC a pointer to data counter to insert
 * @return 0 for success, positive number(represented by error type) for failure
 */
int add_data(char *line, int index, struct dataList *head, int *DC);




 /**
 * Checks for legal string after .string instruction and add it to the data list.
 * @param line The line of text
 * @param index The current index
 * @param head A pointer to the head of the list
 * @param DC a pointer to data counter to insert
 * @return 0 for success, positive number(represented by error type) for failure
 */
int add_string(char *line, int index, struct dataList *head, int *DC);




 /**
 * Checks for legal struct after .struct instruction and add it to the data list.
 * @param line The line of text
 * @param index The current index
 * @param head A pointer to the head of the list
 * @param DC a pointer to data counter to insert
 * @return 0 for success, positive number(represented by error type) for failure
 */
int add_struct(char *line, int index, struct dataList *head, int *DC);



 /**
 * Frees all nodes in data list.
 * @param head A pointer to the head of the list
 */
void free_data_list(struct dataList *head);
  
  
  
#endif  
