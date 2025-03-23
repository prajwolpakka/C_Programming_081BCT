// -- importing all the required modules -- //
#include <stdio.h>                           // for standard I/O and perror -> Displays error messages related to system functions (used in conjunction with exit()).
#include <conio.h>                           // for getch()
#include <string.h>                          // for string operations
#include <math.h>                            // for absolute value  [ not used in code]
#include <windows.h>                         // for system("cls") and Sleep (Windows-specific)
#include <time.h>                            // for timestamps and delays
#include <stdlib.h>                          // for exit() and malloc() / free() -> Allocates and releases dynamic memory for transaction data.
#include "ascii_display.c"                   // assumed to contain UI display functions

// defining structures globally
struct userdata {
    char name[100];
    char password[100];
    char email[100];
    double phone;
    double balance;
};

struct transaction {
    char sender_name[100];
    char receiver_name[100];
    double amount;
    time_t timestamp;			// a time_t data type for storing timestamps.
    char transaction_type[50]; // e.g., "Send Money" or "School Fee"
};

// -- global declarations -- //
FILE *login_data_file, *user_data_file, *transaction_file;
struct userdata current_user, signup_verify;
#define INITIAL_BALANCE 8000
#define MAX_USERS 1000
#define MAX_TRANSACTIONS 10000  //not amount, its for transaction_history

int max_length = 8;
int min_length = 4;

// ---- function prototyping --- //
void login();
void signup();
void reset_password();
void menu_handling();
void show_details();
void exit_program();
void send_money();
void pay_school_fee();
void show_transaction_history();
void encrypt(char[]);
void decrypt(char[]);
void delay(double);
void password_taker(char[], int);
int validate_phone(double);
int validate_receiver(struct userdata receiver, struct userdata users[], int user_count);
int validate_email(const char* email);
void clear_input_buffer();                      
void save_user_data(struct userdata users[], int count);
void log_transaction(const char* sender, const char* receiver, double amount, const char* type);
void load_user_data(struct userdata users[], int* count);

// ---- starting of main function --- //
int main() {
    char initial_choice;


    // Display initial menu with error handling
    do {
        system("cls");
        cpay();
        printf("\t[1] Login\n\t[2] SignUp\n\t[3] Reset Password\n");
        printf("\tChoose an option [1-3]: ");
        initial_choice = getche();
        if (initial_choice != '1' && initial_choice != '2' && initial_choice != '3') {
            printf("\n\tInvalid choice! Please enter [1-3].\n");
            delay(1.5);
        }
    } while (initial_choice != '1' && initial_choice != '2'&& initial_choice != '3');

    switch (initial_choice) {
    case '1':
        {
            // check if this is very first time of running program
            login_data_file = fopen("logindata.dat", "rb");
            if(login_data_file == NULL){     // if this is first time and no file is opened, proceed to signup
                printf("\n\n\t***Seems like you are first user***\n\t\t***Signup first***");
                delay(1.5);

                // now head to case 2;
                goto signup_first;
                }

            login();
            break;
        }

        signup_first:
        case '2':             // if user chooses to signup
            {
                signup();
                break;
            }
        case '3':             // if user chooses to signup
            {
                reset_password();
                break;
            }
    }

    menu_handling();
    return 0;
}

