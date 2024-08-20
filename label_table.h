#ifndef _LABEL_TABLE_H
#define _LABEL_TABLE_H

#define MAX_LABEL_LENGTH 30



struct label_table {
    char name[MAX_LABEL_LENGTH+1];
    int address;
    short type;
    short is_ent;
    struct label_table *next;
    };
    
    
struct externList {
    char name[MAX_LABEL_LENGTH+1];
    char address[3]; /* address is unique base */
    struct externList *next;
    };   
    
    


    
    
 /**
 * Adds a label to the table.
 * @param head A pointer to a pointer to the head of the table
 * @param name The name of the label to insert
 * @param address The address of the label to insert
 * @param type The type of the label to insert
 */
void add_label_to_table(struct label_table **head, char *name, int address, int type);
 
 
 
 /**
 * Checks if the given label exists in the table.
 * @param head A pointer to the head of the table
 * @param name The name of the label to check
 * @return 1 if exists, 0 if not
 */
int is_label_exists(struct label_table *head, char *label_name); /* check if the label already exists */
 
 
 
 /**
 * Update value of data labels by adding IC.
 * @param head A pointer to the head of the table
 * @param IC The instruction counter
 */ 
void update_data_labels(struct label_table *head, int IC); /* update value of data labels by adding IC */
 
 
 
 /**
 * Search a given entry label in the table and update it as entry.
 * @param head A pointer to the head of the table
 * @param name The name of the entry label
 * @return 0 for successful update, positive number for unsuccessful update
 */  
int update_entry(struct label_table *head, char *name);
 
 
 
 /**
 * Search a label in the table by a given name.
 * @param head A pointer to the head of the table
 * @param name The name of the label
 * @return the label if found, NULL if not found
 */  
struct label_table *search_label(struct label_table *head, char *name);
 
 
 
 /**
 * Checks if there is at least one extern label in the table.
 * @param head A pointer to the head of the table
 * @return 1-true, 0-false
 */   
int is_ext_exist(struct label_table *head);
 
 
 
 /**
 * Checks if there is at least one entry label in the table.
 * @param head A pointer to the head of the table
 * @return 1-true, 0-false
 */    
int is_ent_exist(struct label_table *head);
 
 
 
 /**
 * Free all nodes in label table.
 * @param head A pointer to the head of the table
 */     
void free_label_table(struct label_table *head);
 
 
 
 
 /**
 * Adds an extern label to the extern list.
 * @param head A pointer to a pointer to the head of the extern list
 * @param name The name of the label to insert
 * @param address The address of the label to convert to base32 and insert
 */
void add_to_externList(struct externList **head, char *name, int address);
 
 
 
 /**
 * Frees all nodes in the extern list.
 * @param head A pointer to the head of the extern list
 */     
void free_externList(struct externList *head);
 
 
 
 
#endif 
 
