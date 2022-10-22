/*
 * File: flights.c
 * Author:  Tomas Lopes
 * Internal author ID: 103099
 * Description: Defines functions relating schedule and flights
  needed to manage the airports and flights database.
*/
#include "flightdef.h"
flight Flights[MAXFLIGHTS + 1]; /*stores an array with the regitred flights*/
int flightNumber = 0; /*stores the number of defined flights (size of Flights)*/

#define MINCAPACITY 10
#define MAXCAPACITY 100

int get_flight_position(flight check);
int timecmp(Time time1, Time time2);
void print_flight(flight out, int modifier);
void swap_flights(flight *flight1, flight *flight2);
int checkflightcode(char *code);
void iter_flights(flight *list, int size, int modifier);

/*Function that recieves two structures Time
    and returns the diference between the dates*/
int timecmp(Time time1, Time time2) {
	int a = time1.year << 9 | time1.month << 5 | time1.day;
	int b = time2.year << 9 | time2.month << 5 | time2.day;

	return a - b;
}

/*Function that recieves two structures Time
    and returns the diference between the hours in minutes*/
int hourcmp(Time time1, Time time2) {
	int a = time1.hour * 60 + time1.minute;
	int b = time2.hour * 60 + time2.minute;

	return a - b;
}

/*Procedure that reads a string and defines a global variable
 with the current time and date*/
void set_present(char *command) {
	Time tmp;
	int dif;

	sscanf(command, "%d-%d-%d", &tmp.day, &tmp.month, &tmp.year);

	dif = timecmp(tmp, present);
	if (dif < 0 || dif > 512)
		printf("invalid date\n");
	else {
		present = tmp;
		printf("%02d-%02d-%04d\n", present.day, present.month, present.year);
	}
}

/*Function that recieves two structures Time
    and returns another Time containing the sum of the arguments*/
Time timesum(Time time1, Time time2) {
	int daysinmonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	Time sum;

	sum.minute = time1.minute + time2.minute;

	sum.hour = (sum.minute / 60) + time1.hour + time2.hour;
	sum.minute %= 60;

	sum.day = (sum.hour / 24) + time1.day + time2.day;
	sum.hour %= 24;

	sum.month = time1.month + time2.month;

	sum.year = time1.year + time2.year;
	if (sum.month > 12) {
		sum.month -= 12;
		sum.year++;
	}

	while (sum.day > daysinmonth[sum.month - 1]) {
		sum.day -= daysinmonth[sum.month - 1];
		sum.month++;
		if (sum.month > 12) {
			sum.year += 1;
			sum.month -= 12;
		}
	}
	return sum;
}

/*Function that recieves a flight pointer and a string
    and initializes the values in the flight according to the string*/
int init_flight(flight *new, char *command) {
	Time tmp = {0, 0, 0, 0, 0};
	Time takeoff;

	sscanf(command, "%s %[^\n]", new->codigo, command);
	sscanf(command, "%s %s %[^\n]", new->partida, new->chegada, command);

	sscanf(command, "%d-%d-%d %[^\n]", &takeoff.day, &takeoff.month,
	       &takeoff.year, command);
	sscanf(command, "%d:%d %[^\n]", &takeoff.hour, &takeoff.minute, command);

	sscanf(command, "%d:%d %[^\n]", &tmp.hour, &tmp.minute, command);
	sscanf(command, "%d %[^\n]", &new->capacity, command);

	new->takeoff = takeoff;
	new->landing = timesum(takeoff, tmp);
	return (tmp.hour * 60 + tmp.minute);
}

/*Predicate function checks if the recieved flight and its respective duration
as args are a valid addition to database*/
int checkflight(flight check, int dur) {
	/*var dif stores the comparison between
	a flights schedule and current time*/
	int dif;

	dif = timecmp(check.takeoff, present);

	if (!checkflightcode(check.codigo))
		printf("invalid flight code\n");

	else if (get_flight_position(check))
		printf("flight already exists\n");

	else if (!get_airport_position(check.partida))
		printf("%s: no such airport ID\n", check.partida);

	else if (!get_airport_position(check.chegada))
		printf("%s: no such airport ID\n", check.chegada);

	else if (flightNumber >= MAXFLIGHTS)
		printf("too many flights\n");

	else if (dif < 0 || dif > 512)
		printf("invalid date\n");

	else if (dur > 720)
		printf("invalid duration\n");

	else if (check.capacity < MINCAPACITY || check.capacity > MAXCAPACITY)
		printf("invalid capacity\n");

	else
		return 1;

	return 0;
}

/*Predicate recieves a flight code and checks if it fits the format*/
int checkflightcode(char *code) {
	int i;
	for (i = 0; i < 2; i++)
		if (code[i] < 'A' || code[i] > 'Z') return 0;
	for (i = 2; code[i + 1] != '\0'; i++)
		if (code[i] < '0' || code[i] > '9') return 0;
	return 1;
}