// Function to clear input buffer ~ Continues clearing until encountering a newline (\n) or EOF (End of File).
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to take password with star masking
void password_taker(char password[], int max_length) {
    char ch;
    int charposition = 0;			//Tracks the position of characters in the buffer
    //int max_length = sizeof(password) - 1; // Leave room for null terminator

    while (1) {
        ch = _getch(); 				// Use _getch for Windows compatibility
        if (ch == 13) {				 // Enter key
            password[charposition] = '\0';  // Null-terminate the string
            printf("\n");					// Move to the next line after password entry
            break;
        }
        else if (ch == 8 && charposition > 0) { 		// Backspace key (ASCII 8)
            charposition--;								// Move cursor back in buffer
            password[charposition] = '\0';				// Remove character from string
            printf("\b \b");							// /b is for bacspace escape sequence, Visually erase the '*' on screen
        }
        else if (ch == 32 || ch == 9) { 				// Space or Tab
            continue;									// Ignore spaces and tabs
        }
        else if (charposition < max_length) {			// Check buffer limit,simply password length
            password[charposition] = ch;				// Add valid character to buffer
            charposition++;								// Move position forward
            printf("*");								 // Display '*' to mask the password
        }
        else {
            printf("\n\tPassword too long! Maximum %d characters allowed.\n", max_length);
            delay(1.5);									// Display error and pause
            break;
        }
    }
}

// Function to handle user login
void login() {
    struct userdata filedata[MAX_USERS];
    int is_login_successful = 0;

    system("cls");
    display_login();
    login_data_file = fopen("logindata.dat", "rb");

    int n = 0;
    while (fread(&filedata[n], sizeof(struct userdata), 1, login_data_file) == 1 && n < MAX_USERS) {
        decrypt(filedata[n].password);
        n++;
    }


    printf("\tEnter your login details:\n");
    printf("\tUsername: ");
    scanf("%99s", current_user.name);
    clear_input_buffer();                  //Prevents unintended errors in the next input prompt.
    printf("\tPassword: ");					//Any leftover characters in the buffer (like \n from previous inputs) are removed.
   // int max_length = 4;
    password_taker(current_user.password, max_length);

    for (int i = 0; i < n; i++) {
        if (strcmp(filedata[i].name, current_user.name) == 0 &&
            strcmp(filedata[i].password, current_user.password) == 0) {
            printf("\n\tLogin Successful!\n");
            current_user.balance = filedata[i].balance; // Fixed: Use filedata[i].balance
            current_user.phone = filedata[i].phone;
            strcpy(current_user.email, filedata[i].email);
            is_login_successful = 1;
            delay(1);
            break;
        }
    }
 fclose(login_data_file);
    if (!is_login_successful) {
        printf("\n\tIncorrect username or password!\n");
        delay(2);
        printf("\tPress 'x' to return to menu _ any key to re-enter ");
        if(getch()=='x') {
        main();
   		}
        login();
    }
}

void reset_password() {
    struct userdata users[MAX_USERS],resetdata;
    char confirm_password[100];
    int n = 0, user_found = 0;
    load_user_data(users, &n);

    reenter_reset_details:
    system("cls");
    reset_pass();
    printf("\tEnter the following details:\n");

    reenter_reset_username:
    printf("\tUsername: ");
    scanf("%99s", resetdata.name);
    if (resetdata.name[0] == '\0' || strchr(resetdata.name, ' ') != NULL) {
        printf("\tInvalid username! No spaces allowed .\n");
        clear_input_buffer();
        delay(1.5);
        goto reenter_reset_username;
    }

    reenter_reset_email:
    printf("\tEmail (e.g:user@domain.com): ");
    scanf("%99s", resetdata.email);
    if (!validate_email(resetdata.email)) {
        printf("\tInvalid email! Must be in format user@domain.com and no spaces.\n");
        clear_input_buffer();
        delay(1.5);
        goto reenter_reset_email;
    }

    reenter_reset_phone:
    printf("\tPhone number (98/97xxxxxxx): ");
    scanf("%lf", &resetdata.phone);
    if (!validate_phone(resetdata.phone)) {
        printf("\tInvalid phone number! Must be a valid Nepali number (e.g., 98/97xxxxxxx).\n");
        clear_input_buffer();
        delay(1.5);
        goto reenter_reset_phone;
    }

    for (int i = 0; i < n; i++) {
        if (strcmp(users[i].name, resetdata.name) == 0 && strcmp(users[i].email, resetdata.email) == 0 && (users[i].phone == resetdata.phone)) {
        	user_found++;
            reenter_reset_pass:
            passlength_reset_error:
            printf("\tNew Password (min 4 chars): ");
            //int max_length = 4;
            password_taker(resetdata.password, max_length);
            if (strlen(resetdata.password) < min_length) {
                printf("\tPassword must be at least %d characters long.\n",min_length);
                clear_input_buffer();
                delay(1.5);
                goto passlength_reset_error;
            }
            printf("\tConfirm New Password: ");
            password_taker(confirm_password, max_length);
            if (strcmp(resetdata.password, confirm_password) == 0) {
                encrypt(resetdata.password);
               /** if (fwrite(&resetdata, sizeof(struct userdata), 1, login_data_file) != 1) {
       				 perror("Error writing user data");
   				     exit(1);
  					}
 				fclose(reset_data_file); **/
 				strcpy(users[i].password, resetdata.password);
                save_user_data(users, n);
                printf("\tPassword reset successful!\n");
                delay(2);
                login();
            } else {
                printf("\tPasswords do not match!\n");
                delay(1.5);
                goto reenter_reset_pass;
            }
        }
    }
    if (user_found==0) {
        printf("\tNo user found with the given username and email.\n");
        delay(2);
        main();
    }
}


