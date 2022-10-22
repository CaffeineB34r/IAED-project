/*
 * File:  main.c
 * Author:  Tomas Lopes
 * Internal author ID: 103099
 * Description: Program that handles an airport database
    and registers both airports and flight schedules.
*/
/*header file defining the functions used and including libraries and universal
 * variables*/
#include "flightdef.h"

#define MAXREAD 100 /*The ammount of characters a program can read in a line*/

/*Procedure to initialize universal variables defined in included library*/
void l(int read, char *command);
void v(int read, char *command);
Time present = {2022, 1, 1, 0, 0}; /*stores the current date and time*/

int main(void) {
	char option =
	    '0';  /*stores the option selected by the user in command line*/
	int read; /* stores if an input was read*/
	char command[MAXREAD]; /*stores the arguments given by user after the
	                          option*/

	while (option != 'q') {
		command[0] = '\0';
		option = '0';

		fgets(command, MAXREAD, stdin);

		read = sscanf(command, " %c %[^\n]", &option, command);

		switch (option) {
			case 'a':
				add_airport(command);
				break;
			case 'l':
				l(read, command);
				break;
			case 'v':
				v(read, command);
				break;
			case 'p':
				list_flights(command, DEPARTURE);
				break;
			case 'c':
				list_flights(command, ARRIVAL);
				break;
			case 't':
				set_present(command);
				break;
			case 'q':
				return 0;
			default:
				break;
		}
	}
	return 0;
}

/*Procedures to select what happens depending on if an input is read*/
void l(int read, char *command) {
	if (read > 1)
		info_airports(command);
	else
		list_airports();
}

/*Procedures to select what happens depending on if an input is read*/
void v(int read, char *command) {
	if (read > 1)
		add_flight(command);
	else
		list_flights(command, ALLFLIGHTS);
}