#ifndef _CODE_H
#define _CODE_H


struct codeList {
	int address;   /* decimal address */
	char base32[3];     /* code in unique 32 base */
	struct codeList *next;
	};
	
	
	
	
	
	
 /**
 * Adds a code to the code list.
 * @param head A pointer to a pointer to the head of the list
 * @param address The address to insert
 * @param unique The base32 code to insert
 */	
void add_to_code_list(struct codeList **head, int address, char *unique);




 /**
 * Converts the first address from the list to base32, update the head to be the next node and return char array consisted of 2 chars of base32 address, 	   tab, and 2 chars of base32 code.
 * @param head A pointer to a pointer to the head of the list
 * @return array of 6 chars, the first two are the address in base32, the third is tab, the fourth and fifth are the code in base32 and the last is null
 */	
char *get_code(struct codeList **head);



 /**
 * Frees all nodes in code list.
 * @param head A pointer to the head of the list
 */  
void free_code_list(struct codeList *head);



#endif