// Function to validate email format
int validate_email(const char* email) {
    if (email == NULL || email[0] == '\0' || email[0] == '@' || strchr(email, ' ') != NULL) {
        return 0;
    }
    // Basic email validation: must contain '@' and a domain (simplified)
    const char* at = strchr(email, '@');
    if (at == NULL || at == email || at[1] == '\0') {
        return 0;
    }
    const char* dot = strchr(at, '.');
    return dot != NULL && dot > at && dot[1] != '\0';
}

// Function to handle signup of user
void signup() {
    struct userdata signupdata,users[MAX_USERS];
    char verify_password[200];
    int signup_attempt = 0;
    int n = 0;

    load_user_data(users, &n); // Load user data into users array

    login_data_file = fopen("logindata.dat", "ab");
    if (login_data_file == NULL) {
        login_data_file = fopen("logindata.dat", "wb");
        if (login_data_file == NULL) {
            perror("Error creating logindata.dat");
            exit(1);
        }
    }

reenter_details:
    system("cls");
    display_signup();
    //rintf("\tTest Counter: %d\n",n+1);
    printf("\tEnter the following details:\n");

reenter_username:
    printf("\tUsername: ");
    if (signup_attempt != 1) scanf("%99s", signupdata.name);
    else printf("%s\n", signup_verify.name);
    for (int i = 0; i < n; i++) {
        if (strcmp(users[i].name, signupdata.name) == 0) {
            printf("\tThis username is already registered. Please use another username.\n\n");
            delay(1);
            goto reenter_username;
        }
    }
    if (signupdata.name[0] == '\0' || strchr(signupdata.name, ' ') != NULL) {
        printf("\tInvalid username! No spaces allowed .\n");
        clear_input_buffer();
        delay(1.5);
        goto reenter_username;
    }

reenter_email:
    printf("\tEmail (e.g:user@domain.com): ");
    if (signup_attempt != 1) scanf("%99s", signupdata.email);
    else printf("%s\n", signup_verify.email);
    for (int i = 0; i < n; i++) {
        if (strcmp(users[i].email, signupdata.email) == 0) {
            printf("\tThis email is already registered. Please use another email.\n\n");
            delay(1);
            goto reenter_email;
        }
    }
    if (!validate_email(signupdata.email)) {
        printf("\tInvalid email! Must be in format user@domain.com and no spaces.\n");
        clear_input_buffer();
        delay(1.5);
        goto reenter_email;
    }

reenter_phone:
    printf("\tPhone number (98/97xxxxxxx): ");
    if (signup_attempt != 1) scanf("%lf", &signupdata.phone);
    else printf("%.0lf\n", signup_verify.phone);
    for (int i = 0; i < n; i++) {
        if (users[i].phone == signupdata.phone) {
            printf("\tThis contact is already registered. Please use another contact.\n\n");
            delay(1);
            goto reenter_phone;
        }
    }
    if (!validate_phone(signupdata.phone)) {
        printf("\tInvalid phone number! Must be a valid Nepali number (e.g., 98/97xxxxxxx).\n");
        clear_input_buffer();
        delay(1.5);
        goto reenter_phone;
    }

passlength_error:
    printf("\tPassword (min 4 chars): ");

    password_taker(signupdata.password,max_length);
    if (strlen(signupdata.password) < min_length) {
        printf("\tPassword must be at least %d characters long.\n",min_length);
        clear_input_buffer();
        delay(1.5);
        goto passlength_error;
    }
    printf("\tRe-enter password: ");
    password_taker(verify_password,max_length);

    if (strcmp(verify_password, signupdata.password) != 0) {
        printf("\n\tPasswords do not match! Please re-enter password.\n");
        delay(2);
        strcpy(signup_verify.name, signupdata.name);
        strcpy(signup_verify.email, signupdata.email);
        signup_verify.phone = signupdata.phone;
        signup_attempt = 1;
        goto reenter_details;
    }

    signupdata.balance = INITIAL_BALANCE; //initial_balance
    encrypt(signupdata.password);
    if (fwrite(&signupdata, sizeof(struct userdata), 1, login_data_file) != 1) {
        perror("Error writing user data");
        exit(1);
    }
    fclose(login_data_file);
    printf("\n\t***Sign-Up Successful***\n\t***You can now login***\n");
    delay(1.5);
    login();
}

