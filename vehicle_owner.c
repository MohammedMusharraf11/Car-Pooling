#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vehicle_owner.h"

void displayOwnerVehicles() {
    char ownerName[50];
    printf("Enter your name to view your vehicles: ");
    scanf("%s", ownerName);

    char buffer[256];
    FILE *file = fopen("vehicles.csv", "r");
    if (file == NULL) {
        printf("Could not open file to display vehicles.\n");
        return;
    }

    printf("\nYour vehicles:\n");
    int found = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        char *tokenOwner = strtok(buffer, ",");
        char *dateAvailable = strtok(NULL, ",");
        char *dropLocation = strtok(NULL, ",");
        char *pickupLocation = strtok(NULL, ",");
        char *carName = strtok(NULL, ",");
        char *seats = strtok(NULL, ",");

        if (strcmp(tokenOwner, ownerName) == 0) {
            printf("Owner: %s, Date: %s, Pickup: %s, Drop: %s, Car: %s, Seats: %s\n", tokenOwner, dateAvailable, pickupLocation, dropLocation, carName, seats);
            found = 1;
        }
    }

    if (!found) {
        printf("No vehicles found for the given owner.\n");
    }

    fclose(file);
}


void viewReceivedPayments() {
    char ownerName[50];
    printf("\nEnter Your Name to view received payments: ");
    scanf("%s", ownerName);

    FILE *file = fopen("payments.csv", "r");
    if (file == NULL) {
        printf("Could not open file to view received payments.\n");
        return;
    }

    char buffer[256];
    int found = 0;
    printf("\nReceived payments:\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        char *commuter = strtok(buffer, ",");
        char *owner = strtok(NULL, ",");
        char *pickup = strtok(NULL, ",");
        char *drop = strtok(NULL, ",");
        char *amount = strtok(NULL, ",");

        if (strcmp(owner, ownerName) == 0) {
            printf("Commuter: %s, Pickup: %s, Drop: %s, Amount: $%s\n", commuter, pickup, drop, amount);
            found = 1;
        }
    }

    if (!found) {
        printf("No payments received.\n");
    }

    fclose(file);
}




void addVehicle() {
    char ownerName[50], dateAvailable[20], dropLocation[50], pickupLocation[50], carName[50];
    int seats;

    printf("Enter Vehicle Owner Name: ");
    scanf("%s", ownerName);
    printf("Enter Date Available (YYYY-MM-DD): ");
    scanf("%s", dateAvailable);
    printf("Enter Drop Location: ");
    scanf("%s", dropLocation);
    printf("Enter Pickup Location: ");
    scanf("%s", pickupLocation);
    printf("Enter Car Name: ");
    scanf("%s", carName);
    printf("Enter Number of Seats: ");
    if (scanf("%d", &seats) != 1 || seats <= 0) {
        printf("Invalid number of seats. Please enter a positive integer.\n");
        return;
    }

    FILE *file = fopen("vehicles.csv", "a");
    if (file == NULL) {
        printf("Could not open file to save vehicle data.\n");
        return;
    }

    fprintf(file, "%s,%s,%s,%s,%s,%d\n", ownerName, dateAvailable, dropLocation, pickupLocation, carName, seats);
    fclose(file);

    printf("Vehicle added successfully!\n");
}

void displayVehicles() {
    char buffer[256];
    FILE *file = fopen("vehicles.csv", "r");
    if (file == NULL) {
        printf("Could not open file to display vehicles.\n");
        return;
    }

    printf("\nList of vehicles:\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        char *ownerName = strtok(buffer, ",");
        char *dateAvailable = strtok(NULL, ",");
        char *dropLocation = strtok(NULL, ",");
        char *pickupLocation = strtok(NULL, ",");
        char *carName = strtok(NULL, ",");
        char *seats = strtok(NULL, ",");

        printf("Owner: %s, Date: %s, Pickup: %s, Drop: %s, Car: %s, Seats: %s\n", ownerName, dateAvailable, pickupLocation, dropLocation, carName, seats);
    }

    fclose(file);
}

void deleteVehicle() {
    char ownerName[50], carName[50];
    printf("Enter your name: ");
    scanf("%s", ownerName);
    printf("Enter the car name to delete: ");
    scanf("%s", carName);

    FILE *file = fopen("vehicles.csv", "r");
    FILE *tempFile = fopen("temp_vehicles.csv", "w");
    if (file == NULL || tempFile == NULL) {
        printf("Could not open file to delete vehicle.\n");
        return;
    }

    char buffer[256];
    int found = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        char *tokenOwner = strtok(buffer, ",");
        char *tokenDate = strtok(NULL, ",");
        char *tokenDrop = strtok(NULL, ",");
        char *tokenPickup = strtok(NULL, ",");
        char *tokenCar = strtok(NULL, ",");
        char *tokenSeats = strtok(NULL, "\n");

        if (strcmp(tokenOwner, ownerName) == 0 && strcmp(tokenCar, carName) == 0) {
            found = 1;
            printf("Vehicle deleted successfully!\n");
        } else {
            fprintf(tempFile, "%s,%s,%s,%s,%s,%s\n", tokenOwner, tokenDate, tokenDrop, tokenPickup, tokenCar, tokenSeats);
        }
    }

    if (!found) {
        printf("No matching vehicle found to delete.\n");
    }

    fclose(file);
    fclose(tempFile);

    remove("vehicles.csv");
    rename("temp_vehicles.csv", "vehicles.csv");
}