/*Procedure that reads the command to create a new flight and add it if valid*/
void add_flight(char *command) {
	flight new;
	int dur;
	dur = init_flight(&new, command);
	if (checkflight(new, dur)) {
		Flights[flightNumber++] = new;
		Airports[get_airport_position(new.partida) - 1].num_voos++;
	}
}

/*Function that recieves a flight and
    returns its index+1 on universal array or 0 if the argument
    is nout found in array*/
int get_flight_position(flight check) {
	int i;
	for (i = 0; i < flightNumber; i++) {
		if (!strcmp(check.codigo, Flights[i].codigo))
			if (!timecmp(check.takeoff, Flights[i].takeoff)) return i + 1;
	}
	return 0;
}

/*Procedure that recieves a flight list and its size
    and a modifier ARRIVAL or DEPARTURE and orders it in ascending flight
   dates*/
void order_flights(flight *tmp, int size, int modifier) {
	int sorted, j;
	Time cmp1, cmp2;
	for (sorted = 0; sorted < size - 1; sorted++)
		for (j = 0; j < size - sorted - 1; j++) {
			cmp1 = (modifier == ARRIVAL) ? tmp[j].landing : tmp[j].takeoff;
			cmp2 =
			    (modifier == ARRIVAL) ? tmp[j + 1].landing : tmp[j + 1].takeoff;

			if (timecmp(cmp1, cmp2) > 0)
				swap_flights(&tmp[j + 1], &tmp[j]);
			else if (!timecmp(cmp1, cmp2) && hourcmp(cmp1, cmp2) > 0)
				swap_flights(&tmp[j + 1], &tmp[j]);
		}
}

/*Procedure that recieves two flight pointers and swaps their values*/
void swap_flights(flight *swap1, flight *swap2) {
	flight tmp = *swap1;
	*swap1 = *swap2;
	*swap2 = tmp;
}

/*NOTE:SIMILAR FUNCTIONS:COMPACT CODE*/

/*Function that reads a string with the airport IDs
and prints the flights landing in those airports ordered in ascending date*/
/*
void list_arrivals(char *command,int modifier){
    char id[MAXID+1];
    flight tmp[MAXFLIGHTS+1];
    int i,j;
    sscanf(command,"%s",id);
    if (!get_airport_position(id))
        printf("%s: no such airport ID\n",id);
    else if (modifier==ARRIVAL){
        for(i=0,j=0;i<flightNumber;i++)
            if (!strcmp(id,Flights[i].chegada))
                tmp[j++] = Flights[i];

        order_flights(tmp,j,ARRIVAL);
        list_flights(tmp,j,ARRIVAL);
    }
}
*/

/*Function that reads a string and a modifier with the airport IDs and prints
 the flights departing/landing (depending on arg modifier) in those airports
  ordered in ascending date*/
void list_modifier(char *command, int modifier) {
	char id[MAXID + 1];
	flight tmp[MAXFLIGHTS + 1];
	int i, j;
	sscanf(command, "%s", id);
	if (!get_airport_position(id))
		printf("%s: no such airport ID\n", id);

	else if (modifier == ARRIVAL) {
		for (i = 0, j = 0; i < flightNumber; i++)
			if (!strcmp(id, Flights[i].chegada)) tmp[j++] = Flights[i];
	} else if (modifier == DEPARTURE) {
		for (i = 0, j = 0; i < flightNumber; i++)
			if (!strcmp(id, Flights[i].partida)) tmp[j++] = Flights[i];
	}

	order_flights(tmp, j, modifier);
	iter_flights(tmp, j, modifier);
}

void list_flights(char *command, int modifier) {
	if (modifier == ALLFLIGHTS)
		iter_flights(Flights, flightNumber, ALLFLIGHTS);
	else if (modifier == DEPARTURE || modifier == ARRIVAL)
		list_modifier(command, modifier);
	else
		printf("ERROR:list_flights expects a defined argument\n");
}

/*Procedure that recieves a flight list and its size and
    iterates on list printing them according to the modifier
    ARRIVAL or DEPARTURE or 0 for standard print*/
void iter_flights(flight *list, int size, int modifier) {
	int i;
	for (i = 0; i < size; i++) print_flight(list[i], modifier);
}

/*Procedure that recieves a fligght and a modifier
    ARRIVAL or DEPARTURE or 0 and prints according to its format*/
void print_flight(flight out, int modifier) {
	Time output;
	output = (modifier == ARRIVAL) ? out.landing : out.takeoff;
	printf("%s ", out.codigo);
	if (modifier == ALLFLIGHTS || modifier == ARRIVAL)
		printf("%s ", out.partida);
	if (modifier == ALLFLIGHTS || modifier == DEPARTURE)
		printf("%s ", out.chegada);

	printf("%02d-%02d-%04d ", output.day, output.month, output.year);
	printf("%02d:%02d\n", output.hour, output.minute);
}