// Function to validate Nepali phone numbers
int validate_phone(double phone) {
    long long int phone_int = (long long int)phone;
    // Updated to include common Nepali prefixes (98, 97, 96, 98, 97)
    return (phone_int >= 9600000000LL && phone_int <= 9699999999LL) || // Numbers from 9600000000 to 9699999999
       (phone_int >= 9800000000LL && phone_int <= 9869999999LL) || // Numbers from 9800000000 to 9869999999
       (phone_int >= 9880000000LL && phone_int <= 9889999999LL) || // Numbers from 9880000000 to 9889999999
       (phone_int >= 9700000000LL && phone_int <= 9709999999LL) || // Numbers from 9700000000 to 9709999999
       (phone_int >= 9740000000LL && phone_int <= 9769999999LL);   // Numbers from 9740000000 to 9769999999
}

// Function to encrypt user's password (simple shift for demo)
void encrypt(char pass[]) {
    for (int i = 0; pass[i] != '\0'; i++) {
        pass[i] += 350; // Simple shift, not secure for real use
    }
}

// Function to decrypt encrypted password
void decrypt(char pass[]) {
    for (int i = 0; pass[i] != '\0'; i++) {
        pass[i] -= 350;
    }
}

// Function to create a delay in seconds
void delay(double seconds) {     
    Sleep((DWORD)(seconds * 1000)); // Windows-specific, DWORD is an unsigned 32-bit integer (commonly used in Windows API functions).
									//Since Sleep() expects a DWORD value, the calculated result (seconds * 1000) is typecasted to ensure compatibility.
}

// Function to handle the main menu
void menu_handling() {
    char menu_choice;
    do {
        system("cls");
        display_menu(current_user.name);
        //printf("\tPlease select an option [1-5]:\n");
        printf("\t1. Show Details\n\t2. Send Money\n\t3. Pay School Fee\n\t4. Show Transaction History\n\t5. Exit\n");
        printf("\t-----------------------------------------\n");
        printf("\tEnter your choice [1-5]: ");
        menu_choice = getche();
        if (menu_choice < '1' || menu_choice > '5') {
            printf("\n\tInvalid choice! Please enter 1-5.\n");
            delay(1.5);
        }
    } while (menu_choice < '1' || menu_choice > '5');

    switch (menu_choice) {
        case '1': show_details(); break;
        case '2': send_money(); break;
        case '3': pay_school_fee(); break;
        case '4': show_transaction_history(); break;
        case '5': exit_program(); break;
    }
}

