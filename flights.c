/**
*  Contains functions for calculations and operations related to flights. 
*  This could involve calculating distances, updating flight information, etc.
*  @file Flight.c 
*  @author Pranav Kale pkale 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "flights.h"

/**
* Constructs an Inventory Structure 
* Dynamically Allocates Memory
* @return recordInventory the newly allocated inventory
*/
FlightDatabase *makeDatabase() 
{
    FlightDatabase *flightdb = (FlightDatabase *) malloc(sizeof(FlightDatabase));
    flightdb->flight = (Flight **) malloc(INITIALCAP * sizeof(Flight *));
    flightdb->count = 0;
    flightdb->capacity = INITIALCAP;
    return flightdb;
}

/**
 * Frees the memory allocated for the flights in the FlightDatabase.
 * @param flightdb the FlightDatabase to free.
 */
void freeFlightDatabase(FlightDatabase *flightdb) {
    if (flightdb == NULL) {
        return;
    } 
    for (int i = 0; i < flightdb->count; i++) {
        free(flightdb->flight[i]);
    }
    free(flightdb->flight);
    free(flightdb);
}

/**
 * Frees the memory allocated for the country nodes in the HashTable.
 * @param table the HashTable to free.
 */
void freeHashTable(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        CountryNode *countryNode = table->buckets[i];
        while (countryNode != NULL) {
            CountryNode *tempCountry = countryNode;
            AirportNode *airportNode = countryNode->airports;
            while (airportNode != NULL) {
                AirportNode *tempAirport = airportNode;
                airportNode = airportNode->next;
                free(tempAirport);
            }
            countryNode = countryNode->next;
            free(tempCountry);
        }
    }
    free(table->buckets);
    free(table);
}

/** 
* Get all the Data for a Flight Recording 
* @param fname is the file to open
* @param flight the flight struct to fill 
*/
bool getData(const char * fname, Flight *flight) {
    FILE *input = fopen(fname, "r");

    fscanf(input, "Current Date: %[^\n]\n", flight->date);
    fscanf(input, "Flight ID: %[^\n]\n", flight->flightID);
    fscanf(input, "Flight Departure Airport: %[^\n]\n", flight->departureAirport);
    fscanf(input, "Flight Departure Country: %[^\n]\n", flight->departureCountry);
    fscanf(input, "Flight Departure Time: %[^\n]\n", flight->departTime);
    fscanf(input, "Flight Arrival Time: %[^\n]\n", flight->arrivalTime);
    fscanf(input, "Flight Arrival Airport: %[^\n]\n", flight->arrivalAirport);
    fscanf(input, "Seats: %d\n", &flight->seats);
    fscanf(input, "Aircraft Type: %[^\n]\n", flight->aircraftType);
    fscanf(input, "Pilot: %[^\n]\n", flight->pilot);
    fscanf(input, "Total Miles on Plane: %lf\n", &flight->totalMiles);
    fscanf(input, "Total Trips by Plane: %d\n", &flight->totalTrips);
    fscanf(input, "Operator: %[^\n]\n", flight->operatorName);
    
    fclose(input);

    return true;
}

/**
* Hashes the string uniquely and returns value
* @param str the string to hash 
* @return hashed value 
*/
unsigned int hash(const char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

/**
* Creates a hash table of given size 
* Involves the use of buckets to store country nodes and airports
* @param size the size of the table
*/
HashTable *createTable(int size) {
    HashTable *table = malloc(sizeof(HashTable));
    table->size = size;
    table->buckets = calloc(size, sizeof(CountryNode *));
    return table;
}

/**
* Inserts an Airport to a country in the hash table 
* @param country the country to add an airport to
* @param airport the airport to check for 
*/
void insertAirport(HashTable *table, const char *country, const char *airport) {
    unsigned int index = hash(country);
    CountryNode *countryNode = table->buckets[index];

    while (countryNode != NULL && strcmp(countryNode->country, country) != 0) {
        countryNode = countryNode->next;
    }

    if (countryNode == NULL) {
        countryNode = malloc(sizeof(CountryNode));
        strcpy(countryNode->country, country);
        countryNode->airports = NULL;
        countryNode->next = table->buckets[index];
        table->buckets[index] = countryNode;
    }

    AirportNode *current = countryNode->airports;

    while (current != NULL) {
        if (strcmp(current->airport, airport) == 0) {
            return;
        }
        current = current->next;
    }
    
    AirportNode *airportNode = malloc(sizeof(AirportNode));
    strcpy(airportNode->airport, airport);
    airportNode->next = countryNode->airports;
    countryNode->airports = airportNode;
}

/**
    Calculate all the Average Miles per Flight 
    Allows customer to know usual flight miles on each travel
    @param miles the mileage of plane
    @param trips the # of trips the plane has chartered
    @return avgMiles the average miles per trip
*/
double calculateAverageMilesPerFlight(double miles, int trips) {
    double average = (double) miles / trips;
    return average;
}

/**
* Uses military time value to determine flight time total 
* Arrival minus the Departing Time and return a text about time of flight
* Example: the string for depart is "12:33 PM" the string for arrival is "03:19 PM"
* Take 03:19 PM and 12:33 PM
* Returns a time like this: "2 hours and 52 minutes.""
*/
char* calculateFlightTime(const char departTime[], const char arrivalTime[]) 
{
    int departHour, departMinutes, arrivalHour, arrivalMinutes;
    char departFormat[3], arrivalFormat[3];

    sscanf(departTime, "%d:%d %2s", &departHour, &departMinutes, departFormat);
    sscanf(arrivalTime, "%d:%d %2s", &arrivalHour, &arrivalMinutes, arrivalFormat);

    if (strcmp(departFormat, "PM") == 0 && departHour < 12) {
        departHour += 12;
    }

    if (strcmp(arrivalFormat, "PM") == 0 && arrivalHour < 12) {
        arrivalHour += 12;
    }

    int totalMinutes = (arrivalHour * 60 + arrivalMinutes) - (departHour * 60 + departMinutes);

    if (totalMinutes < 0) {
        totalMinutes += 24 * 60;
    }

    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;

    char* time = (char*)malloc(30);
  
    sprintf(time, "%d hours and %d minutes", hours, minutes);
    return time;
}

/**
* Calculate the total revenue of the flight
* Take the number of seats and multiply by fixed price
*/
double calculateTotalRevenue(int numSeats, double pricePerSeat)
{
    double price = numSeats * pricePerSeat;
    return price;
} 

/**
 * Calculate the number of remaining seats on a flight.
 * Takes the total number of seats and the number of seats booked.
 * Returns the remaining number of seats.
 */
int calculateRemainingSeats(int totalSeats, int seatsBooked) {
    int remainingSeats = totalSeats - seatsBooked;
    return (remainingSeats >= 0) ? remainingSeats : 0;
}