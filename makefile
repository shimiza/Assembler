assembler : assembler.o macro.o base_conversion.o checks.o label_table.o data.o code.o first_pass.o second_pass.o cmd_analysis.o encode.o utilities.o
	gcc -g -ansi -Wall -pedantic assembler.o macro.o base_conversion.o checks.o label_table.o data.o code.o first_pass.o second_pass.o cmd_analysis.o encode.o utilities.o -o assembler
assembler.o : assembler.c macro.h first_pass.h second_pass.h utilities.h general.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o
macro.o : macro.c macro.h general.h
	gcc -c -ansi -Wall -pedantic macro.c -o macro.o
base_conversion.o : base_conversion.c base_conversion.h general.h
	gcc -c -ansi -Wall -pedantic base_conversion.c -o base_conversion.o
checks.o : checks.c checks.h general.h
	gcc -c -ansi -Wall -pedantic checks.c -o checks.o
label_table.o : label_table.c label_table.h general.h
	gcc -c -ansi -Wall -pedantic label_table.c -o label_table.o
data.o : data.c data.h utilities.h general.h
	gcc -c -ansi -Wall -pedantic data.c -o data.o
code.o : code.c code.h general.h base_conversion.h
	gcc -c -ansi -Wall -pedantic code.c -o code.o
first_pass.o : first_pass.c first_pass.h label_table.h data.h checks.h utilities.h cmd_analysis.h general.h
	gcc -c -ansi -Wall -pedantic first_pass.c -o first_pass.o
second_pass.o : second_pass.c second_pass.h	label_table.h code.h data.h checks.h data.h encode.h general.h
	gcc -c -ansi -Wall -pedantic second_pass.c -o second_pass.o
cmd_analysis.o : cmd_analysis.c cmd_analysis.h utilities.h general.h
	gcc -c -ansi -Wall -pedantic cmd_analysis.c -o cmd_analysis.o
encode.o : encode.c encode.h label_table.h code.h data.h utilities.h base_conversion.h cmd_analysis.h general.h
	gcc -c -ansi -Wall -pedantic encode.c -o encode.o
utilities.o : utilities.c utilities.h label_table.h	code.h data.h base_conversion.h general.h