// Function to show user details
void show_details() {
    struct userdata users[MAX_USERS];
    int n=0;
    load_user_data(users, &n); // Load user data into users array

    system("cls");
    printf("\n\tUSER DETAILS\n\t-------------------------------\n");
    printf("\tUsername: %s\n\tPhone: %.0f\n\tEmail: %s\n\tCurrent Balance: Rs %.2f\n\t-------------------------------\n",
           current_user.name, current_user.phone, current_user.email, current_user.balance);
    printf("\tPress any key to return to menu: ");
    getch();
    menu_handling();
}

// Function to send money to another user
void send_money() {
    struct userdata receiver, users[MAX_USERS];
    int n = 0, is_transaction_successful = 0;

    load_user_data(users, &n); // Load user data into users array

    system("cls");
    printf("\n\tFUND TRANSFER\n\t----------------------------------------\n");
    printf("\n\tAvailable Users (excluding you):\n");
    printf("\tUser\t\tPhone\n\t----------------------------------------\n");

    for (int i = 0; i < n; i++) {
        if (strcmp(users[i].name, current_user.name) != 0 || users[i].phone != current_user.phone) {
            printf("\t%-15s\t%.0f\n", users[i].name, users[i].phone);
        }
    }
    printf("\t----------------------------------------\n");

    printf("\n\tEnter receiver username: ");
    scanf("%99s", receiver.name);
    clear_input_buffer();
    printf("\tEnter receiver phone number: ");
    scanf("%lf", &receiver.phone);
    clear_input_buffer();
    for (int i = 0; i < n; i++){
    if ((!validate_receiver(receiver, users, n)) || strcmp(receiver.name, current_user.name) == 0 || (receiver.phone == current_user.phone)) {
        printf("\n\tInvalid receiver details! Transaction failed.\n");
        delay(2);
        printf("\tPress 'x' to return to menu _ any key to re-enter ");
        if(getch()=='x') {
        menu_handling();
   		}
        send_money();
        return;
    }
    }

    printf("\tEnter amount to send (Rs): ");
    double amount;
    scanf("%lf", &amount);
    clear_input_buffer();

    if (amount <= 0) {
        printf("\n\tInvalid amount! Amount must be positive.\n");
        delay(2);
        printf("\tPress 'x' to return to menu _ any key to re-enter ");
        if(getch()=='x') {
        menu_handling();
   		}
        send_money();
        return;
    }

    for (int i = 0; i < n; i++) {
        if (strcmp(users[i].name, current_user.name) == 0 && users[i].phone == current_user.phone) {
            if (users[i].balance < amount) {
                printf("\n\tInsufficient balance! Your current balance is Rs %.2f\n", users[i].balance);
                delay(2);
                printf("\tPress 'x' to return to menu _ any key to re-enter ");
       		 	if(getch()=='x') {
       	 		menu_handling();
   				}
                send_money();
                return;
            }
            users[i].balance -= amount;
            current_user.balance = users[i].balance;
        }
        if (strcmp(users[i].name, receiver.name) == 0 && users[i].phone == receiver.phone) {
            users[i].balance += amount;
            is_transaction_successful = 1;
        }
    }

    if (is_transaction_successful) {
        save_user_data(users, n);
        log_transaction(current_user.name, receiver.name, amount, "Send Money");
        printf("\n\tTransaction Successful!\n");
        printf("\tYour new balance is: Rs %.2f\n", current_user.balance);
    } else {
        printf("\n\tTransaction failed due to an error!\n");
    }

    printf("\t----------------------------------------\n");
    printf("\tPress any key to return to menu: ");
    getch();
    menu_handling();
}

