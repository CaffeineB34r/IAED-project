/*
 * File: airports.c
 * Author:  Tomas Lopes
 * Internal author ID: 103099
 * Description: Defines functions relating airports
  needed to manage the airports and flights database.
*/
#include "flightdef.h"

Airport Airports[MAXAIRPORTS + 1]; /*stores an array with the regitred airports*/
int airportNumber=0; /*stores the number of defined airports (size of array)*/

void init_airport(Airport *new, char *command);
int get_airport_position(char *code);
int checkairport(Airport new);
void info_airports(char *command);
void print_info(char *apt);
void print_airport(Airport out);
void order_airports(void);
void swap_airports(Airport *aeroporto1, Airport *aeroporto2);

/*Procedure that reads the command to create a new airpport and add it if
 * valid*/
void add_airport(char *command) {
	Airport new;

	init_airport(&new, command);

	if (checkairport(new)) {
		Airports[airportNumber++] = new;
		printf("airport %s\n", new.codigo);

		order_airports();
	}
}

/*Function that recieves a airport pointer and a string
    and initializes the values in the airport according to the string*/
void init_airport(Airport *new, char *command) {
	sscanf(command, "%s %[a-zA-Z] %[^\n]", new->codigo, new->pais, new->cidade);
	new->num_voos = 0;
}

/*Function that recieves an airport and
    returns its index+1 on universal array or 0 if the argument
    is nout found in array*/
int get_airport_position(char *id) {
	int i;

	for (i = 0; i < airportNumber; i++)
		if (!(strcmp(id, Airports[i].codigo))) return i + 1;

	return 0;
}

/*Predicate recieves an airport and checks if it can be added to the database */
int checkairport(Airport new) {
	int i;
	char c;

	for (i = 0; i < MAXID; i++) {
		c = new.codigo[i];

		if (!(c >= 'A' && c <= 'Z')) {
			printf("invalid airport ID\n");
			return 0;
		}
	}

	if (airportNumber >= MAXAIRPORTS) {
		printf("too many airports\n");
		return 0;
	}
	if (get_airport_position(new.codigo)) {
		printf("duplicate airport\n");
		return 0;
	}

	return 1;
}

/*Predicate to be called with one or more airport IDs on a string and
print information on each airport in t5he given order*/
void info_airports(char *command) {
	char c[MAXID + 1];

	while (sscanf(command, " %s %[^\n]", c, command) > 1) print_info(c);
	print_info(c);
}

/*Predicate to get each airport id and print its information
    or print an error if its not found*/
void print_info(char *apt) {
	int i;

	i = get_airport_position(apt);

	if (i)
		print_airport(Airports[i - 1]);

	else
		printf("%s: no such airport ID\n", apt);
}

/*Procedure that prints the defined Airports in alphabetical order*/
void list_airports(void) {
	int i;

	for (i = 0; i < airportNumber; i++) print_airport(Airports[i]);
}

/*Procedure that recieves an Airport and prints it according to the format*/
void print_airport(Airport out) {
	printf("%s %s %s %d\n", out.codigo, out.cidade, out.pais, out.num_voos);
}

/*Procedure that recieves two Airports poointers and swaps their values*/
void swap_airports(Airport *aeroporto1, Airport *aeroporto2) {
	Airport tmp = *aeroporto1;
	*aeroporto1 = *aeroporto2;
	*aeroporto2 = tmp;
}

/*Procedure that orders the airports in the global variable
in alphabetical order of their IDs*/
void order_airports(void) {
	int sorted, j;

	for (sorted = 0; sorted < airportNumber - 1; sorted++)
		for (j = 0; j < airportNumber - sorted - 1; j++)
			if (strcmp(Airports[j].codigo, Airports[j + 1].codigo) > 0)
				swap_airports(&Airports[j], &Airports[j + 1]);
}