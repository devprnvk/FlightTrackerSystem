/**
*  Handles functions related to displaying information on the console.
*  This could include functions for printing tables and data information.
*  @file Display.c 
*  @author Pranav Kale pkale 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h> 
#include "display.h"
#include "flights.h"

// Check if compiled as C++
#ifdef __cplusplus
// Include C++ headers
#include <map>
#include <string>

// Define the AirportInfo struct
struct AirportInfo {
    char name[50];
    char city[50];
    char country[50];
};

// Define the AirportMap type
typedef std::map<std::string, struct AirportInfo> AirportMap;

// Declare the populateAirportMap function
extern "C" void populateAirportMap(FlightDatabase *fdatab, AirportMap &airportMap);

// Define the displayAllAirports function
extern "C" void displayAllAirports(FlightDatabase *fdatab)
{
    // Initialize the C++ map
    AirportMap airportMap;
    populateAirportMap(fdatab, airportMap);
    for (const auto &entry : airportMap) {
        printf("Airport: %s, City: %s, Country: %s\n",
               entry.first.c_str(),
               entry.second.city,
               entry.second.country);
    }
}
#endif

/**
* Display all the flights as a row in the table based on given 2d array
* 
*/
void displayFlightTableRow(FlightDatabase *fdatab)
{
  for (int i = 0; i < fdatab->count; i++) {
        Flight *flight = fdatab->flight[i];
        printf("| %-21s | %-5s | %-21s | %-10s | %-9s | %-20s |\n",
               flight->operatorName,
               flight->flightID,
               flight->departureAirport,
               flight->departTime,
               flight->arrivalTime,
               flight->arrivalAirport);
  }
}

/**
* Displays the Table Header for all flights
* Includes: Airline of flight, flight ID, Origin, Depart time, Arrival, Flight Total
*/
void displayFlightTableHeader() 
{
    printf("|        Airline        |   ID  |        Origin         |  Departed  |  Arrival  |      Destination     |\n");
    printf("|-----------------------+-------+-----------------------+------------+-----------+----------------------+\n");
}

/**
* Clears the console for the user 
* This is called every time user runs one of the show methdos 
* Called after show flight table option, show flight data option, show airports
*/
void clearConsole()
{
    #ifdef _WIN32
      system("cls");
    #else
      system("clear");
    #endif
}

/**
* Display's all the menu options to user
*/
void displayMenuOptions(const char options[][50]) 
{
    for (int i = 0; i < 6; i++) {
        printf("%s\n", options[i]);
    }
}

/**
*  Display specific information for One Flight (NOT A TABLE)
*  Includes ID, # of seats, engine type, Plane (Ex. Boeing 777), Pilot, 
*  Total miles, Total trips, and Operator (Ex.American Airlines)
*/
void displaySingleFlightData(FlightDatabase *fdatab, char const *str) {
    bool exists = false;
    for (int i = 0; i < fdatab->count; i++) {
        Flight *flight = fdatab->flight[i];
        if (strcmp(flight->flightID, str) == 0) {
            exists = true;
            printf("Flight found!\n\n");
            printf("[FLIGHT DATE]: %s\n", flight->date);
            printf("\n.------------------------------------------------------.\n");
            int val = 25 - strlen(flight->operatorName) - strlen(flight->flightID);
            printf("|%s %s", flight->operatorName, flight->flightID);
            for (int i = 0; i < val; i++) {
                putchar(' ');
            }
            printf("|                           |\n");
            printf("|                          |                           |\n");
            printf("|                        .-'-.                         |\n");
            printf("|                       ' === '                        |\n");
            printf("|                \\-----'  .-.  '-----/                 |\n");
            printf("|    ——————————————————'  '-'  '————————————————————   |\n");
            printf("|      '''''-|--/   \\==^'--T--'^==//   \\--|-'''''      |\n");
            printf("|               \\___/      O       \\___/               |\n");
            printf("|                                                      |\n");
            printf(".------------------------------------------------------.\n");
            printf("\nFlight Departing At: %s\n", flight->departTime);
            printf("Flight Arriving At: %s\n", flight->arrivalTime);
            printf("-------------------------\n");
            printf("Departure Airport: %s\n", flight->departureAirport);
            printf("Arrival Airport: %s\n", flight->arrivalAirport);
            printf("-------------------------\n");
            printf("Pilot Name: %s\n", flight->pilot);
            printf("Seats Available: %d\n", flight->seats);
            printf("Aircraft Type: %s\n", flight->aircraftType);
            printf("-------------------------\n");

            // Add other flight data here if needed
            break;
        }
    }
    if (!exists) {
        printf("Unknown Flight ID, please Re-Enter.\n\n");
    }
}