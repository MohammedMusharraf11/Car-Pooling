#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commuter.h"
#include <time.h>

void searchForRides() {
    char pickupLocation[50], dropLocation[50], dateAvailable[20];
    printf("\nEnter Pickup Location: ");
    scanf("%s", pickupLocation);
    printf("Enter Drop Location: ");
    scanf("%s", dropLocation);
    printf("Enter Date (YYYY-MM-DD): ");
    scanf("%s", dateAvailable);

    char buffer[256];
    FILE *file = fopen("vehicles.csv", "r");
    if (file == NULL) {
        printf("Could not open file to search for rides.\n");
        return;
    }

    printf("\nAvailable rides:\n");
    int found = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        char *ownerName = strtok(buffer, ",");
        char *date = strtok(NULL, ",");
        char *drop = strtok(NULL, ",");
        char *pickup = strtok(NULL, ",");
        char *pickup1 = strtok(NULL, ",");
        char *seats = strtok(NULL, ",");

        if (strcmp(pickup, pickupLocation) == 0 && strcmp(drop, dropLocation) == 0 && strcmp(date, dateAvailable) == 0) {
            printf("Owner: %s, Seats: %s\n", ownerName, seats);
            found = 1;
        }
    }

    if (!found) {
        printf("No rides found for the given criteria.\n");
    }

    fclose(file);

    // Option to book a ride
    if (found) {
        char response;
        printf("\nDo you want to book a ride? (y/n): ");
        scanf(" %c", &response);
        if (response == 'y' || response == 'Y') {
            bookRide(pickupLocation, dropLocation, dateAvailable);
        }
    }
}

void bookRide(char pickupLocation[], char dropLocation[], char dateAvailable[]) {
    char commuterName[50], ownerName[50];
    printf("\nEnter Your Name: ");
    scanf("%s", commuterName);

    FILE *file = fopen("vehicles.csv", "r");
    FILE *bookingsFile = fopen("bookings.csv", "a");
    if (file == NULL || bookingsFile == NULL) {
        printf("Could not open file to book ride.\n");
        return;
    }

    char buffer[256];
    int found = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        char *owner = strtok(buffer, ",");
        char *date = strtok(NULL, ",");
        char *drop = strtok(NULL, ",");
        char *pickup = strtok(NULL, ",");
        char *seats = strtok(NULL, ",");

        if (strcmp(pickup, pickupLocation) == 0 && strcmp(drop, dropLocation) == 0 && strcmp(date, dateAvailable) == 0) {
            fprintf(bookingsFile, "%s,%s,%s,%s,%s,%s\n", commuterName, owner, dateAvailable, dropLocation, pickupLocation, seats);
            strcpy(ownerName, owner);
            found = 1;
            printf("Ride booked successfully! Waiting for owner confirmation.\n");
            break;
        }
    }

    if (!found) {
        printf("No matching ride found to book.\n");
    }

    fclose(file);
    fclose(bookingsFile);

    if (found) {
        srand(time(NULL));
        float billAmount = (rand() % 1000) + 500;
        printf("Your bill amount: $%.2f\n", billAmount);

        char paymentConfirmation;
        printf("Confirm payment? (y/n): ");
        scanf(" %c", &paymentConfirmation);
        if (paymentConfirmation == 'y' || paymentConfirmation == 'Y') {
            printf("Payment successful!\n");
            printf("Detailed information about your upcoming ride:\n");
            printf("Owner: %s\nPickup Location: %s\nDrop Location: %s\nDate: %s\n", ownerName, pickupLocation, dropLocation, dateAvailable);

            FILE *paymentsFile = fopen("payments.csv", "a");
            if (paymentsFile == NULL) {
                printf("Could not open file to save payment details.\n");
                return;
            }
            fprintf(paymentsFile, "%s,%s,%s,%s,%.2f\n", commuterName, ownerName, pickupLocation, dropLocation, billAmount);
            fclose(paymentsFile);
        } else {
            printf("Payment not confirmed. Ride booking canceled.\n");
        }
    }
}



void viewRecentTransactions() {
    char commuterName[50];
    printf("\nEnter Your Name to view recent transactions: ");
    scanf("%s", commuterName);

    FILE *file = fopen("payments.csv", "r");
    if (file == NULL) {
        printf("Could not open file to view recent transactions.\n");
        return;
    }

    char buffer[256];
    int found = 0;
    printf("\nYour recent transactions:\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        char *name = strtok(buffer, ",");
        char *owner = strtok(NULL, ",");
        char *pickup = strtok(NULL, ",");
        char *drop = strtok(NULL, ",");
        char *amount = strtok(NULL, ",");

        if (strcmp(name, commuterName) == 0) {
            printf("Owner: %s, Pickup: %s, Drop: %s, Amount: $%s\n", owner, pickup, drop, amount);
            found = 1;
        }
    }

    if (!found) {
        printf("No recent transactions found.\n");
    }

    fclose(file);
}



void viewUpcomingRides() {
    char commuterName[50];
    printf("\nEnter Your Name to view upcoming rides: ");
    scanf("%s", commuterName);

    FILE *file = fopen("confirmed_bookings.csv", "r");
    if (file == NULL) {
        printf("Could not open file to view upcoming rides.\n");
        return;
    }

    char buffer[256];
    int found = 0;
    printf("\nYour upcoming rides:\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        char *commuter = strtok(buffer, ",");
        char *owner = strtok(NULL, ",");
        char *date = strtok(NULL, ",");
        char *drop = strtok(NULL, ",");
        char *pickup = strtok(NULL, ",");
        char *seats = strtok(NULL, ",");

        if (strcmp(commuter, commuterName) == 0) {
            printf("Owner: %s, Date: %s, Pickup: %s, Drop: %s, Seats: %s\n", owner, date, pickup, drop, seats);
            found = 1;
        }
    }

    if (!found) {
        printf("No upcoming rides found.\n");
    }

    fclose(file);
}


void commuterMenu() {
    int choice;
  
    do {
        printf("\nCommuter Menu:\n");
        printf("1. Search for rides and book\n");
        printf("2. View upcoming rides\n");
        printf("3. View recent transactions\n"); // New option
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                searchForRides();
                break;
            case 2:
                viewUpcomingRides();
                break;
            case 3:
                viewRecentTransactions(); // Call the new function
                break;
            case 4:
                printf("\nThank you for using Viper Car Travelers!\n");
                exit(0);
            default:
                printf("Invalid choice. Please select again.\n");
                break;
        }
    } while (choice != 4);
}
