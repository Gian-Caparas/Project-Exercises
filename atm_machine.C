#include <stdio.h>      // For standard input and output functions
#include <string.h>     // For string manipulation functions
#include <termios.h>    // For terminal settings (to hide password input)
#include <unistd.h>     // For POSIX operating system API (used in terminal settings)

#define MAX_USERS 10          // Maximum number of users
#define USERNAME_LENGTH 50    // Maximum length for usernames
#define PASSWORD_LENGTH 50    // Maximum length for passwords

// Structure to hold user information
typedef struct {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    float balance;
} User;

User users[MAX_USERS]; // Array to hold users
int userCount = 0;     // Current number of users

// Function to add a new user
void addUser(const char* username, const char* password) {
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    users[userCount].balance = 0.0; // Initial balance
    userCount++;
}

// Function to hide password input
void getPassword(char* password) {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO); // Disable echoing of typed characters
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fgets(password, PASSWORD_LENGTH, stdin);
    password[strcspn(password, "\n")] = '\0'; // Remove newline character
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore settings
}

// Function to authenticate user credentials
User* authenticate(const char* username, const char* password) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            return &users[i]; // Return the user if found
        }
    }
    return NULL; // Return NULL if not found
}

int main() {
    int option;

    do {
        printf("\nATM Menu:\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);
        getchar(); // Consume newline left by scanf

        if (option == 1) {
            if (userCount < MAX_USERS) {
                char username[USERNAME_LENGTH];
                char password[PASSWORD_LENGTH];

                printf("Enter a username: ");
                scanf("%s", username);
                getchar(); // Consume newline left by scanf

                printf("Enter a password: ");
                getPassword(password); // Use custom password input function

                addUser(username, password);
                printf("Account created successfully!\n");
            } else {
                printf("Maximum user limit reached!\n");
            }
        } else if (option == 2) {
            char username[USERNAME_LENGTH];
            char password[PASSWORD_LENGTH];

            printf("Enter your username: ");
            scanf("%s", username);
            getchar(); // Consume newline left by scanf

            printf("Enter your password: ");
            getPassword(password); // Use custom password input function

            User* authenticatedUser = authenticate(username, password);

            if (authenticatedUser) {
                int choice;
                do {
                    printf("\nWelcome, %s! Your current balance is: %.2f\n", 
                           authenticatedUser->username, authenticatedUser->balance);
                    printf("1. Deposit\n2. Withdraw\n3. Logout\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);
                    getchar(); // Consume newline left by scanf

                    if (choice == 1) {
                        float deposit;
                        printf("Enter an amount for deposit: ");
                        scanf("%f", &deposit);
                        authenticatedUser->balance += deposit;
                        printf("Your new balance is: %.2f\n", authenticatedUser->balance);
                    } else if (choice == 2) {
                        float withdraw;
                        printf("Enter an amount to withdraw: ");
                        scanf("%f", &withdraw);

                        if (withdraw > authenticatedUser->balance) {
                            printf("Insufficient balance!\n");
                        } else {
                            authenticatedUser->balance -= withdraw;
                            printf("Your new balance is: %.2f\n", authenticatedUser->balance);
                        }
                    }
                } while (choice != 3);
            } else {
                printf("Invalid username or password.\n");
            }
        }
    } while (option != 3);

    printf("Thank you for using the ATM. Goodbye!\n");
    return 0;
}
