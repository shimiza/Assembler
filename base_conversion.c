#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base_conversion.h"
#include "general.h"


char digits[32]={'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};


char *decimal_to_unique_base(int num) /* convert num from decimal to unique base32 */
{
	unsigned int abs = num;
	int rem;
	char *unique = malloc(3*sizeof(char));
	if (!unique)
	{
		printf("Error allocating memory\n");
    	exit(1);
    }
    rem = abs % 32;
    unique[1] = digits[rem];
    abs /= 32;
    rem = abs % 32;
    unique[0] = digits[rem];
    unique[2] = '\0';
    return unique;
} 


