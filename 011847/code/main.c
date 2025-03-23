#include <stdio.h> // Include standard input/output library
#include <stdlib.h> // Include standard library for random numbers
#include <string.h> // Include string library for string manipulation
#include <time.h> // Include time library for random seed generation

// Function prototypes
void kbcGame(); // Declare function for KBC game
void hangmanGame(); // Declare function for Hangman game
void numberGame(); // Declare function for Number Generator game
void clearScreen(); // Declare function for clearing the screen

// Main menu
int main() {
    int choice; // Declare variable to store user's choice
    srand(time(NULL)); // Seed random number generator with current time

    while (1) {
        clearScreen(); // Clear the screen
        printf("\n=== KHELA AND SIKA ===\n"); // Display main menu title
        printf("1. KBC (Kaun Banega Crorepati)\n"); // Option 1: KBC
        printf("2. Hangman\n"); // Option 2: Hangman
        printf("3. Number Generator\n"); // Option 3: Number Generator
        printf("4. Exit\n"); // Option 4: Exit
        printf("Enter your choice (1-4): "); // Prompt for user's choice
        scanf("%d", &choice); // Read user's choice

        // Switch based on user's choice
        switch (choice) {
            case 1: kbcGame(); break; // Call kbcGame() if choice is 1
            case 2: hangmanGame(); break; // Call hangmanGame() if choice is 2
            case 3: numberGame(); break; // Call numberGame() if choice is 3
            case 4:
                printf("Thanks for playing!\n"); // Display exit message
                return 0; // Exit the program
            default:
                printf("Invalid choice! Press enter to continue..."); // Invalid input
                getchar(); getchar(); // Wait for user to press enter
        }
    }
    return 0; // Exit main function
}

// Clear screen function
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // Clear screen for Windows
    #else
        system("clear"); // Clear screen for Linux/Unix
    #endif
}

// KBC Game
void kbcGame() {
    char *questions[14] = {
        "1N equals to how much dyne?",
        "Which of the following is not a vector?",
        "What is the SI unit of power?",
        "Who discovered gravity?",
        "What is the speed of light in vacuum?",
        "Which law states F = ma?",
        "What is the main source of energy for Earth?",
        "Which particle has a negative charge?",
        "What is the formula for kinetic energy?",
        "Which type of energy is stored in a spring?",
        "What is the unit of electrical resistance?",
        "Which color has the highest frequency in visible light?",
        "What is the acceleration due to gravity on Earth?",
        "Which gas makes up most of Earth's atmosphere?"
    };

    char *options[14][4] = {
        {"A. 10^5", "B. 10^7", "C. 10^9", "D. 10^2"},
        {"A. Force", "B. Velocity", "C. Temperature", "D. Acceleration"},
        {"A. Joule", "B. Watt", "C. Newton", "D. Volt"},
        {"A. Einstein", "B. Newton", "C. Galileo", "D. Tesla"},
        {"A. 3x10^8 m/s", "B. 3x10^6 m/s", "C. 3x10^10 m/s", "D. 3x10^4 m/s"},
        {"A. Newton's 1st", "B. Newton's 2nd", "C. Newton's 3rd", "D. Kepler's 1st"},
        {"A. Moon", "B. Sun", "C. Wind", "D. Oceans"},
        {"A. Proton", "B. Neutron", "C. Electron", "D. Positron"},
        {"A. mv^2", "B. 1/2 mv^2", "C. mgh", "D. 1/2 kx^2"},
        {"A. Kinetic", "B. Thermal", "C. Potential", "D. Chemical"},
        {"A. Ohm", "B. Ampere", "C. Volt", "D. Watt"},
        {"A. Red", "B. Blue", "C. Green", "D. Violet"},
        {"A. 9.8 m/s^2", "B. 8.9 m/s^2", "C. 10 m/s^2", "D. 9 m/s^2"},
        {"A. Oxygen", "B. Nitrogen", "C. Carbon Dioxide", "D. Hydrogen"}
    };

    char answers[14] = {'A', 'C', 'B', 'B', 'A', 'B', 'B', 'C', 'B', 'C', 'A', 'D', 'A', 'B'};
    int prizeMoney[14] = {1000, 2000, 3000, 5000, 10000, 20000, 40000, 80000,
                           160000, 320000, 640000, 1250000, 2500000, 1000000};

    int score = 0, fiftyUsed = 0, swapUsed = 0, currentQuestion = 0;
    char choice;

    clearScreen();
    printf("Welcome to KBC!\n");

    while (currentQuestion < 14) {
        printf("\n=== SCOREBOARD ===\n");
        printf("Current Prize: $%d\n", score);
        printf("Next Prize: $%d\n", prizeMoney[currentQuestion]);

        printf("\nQuestion %d:\n", currentQuestion + 1);
        printf("%s\n", questions[currentQuestion]);

        printf("Lifelines: [F] 50/50%s [S] Swap%s\n",
               fiftyUsed ? " (Used)" : "", swapUsed ? " (Used)" : "");

        printf("Enter your choice (A-D, F, S, or Q to quit): ");
        scanf(" %c", &choice);
        choice = toupper(choice);

        if (choice == 'Q') break;

        if (choice >= 'A' && choice <= 'D') {
            if (choice == answers[currentQuestion]) {
                score = prizeMoney[currentQuestion];
                printf("Correct! You've won $%d\n", score);
                currentQuestion++;
            } else {
                printf("Wrong! Game Over! You won: $%d\n", score);
                getchar(); getchar();
                return;
            }
        }
    }

    if (score >= 1000000) {
        printf("Congratulations! You won $1,000,000!\n");
    }
    getchar(); getchar();
}

void hangmanGame() {
    char *words[] = {"Ethanol", "Uranium", "Nitrogen"};
    char word[20], display[20];
    int attempts = 6, wordLen, correct = 0;
    char guess;

    clearScreen();
    printf("Welcome to Hangman!\n");
    strcpy(word, words[rand() % 3]);
    wordLen = strlen(word);

    for (int i = 0; i < wordLen; i++) display[i] = '_';
    display[wordLen] = '\0';

    while (attempts > 0 && correct < wordLen) {
        printf("\nAttempts left: %d\n", attempts);
        printf("%s\n", display);
        printf("Guess a letter: ");
        scanf(" %c", &guess);
        guess = toupper(guess);

        int found = 0;
        for (int i = 0; i < wordLen; i++) {
            if (word[i] == guess) {
                display[i] = guess;
                correct++;
                found = 1;
            }
        }
        if (!found) attempts--;
    }

    printf(correct == wordLen ? "\nCongratulations! You won!\n" : "\nGame Over!\n");
    getchar(); getchar();
}

void numberGame() {
    int num1, num2, answer, userAnswer, score = 0;
    char operator;

    clearScreen();
    printf("Welcome to Number Generator!\n");

    for (int i = 0; i < 5; i++) {
        num1 = rand() % 100;
        num2 = rand() % 100;
        operator = "+-*"[rand() % 3];

        printf("\nQuestion %d: %d %c %d = ", i + 1, num1, operator, num2);
        scanf("%d", &userAnswer);

        printf(userAnswer == answer ? "Correct!\n" : "Wrong!\n");
    }
    getchar(); getchar();
}