// Function to validate receiver details
int validate_receiver(struct userdata receiver, struct userdata users[], int user_count) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, receiver.name) == 0 && users[i].phone == receiver.phone) {
            return 1;
        }
    }
    return 0;
}

// Function to pay school fee
void pay_school_fee() {
    struct userdata users[MAX_USERS];
    int n = 0, school_choice;

    struct school {
        char name[150];
        float fee;
    } schools[4] = {
        {"IOE Pulchowk", 1000.0},
        {"IOE Thapathali", 1500.0},
        {"IOE WRC", 2000.0},
   		{"IOE ERC", 2500.0}
    };

    load_user_data(users, &n); // Load user data into users array

    do {
        system("cls");
        printf("\n\tCOLLEGE FEE PAYMENT\n\t----------------------------------------\n");
        printf("\n\tAvailable Colleges:\n");
        for (int i = 0; i < 4; i++) {
            printf("\t%d. %-25s Rs %.2f\n", i + 1, schools[i].name, schools[i].fee);
        }
        printf("\t----------------------------------------\n");
        printf("\n\tEnter your choice [1-4]: ");
        scanf("%d", &school_choice);
        clear_input_buffer();
        if (school_choice < 1 || school_choice > 4) {
            printf("\n\tInvalid choice! Please enter 1-4.\n");
            delay(1.5);
            printf("\tPress 'x' to return to menu _ any key to re-enter ");
        	if(getch()=='x') {
        	menu_handling();
   			}
        }
    } while (school_choice < 1 || school_choice > 4);

    for (int i = 0; i < n; i++) {
        if (strcmp(users[i].name, current_user.name) == 0 && users[i].phone == current_user.phone) {
            if (users[i].balance < schools[school_choice - 1].fee) {
                printf("\n\tInsufficient balance! Your current balance is Rs %.2f\n", users[i].balance);
                delay(2);
                printf("\tPress 'x' to return to menu _ any key to re-enter ");
        		if(getch()=='x') {
        		menu_handling();
   				}
                pay_school_fee();
                return;
            }
            users[i].balance -= schools[school_choice - 1].fee;
            current_user.balance = users[i].balance;
        }
    }

    save_user_data(users, n);
    log_transaction(current_user.name, schools[school_choice - 1].name, schools[school_choice - 1].fee, "School Fee");
    printf("\n\tTransaction Successful!\n");
    printf("\tYour new balance is: Rs %.2f\n", current_user.balance);
    printf("\t----------------------------------------\n");
    printf("\tPress any key to return to menu: ");
    getch();
    menu_handling();
}

// Function to show transaction history
void show_transaction_history() {
    struct transaction* trans = malloc(MAX_TRANSACTIONS * sizeof(struct transaction));   //malloc ->  Allocates memory for transaction records dynamically. 
    if (trans == NULL) {								//Prevents memory overflow or segmentation faults by exiting if allocation fails.
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);						// Stops execution if memory fails to allocate
    }

    transaction_file = fopen("transactions.dat", "rb");
    if (transaction_file == NULL) {
        printf("\n\tNo transaction history available.\n");
        free(trans);								// opposite to malloc ,Ensures allocated memory is released after use to improve program efficiency.
        delay(2);
        menu_handling();
        return;
    }

    int n = 0;
    while (fread(&trans[n], sizeof(struct transaction), 1, transaction_file) == 1 && n < MAX_TRANSACTIONS - 1) {
        n++;
    }
    fclose(transaction_file);

    system("cls");
    printf("\n\tTRANSACTION HISTORY\n");
    printf("\t-----------------------------------------------------------------------------------------------\n");
    // Adjusted column widths for a balanced look
    printf("\t%-18s %-18s %-10s %-18s %20s\n", "Sender", "Receiver", "Amount", " Date/Time", "   Type");
    printf("\t-----------------------------------------------------------------------------------------------\n");

    int has_transactions = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(trans[i].sender_name, current_user.name) == 0 || strcmp(trans[i].receiver_name, current_user.name) == 0) {
            char time_str[26];										//ctime() – Time to String Conversion (Indirectly String Related)
            strncpy(time_str, ctime(&trans[i].timestamp), 24);       //Formats timestamp to a readable format eg: if we dont need to show 2025 , decrease '24' to '20'
            time_str[24] = '\0'; // Ensure null-termination
            char sign = (strcmp(trans[i].sender_name, current_user.name) == 0) ? '-' : '+';
            printf("\t%-18s %-18s %c%-10.2f %-18s %20s\n",
                   trans[i].sender_name, trans[i].receiver_name, sign, trans[i].amount, time_str, trans[i].transaction_type);
            has_transactions = 1;
        }
    }

    if (!has_transactions) {
        printf("\tNo transactions found for your account.\n");
    }

    printf("\t-----------------------------------------------------------------------------------------------\n");
    printf("\tPress any key to return to menu: ");
    getch();
    free(trans);							//Ensures allocated memory is released after use to improve program efficiency.
    menu_handling();
}

