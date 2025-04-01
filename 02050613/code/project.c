#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DAYS 7
#define INTEREST_RATE 0.1

// Structure to hold item information.
struct Item {
    char name[20];
    int stock;
    float rate;
};

// Global array of items (this can be extended as needed).
struct Item items[] = {
    {"Bike",    10, 20.0},
    {"Car",      5, 100.0},
    {"Scooter", 15, 15.0}
};
int numItems = sizeof(items)/sizeof(items[0]);

// Function to display current stock
void displayStock() {
    int i;
    printf("\n-------- Current Stock --------\n");
    for (i = 0; i < numItems; i++) {
        printf("%d. %s - Stock: %d, Rate per day: %.2f\n",
               i + 1, items[i].name, items[i].stock, items[i].rate);
    }
    printf("-------------------------------\n");
}

// Function to create a bill file and open it in Notepad.
void printBill(char *customerName, char *phone, char *transactionType,
               char *itemName, int quantity, int days, float baseCost,
               float interest, float totalCost)
{
    FILE *fp = fopen("bill.txt", "w");
    if (fp == NULL) {
        printf("Error creating bill file.\n");
        return;
    }
    fprintf(fp, "---------- Rental Service Bill ----------\n");
    fprintf(fp, "Customer Name: %s\n", customerName);
    fprintf(fp, "Phone Number : %s\n", phone);
    fprintf(fp, "Transaction  : %s\n", transactionType);
    fprintf(fp, "Item         : %s\n", itemName);
    fprintf(fp, "Quantity     : %d\n", quantity);
    fprintf(fp, "Days         : %d\n", days);
    fprintf(fp, "Base Cost    : %.2f\n", baseCost);
    if (interest > 0)
        fprintf(fp, "Interest Fee : %.2f\n", interest);
    fprintf(fp, "Total Cost   : %.2f\n", totalCost);
    fprintf(fp, "-----------------------------------------\n");
    fclose(fp);

    // Open the bill in Notepad (this works on Windows)
    system("notepad bill.txt");
}

// Function to save the customer's information in a file.
void saveRenterInfo(char *customerName, char *phone, char *transactionType,
                    char *itemName, int quantity, int days, float totalCost)
{
    FILE *fp = fopen("renters.txt", "a"); // append mode
    if (fp == NULL) {
        printf("Error opening renters file.\n");
        return;
    }
    fprintf(fp, "%s, %s, %s, %s, %d, %d, %.2f\n",
            customerName, phone, transactionType, itemName, quantity, days, totalCost);
    fclose(fp);
}

// Function to process a renting transaction.
void rentItem() {
    char customerName[50], phone[20];
    int choice, quantity, days;
    float baseCost, interest = 0, totalCost;

    // Display available items.
    displayStock();
    printf("Enter the number corresponding to the item you want to rent: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > numItems) {
        printf("Invalid item choice.\n");
        return;
    }
    int index = choice - 1;

    // Get customer details.
    printf("Enter your name: ");
    scanf(" %[^\n]s", customerName);
    printf("Enter your phone number: ");
    scanf(" %[^\n]s", phone);

    // Get rental details.
    printf("Enter quantity to rent: ");
    scanf("%d", &quantity);
    if (quantity > items[index].stock) {
        printf("Requested quantity not available. Available stock: %d\n", items[index].stock);
        return;
    }
    printf("Enter number of days for renting: ");
    scanf("%d", &days);

    // Deduct the quantity from stock.
    items[index].stock -= quantity;

    // Calculate base cost.
    baseCost = items[index].rate * quantity * days;

    // Calculate interest if days exceed the allowed limit.
    if (days > MAX_DAYS) {
        int extraDays = days - MAX_DAYS;
        interest = extraDays * (items[index].rate * quantity * INTEREST_RATE);
    }
    totalCost = baseCost + interest;

    printf("\nRental processed successfully.\n");
    printf("Total Cost: %.2f (Interest Fee: %.2f if applicable)\n", totalCost, interest);

    // Print the bill and save the customer’s record.
    printBill(customerName, phone, "Rent", items[index].name, quantity, days, baseCost, interest, totalCost);
    saveRenterInfo(customerName, phone, "Rent", items[index].name, quantity, days, totalCost);

    // Display updated stock.
    displayStock();
}

// Function to process a return transaction.
void returnItem() {
    char customerName[50], phone[20];
    int choice, quantity, days;
    float baseCost, interest = 0, totalCost;

    // Display available items.
    displayStock();
    printf("Enter the number corresponding to the item you are returning: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > numItems) {
        printf("Invalid item choice.\n");
        return;
    }
    int index = choice - 1;

    // Get customer details.
    printf("Enter your name: ");
    scanf(" %[^\n]s", customerName);
    printf("Enter your phone number: ");
    scanf(" %[^\n]s", phone);

    // Get return details.
    printf("Enter quantity to return: ");
    scanf("%d", &quantity);

    // Add the returned quantity back to stock.
    items[index].stock += quantity;

    printf("Enter number of days you kept the item: ");
    scanf("%d", &days);

    // Calculate base cost.
    baseCost = items[index].rate * quantity * days;

    // Calculate interest if the item was kept beyond the allowed days.
    if (days > MAX_DAYS) {
        int extraDays = days - MAX_DAYS;
        interest = extraDays * (items[index].rate * quantity * INTEREST_RATE);
    }
    totalCost = baseCost + interest;

    printf("\nReturn processed successfully.\n");
    printf("Total Cost: %.2f (Interest Fee: %.2f if applicable)\n", totalCost, interest);

    // Print the bill and save the customer’s record.
    printBill(customerName, phone, "Return", items[index].name, quantity, days, baseCost, interest, totalCost);
    saveRenterInfo(customerName, phone, "Return", items[index].name, quantity, days, totalCost);

    // Display updated stock.
    displayStock();
}

int main() {
    int choice;
    while (1) {
        printf("\n======== Rental Management System ========\n");
        printf("1. Rent an Item\n");
        printf("2. Return an Item\n");
        printf("3. Display Stock (Owner view)\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                rentItem();
                break;
            case 2:
                returnItem();
                break;
            case 3:
                displayStock();
                break;
            case 4:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