#include <ctype.h> // Include ctype.h for isspace()

// Function to trim whitespace from a string
void trimWhitespace(char *str) {
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) {
        str[--len] = '\0';
    }
}

void acceptBooking() {
    char ownerName[50];
    printf("\nEnter Your Name (Vehicle Owner) to view and accept bookings: ");
    scanf("%s", ownerName);

    FILE *file = fopen("bookings.csv", "r");
    FILE *tempFile = fopen("temp.csv", "w");
    FILE *confirmedFile = fopen("confirmed_bookings.csv", "a");

    if (file == NULL || tempFile == NULL || confirmedFile == NULL) {
        printf("Could not open file to accept bookings.\n");
        return;
    }

    char buffer[256];
    int found = 0;
    printf("\nPending bookings:\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        char *commuter = strtok(buffer, ",");
        char *owner = strtok(NULL, ",");
        char *date = strtok(NULL, ",");
        char *drop = strtok(NULL, ",");
        char *pickup = strtok(NULL, ",");
        char *seats = strtok(NULL, ",");

        if (owner == NULL || commuter == NULL || date == NULL || drop == NULL || pickup == NULL || seats == NULL) {
            continue; // Skip lines with missing fields
        }

        trimWhitespace(owner); // Trim whitespace from owner name

        printf("DEBUG: Checking booking for owner: %s\n", owner);
        if (strcmp(owner, ownerName) == 0) {
            printf("Commuter: %s, Date: %s, Pickup: %s, Drop: %s, Seats: %s\n", commuter, date, pickup, drop, seats);
            found = 1;
        }
    }

    if (!found) {
        printf("No bookings found for the given owner.\n");
        fclose(file);
        fclose(tempFile);
        fclose(confirmedFile);
        remove("temp.csv");
        return;
    }

    printf("\nEnter the name of the commuter to accept the booking: ");
    char commuterName[50];
    scanf("%s", commuterName);

    rewind(file);
    found = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        char *commuter = strtok(buffer, ",");
        char *owner = strtok(NULL, ",");
        char *date = strtok(NULL, ",");
        char *drop = strtok(NULL, ",");
        char *pickup = strtok(NULL, ",");
        char *seats = strtok(NULL, ",");

        if (owner == NULL || commuter == NULL || date == NULL || drop == NULL || pickup == NULL || seats == NULL) {
            fprintf(tempFile, "%s\n", buffer); // Write back malformed or empty lines
            continue; // Skip lines with missing fields
        }

        trimWhitespace(owner); // Trim whitespace from owner name

        printf("DEBUG: Processing booking for commuter: %s, owner: %s\n", commuter, owner);
        if (strcmp(owner, ownerName) == 0 && strcmp(commuter, commuterName) == 0) {
            fprintf(confirmedFile, "%s,%s,%s,%s,%s,%s\n", commuter, owner, date, drop, pickup, seats);
            printf("Booking accepted for commuter: %s\n", commuterName);
            found = 1;
        } else {
            fprintf(tempFile, "%s,%s,%s,%s,%s,%s\n", commuter, owner, date, drop, pickup, seats);
        }
    }

    fclose(file);
    fclose(tempFile);
    fclose(confirmedFile);

    if (found) {
        remove("bookings.csv");
        rename("temp.csv", "bookings.csv");
    } else {
        printf("No booking found for the given commuter.\n");
        remove("temp.csv");
    }
}


void ownerMenu() {
    int choice;
  
    do {
        printf("\nOwner Menu:\n");
        printf("1. Add Vehicle\n");
        printf("2. Delete Vehicle\n");
        printf("3. Accept Booking\n");
        printf("4. View Received Payments\n");
        printf("5. Display My Vehicles\n"); // New option
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addVehicle();
                break;
            case 2:
                deleteVehicle();
                break;
            case 3:
                acceptBooking();
                break;
            case 4:
                viewReceivedPayments();
                break;
            case 5:
                displayOwnerVehicles(); // Call the new function
                break;
            case 6:
                printf("\nThank you for using Viper Car Travelers!\n");
                exit(0);
            default:
                printf("Invalid choice. Please select again.\n");
                break;
        }
    } while (choice != 6);
}