// Function to load user data from file
void load_user_data(struct userdata users[], int* count) {
    user_data_file = fopen("logindata.dat", "rb");
    if (user_data_file == NULL) {
        *count = 0;
        return;
    }

    *count = 0;
    while (fread(&users[*count], sizeof(struct userdata), 1, user_data_file) == 1 && *count < MAX_USERS) {
        (*count)++;
    }
    fclose(user_data_file);
}

// Function to save user data to file
void save_user_data(struct userdata users[], int count) {
    user_data_file = fopen("logindata.dat", "wb");
    if (user_data_file == NULL) {
        perror("Error saving user data");
        exit(1);
    }
    if (fwrite(users, sizeof(struct userdata), count, user_data_file) != count) {
        perror("Error writing user data");
        exit(1);
    }
    fclose(user_data_file);
}

// Function to log a transaction
void log_transaction(const char* sender, const char* receiver, double amount, const char* type) {
    struct transaction trans;
    strncpy(trans.sender_name, sender, sizeof(trans.sender_name) - 1);            //Limits the number of copied characters to prevent overflow.
    trans.sender_name[sizeof(trans.sender_name) - 1] = '\0';						//Ensures the string is null-terminated by adding '\0' at the end.
    strncpy(trans.receiver_name, receiver, sizeof(trans.receiver_name) - 1); 		//Prevention of buffer overflow by copying up to the buffer size minus one.
    trans.receiver_name[sizeof(trans.receiver_name) - 1] = '\0';
    trans.amount = amount;
    trans.timestamp = time(NULL);          										// Captures the current system time
    strncpy(trans.transaction_type, type, sizeof(trans.transaction_type) - 1);
    trans.transaction_type[sizeof(trans.transaction_type) - 1] = '\0';

    transaction_file = fopen("transactions.dat", "ab");
    if (transaction_file == NULL) {
        perror("Error opening transactions.dat");
        exit(1);
    }
    if (fwrite(&trans, sizeof(struct transaction), 1, transaction_file) != 1) {
        perror("Error logging transaction");
        exit(1);
    }
    fclose(transaction_file);
}

// Function to exit the program
void exit_program() {
    system("cls");
    printf("\n\tThank you for using C_PAY\n");
    delay(1);
    printf("\n\tDeveloped by:\n\n");
    delay(1);
    printf("\tAswin Kandel\t\t(081BCT004)\n");
    printf("\tDikesh Manandhar\t(081BCT008)\n");
    delay(1);
    printf("\tKishan Kumar Shah\t(081BCT014)\n");
    delay(1);
    printf("\tPujag Dallakoti\t\t(081BCT024)\n");
    delay(3);
    exit(0);
}
