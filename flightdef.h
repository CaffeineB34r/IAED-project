/*
 * File: flightdef.h
 * Author:  Tomas Lopes
 * Internal author ID: 103099
 * Description: Program that handles an airport database
 	and registers both airports and flight schedules.
*/

#ifndef FLIGHT_OPS
#define FLIGHT_OPS 

#include <stdio.h>
#include <string.h>


#define MAXFLIGHTS 30000
#define MAXAIRPORTS 40
#define MAXID 3
#define MAXCOUNTRY 30
#define MAXCITY 50


#define ARRIVAL 1
#define DEPARTURE -1
#define ALLFLIGHTS 0

typedef struct Airport Airport;
typedef struct Time Time;
typedef struct flight flight;


void add_airport(char *command);
void info_airports(char *command);

void add_flight(char *command);
void list_flights(char *command,int modifier);

void list_airports(void);

void set_present(char *command);


int get_airport_position(char *id);


/*structure of an Airport*/
struct Airport{
	char codigo[MAXID+1];/*stores the airport's ID*/ 
	char pais[MAXCOUNTRY+1]; /*stores the airport's country*/
	char cidade[MAXCITY+1]; /*stores the airport's city*/
	int num_voos; /*stores the numbere of flights in departing the airport*/
};

struct Time{
	int year;
	int month;
	int day;
	int hour;
	int minute;
};

struct flight{
	char codigo[7];/*stores the flight id*/
	char partida[MAXID+1];/*stores the flight departing airport*/
	char chegada[MAXID+1]; /*stores the flight arrival airport*/
	Time takeoff; /*stores the flight's takeoff time*/
	Time landing; /*stores the flight's landing time*/
	int capacity; /*stores the flight's capacity*/
};

extern Airport Airports[MAXAIRPORTS+1];/*stores an array with the regitred airports*/
extern int airportNumber;/*stores the number of defined airports (size of array)*/

extern flight Flights[MAXFLIGHTS+1];/*stores an array with the regitred flights*/
extern int flightNumber;/*stores the number of defined flights (size of Flights)*/

extern Time present; /*stores the current date and time*/


#endif