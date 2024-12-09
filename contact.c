#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

#define maxPhone 12
#define maxContact 100
#define sevenDaysInSec 604800

// Structure for all contact information
struct Info {
    char name[31];
    char phone[maxPhone];
    char email[31];
    time_t deleteTime;
};

// Declare function prototypes
void addContact();
int validatePhone(char* phone);
int validateEmail(char* email);
void trimWhitespace(char* str);
int checkRepeatedContact(char* phone);
void toLowerCase(char *str);
void searchContact();
void deleteContact();
void restoreContact();
void editContact();
void bubbleSort(struct Info contacts[], int n, int sortOption);
void displayContacts(const char *filename);
int readContacts(struct Info contacts[]);
void writeContacts(struct Info contacts[], int n);
void sortContacts();
void displaySortedContacts(struct Info contacts[], int n);

// Main function
int main(void) {
 
    char choice[2];  
    
    do {
        // Display menu
        printf("\n----------------------------\n");
        printf(" Contact Management System\n");
        printf("          Menu\n");
        printf("----------------------------\n");
        printf("\n1. Add New Contact\n");
        printf("2. Search for Contact\n");
        printf("3. Delete Contact\n");
        printf("4. Restore Contact\n");
        printf("5. Edit Contact\n");
        printf("6. Sort Contacts\n");
        printf("7. Display Contacts\n");
        printf("8. Save and Exit\n");

        printf("\nPlease input your choice: ");
        scanf("%s", choice);  // Read input as string
 
        
        if (strlen(choice) != 1) {
            printf("Invalid choice! Please select a valid option (1-5).\n");
            continue;
        }

        switch (choice[0]) {
            case '1': 
                addContact(); 
                break;
            case '2': 
                searchContact(); 
                break;
            case '3': 
                deleteContact(); 
                break;
            case '4': 
                restoreContact(); 
                break;
            case '5': 
                editContact();
                break;
            case '6': 
                sortContacts();
                break;
            case '7':
                displayContacts("contact.txt");
                break;
            case '8':
                printf("Exiting the program. Goodbye!\n"); 
                break;
            default: 
                printf("Invalid choice! Please select a valid option (1-8).\n");
        }

    }  while (choice[0] != '8');  // Loop until user chooses to exit

}


// Check validity of phone number
int validatePhone(char* phone) {
    int length = strlen(phone);

    // Check if all characters are digits
    for (int i = 0; i < length; i++) {
        if (!isdigit(phone[i])) {
            return 0; // Invalid if any character is not a digit
        }
    }

    // UK mobile format
    if ((length == 10 || length ==11) && phone[0] == '0') {
        
        // UK mobile: 07xxx xxxxxx
        if (phone[1] == 7) {
            return 1;
        }
    
    
    // UK landline: 01x || 02x || 03x || 05x || 08x
        if (phone[1] == '1' || phone[1] == '2' || phone[1] == '3' || phone[1] == '5' || phone[1] == '8') {
            return 1;
        }
    } 

    // Malaysia mobile format
    if (phone[0] == '0') {
        // mobile: 010 || 011 || 012 || 013 || 014 || 016 || 017 || 018 || 019
        if (phone[1] == '1' && phone[2] != '5' && (length == 10 || length == 11)) {
            return 1;
        }

        //landline:  03 || 04 || 05 || 06 || 07
        if ((phone[1] == '3' || phone[1] == '4' || phone[1] == '5' || phone[1] == '6' || phone[1] == '7') && (length == 9 || length == 10)) {
            return 1;
        }
    }
    return 0;
}


// Check validity of email
int validateEmail(char* email) {
    trimWhitespace(email);
    int symbol = -1;
    int dot = -1; // .
    int symbolCount = 0;  // @
    int length = strlen(email);
    
    for (int i = 0; i < length; i++) {
        if (email[i] == '@') {
            symbolCount++;
            if (symbolCount == 1 && email[0] != '@') {
                symbol = i;  // '@' cannot be the first character and not more than 1
            }
        } 
        else if (email[i] == '.' && symbol != -1) {
            dot = i;  // '@' must be apper before '.'
        }
    }
    
    // Validate the email: exactly one '@', and a '.' after '@', and at least 2 characters after '.'
    if (symbolCount == 1 && dot != -1 && dot > symbol && length - dot > 2) {
        return 1;  // Valid email
    }
    return 0;  // Invalid email
}


// Reject repeated contact function
int checkRepeatedContact(char* phone) {
    FILE *file = fopen("contact.txt", "r");
    char line[200];
    struct Info contact;
    
    if (file == NULL) {
        printf("Error opening file!\n");
        return 0;
    }

    fgets(line, sizeof(line), file); 
    fgets(line, sizeof(line), file);


    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%s\t%s\t%s", contact.name, contact.phone, contact.email);

        // Check if the phone number is repeated
        if (strcmp(contact.phone, phone) == 0) {
            fclose(file);
            return 0;  // repeated
        }
    }

    fclose(file);
    return 1;  // new
}


// Function to trim leading and trailing whitespace
void trimWhitespace(char* str) {
    // Trim leading whitespace
    while (isspace((unsigned char)*str)) str++;
    
    // Trim trailing whitespace
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';  // Null terminate the trimmed string
}


// Add Contact Function
void addContact() {
    struct Info newContact;
    FILE *file;
    
    file = fopen("contact.txt", "a+");
    
    if(file == NULL) {
        printf("Error opening file!");
        return;
    }
    
    printf("\nName: ");
    scanf(" %30[^\n]%*c", newContact.name);
    
    
    // Display format to user in table form
    printf("\nPlease enter the phone number by following the format given below:\n") ;
    printf("\n-----------------------------------------------------------------------------------------\n");
    printf("UK Phone Format\t\t\t| Malaysia Phone Format\n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("Mobile    | 07x\t\t\t| Mobile    | 010x, 011x, 012x, 013x,\n");
    printf("          |   \t\t        | \t    | 014x, 016x, 017x, 018x, 019x\n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("Landline  | 01x, 02x, 03x\t| Landline  | 03x, 04x, 05x, 06x, 07x\n");
    printf("-----------------------------------------------------------------------------------------\n\n");

    do {
        printf("Phone Number (10 or 11 digits): ");
        scanf("%s", newContact.phone); 
        
        if (!validatePhone(newContact.phone)) {
            printf("\n***Invalid phone number!!!\n");
            printf("Please try again.\n\n\a");
        }
        
        
        if (!checkRepeatedContact(newContact.phone)) {
            printf("\nThis phone number is already in your contact list:\n");
            printf("Please try to search in contact list\n\a");
            printf("\nExiting to menu...\n");
            return;
        }

    } while (!validatePhone(newContact.phone)); 
    
    do {
        printf("Email address: ");
        scanf("%s", newContact.email);
        
        if (!validateEmail(newContact.email)) {
            printf("Invalid email format. Please try again:\n\a");
        }
    } while (!validateEmail(newContact.email));
    
    fseek(file, 0, SEEK_END);  // Go to the end of the file
    long fileSize = ftell(file);  // Determine if the file is empty or already contains data.
    
    if (fileSize == 0) {
        fprintf(file, "%-35s\t%-15s\t%-30s\n", "Name", "Phone", "Email"); // Header
        fprintf(file, "----------------------------------------------------------------\n");
    }

    // Write contact information in a formatted way (table format)
    fprintf(file, "%-35s\t%-15s\t%-30s\n", newContact.name, newContact.phone, newContact.email);
    fclose(file);
    printf("\nContact added successfully!\n\nName: %s\nPhone Number: %s\nEmail: %s\n", newContact.name, newContact.phone, newContact.email);
    printf("\nExiting to menu...\n");
}


// Function to make keyword into lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}


// Search contact function
void searchContact() {
    struct Info searchContact;
    FILE *file;
    char line[200];
    char keyword[100], lowerKeyword[100];
    char *terms[3];  // Array to hold the individual search terms
    int found = 0, foundCounter = 1;

    printf("Enter contact details (name, phone, email) separated by spaces: ");
    scanf(" %[^\n]", keyword);  // Read the keyword to search for

    printf("Contacts found from your contact list are: \n");

    // Convert the keyword to lowercase and trim whitespace
    strcpy(lowerKeyword, keyword);
    toLowerCase(lowerKeyword);
    trimWhitespace(lowerKeyword);

    // Split the input into terms (name, phone, email)
    char *token = strtok(lowerKeyword, " ");  // Split by space
    int i = 0;
    while (token != NULL && i < 3) {
        terms[i++] = token;
        token = strtok(NULL, " ");
    }

    // Open the contact.txt file in read mode
    file = fopen("contact.txt", "r");
    if (file == NULL) {
        printf("Error opening file or file does not exist!\n");
        return;
    }

    // Skip the header lines
    fgets(line, sizeof(line), file);  // Skip header line
    fgets(line, sizeof(line), file);  // Skip separator line

    // Read each line of the contact file
    while (fgets(line, sizeof(line), file)) {
        // Extract contact details
        char *portion = strtok(line, "\t");
        if (portion != NULL) {
            strcpy(searchContact.name, portion);  // Extract name
            trimWhitespace(searchContact.name);
        }

        portion = strtok(NULL, "\t");
        if (portion != NULL) {
            strcpy(searchContact.phone, portion);  // Extract phone
            trimWhitespace(searchContact.phone);
        }

        portion = strtok(NULL, "\t");
        if (portion != NULL) {
            strcpy(searchContact.email, portion);  // Extract email
            trimWhitespace(searchContact.email);
        }

        // Convert fields to lowercase for comparison
        char lowerName[100], lowerPhone[100], lowerEmail[100];
        strcpy(lowerName, searchContact.name);
        strcpy(lowerPhone, searchContact.phone);
        strcpy(lowerEmail, searchContact.email);

        toLowerCase(lowerName);
        toLowerCase(lowerPhone);
        toLowerCase(lowerEmail);

        // Check if all search terms match respective fields
        int matchFound = 1;
        for (int j = 0; j < i; j++) {
            if ((terms[j] && !strstr(lowerName, terms[j]) && !strstr(lowerPhone, terms[j]) && !strstr(lowerEmail, terms[j]))) {
                matchFound = 0;
                break;
            }
        }

        // If all search terms match, print the contact
        if (matchFound) {
            printf("Contact %d: Name: %s, Phone: %s, Email: %s\n", foundCounter, searchContact.name, searchContact.phone, searchContact.email);
            found = 1;
            foundCounter++;
        }
    }

    // If no match is found
    if (!found) {
        printf("No contacts found matching your search.\n");
    }

    fclose(file);
}


// Delete contact function
void deleteContact() {
    struct Info deleteContact;
    FILE *file, *tempFile, *binFile;
    char line[200];
    char keyword[100];
    int found = 0;
    int deleteIndex = -1;
    int currentIndex = 0;

    // Open contact file
    file = fopen("contact.txt", "r");
    if (file == NULL) {
        printf("Error opening file or file does not exist!\n");
        return;
    }

    // Open temporary file to store the contacts except the one to be deleted
    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }

    // Open bin file for storing deleted contacts
    binFile = fopen("bin.txt", "a");
    if (binFile == NULL) {
        printf("Error opening recycle bin file!\n");
        fclose(file);
        fclose(tempFile);
        return;
    }

    // Ask for the keyword or contact name to delete
    printf("Enter the name or keyword of the contact to delete: ");
    scanf(" %[^\n]", keyword);

    // Write headers to the temp file
    fprintf(tempFile, "%-35s\t%-15s\t%-30s\n", "Name", "Phone", "Email");
    fprintf(tempFile, "-------------------------------------------------------------------\n");

    // Write headers to the bin file if it is empty
    fseek(binFile, 0, SEEK_END);
    if (ftell(binFile) == 0) {
        fprintf(binFile, "%-35s\t%-15s\t%-30s\t%-25s\n", "Name", "Phone", "Email", "Deleted Time");
        fprintf(binFile, "----------------------------------------------------------------------------------------------------------------\n");
    }

    // Skip the header lines in the contact file
    fgets(line, sizeof(line), file);  // Skip header line
    fgets(line, sizeof(line), file);  // Skip separator line

    // Read each line of the contact file
    while (fgets(line, sizeof(line), file)) {
        currentIndex++;

        char *portion = strtok(line, "\t");
        if (portion != NULL) {
            strcpy(deleteContact.name, portion);
            trimWhitespace(deleteContact.name);
        }

        portion = strtok(NULL, "\t");
        if (portion != NULL) {
            strcpy(deleteContact.phone, portion);
            trimWhitespace(deleteContact.phone);
        }

        portion = strtok(NULL, "\t");
        if (portion != NULL) {
            strcpy(deleteContact.email, portion);
            trimWhitespace(deleteContact.email);
        }

        // Check if the keyword matches the contact name, phone, or email
        if (strstr(deleteContact.name, keyword) != NULL ||
            strstr(deleteContact.phone, keyword) != NULL ||
            strstr(deleteContact.email, keyword) != NULL) {
            found++;
            if (found > 1) {
                break; // No need to continue once multiple matches are found
            }
            deleteIndex = currentIndex; // Save index for single match
        } else {
            fprintf(tempFile, "%-35s\t%-15s\t%-30s\n", deleteContact.name, deleteContact.phone, deleteContact.email);
        }
    }

    if (found == 0) {
        printf("No contact found with the keyword: %s\n", keyword);
    } else if (found > 1) {
        printf("Multiple contacts match the keyword '%s'. Please be more specific.\n", keyword);
    } else {
        // Rewind file to locate the exact contact to delete
        rewind(file);
        fgets(line, sizeof(line), file);  // Skip header
        fgets(line, sizeof(line), file);  // Skip separator

        currentIndex = 0;
        while (fgets(line, sizeof(line), file)) {
            currentIndex++;

            char *portion = strtok(line, "\t");
            if (portion != NULL) {
                strcpy(deleteContact.name, portion);
                trimWhitespace(deleteContact.name);
            }

            portion = strtok(NULL, "\t");
            if (portion != NULL) {
                strcpy(deleteContact.phone, portion);
                trimWhitespace(deleteContact.phone);
            }

            portion = strtok(NULL, "\t");
            if (portion != NULL) {
                strcpy(deleteContact.email, portion);
                trimWhitespace(deleteContact.email);
            }

            if (currentIndex == deleteIndex) {
                // Record deletion in the bin file
                deleteContact.deleteTime = time(NULL);
                char deleteTimeStr[26];
                strncpy(deleteTimeStr, ctime(&deleteContact.deleteTime), 25);
                deleteTimeStr[24] = '\0'; // Remove the newline character

                fprintf(binFile, "%-35s\t%-15s\t%-30s\t%-25s\n",
                        deleteContact.name, deleteContact.phone,
                        deleteContact.email, deleteTimeStr);

                printf("\nContact '%s' deleted successfully and moved to recycle bin.\n", deleteContact.name);
                continue;
            }
        }
    }

    // Close files
    fclose(file);
    fclose(tempFile);
    fclose(binFile);

    if (found <= 1) {
        // Replace the original file with the temp file
        remove("contact.txt");
        rename("temp.txt", "contact.txt");
    } else {
        remove("temp.txt"); // Clean up temp file if no deletion occurred
    }
}


// Restore contact function
void restoreContact() {
    struct Info restoreContact;
    FILE *binFile, *contactFile, *tempFile;
    char line[200];
    char keyword[100];
    int found = 0, lineCount = 0;

    // Open the bin.txt file
    binFile = fopen("bin.txt", "r");
    if (binFile == NULL) {
        printf("Error opening file or file does not exist!\n");
        return;
    }

    // Count lines in bin.txt to check if it has contacts
    while (fgets(line, sizeof(line), binFile)) {
        lineCount++;
    }

    // If the file has 2 or fewer lines, it contains only headers or is empty
    if (lineCount <= 2) {
        printf("No contact in bin.\n");
        fclose(binFile);
        return;
    }

    // Reset file pointer to the beginning of the file
    rewind(binFile);

    // Open the contact.txt file to append the restored contacts
    contactFile = fopen("contact.txt", "a+");
    if (contactFile == NULL) {
        printf("Error opening contact file!\n");
        fclose(binFile);
        return;
    }

    // Open a temporary file to keep remaining contacts in the bin
    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file!\n");
        fclose(binFile);
        fclose(contactFile);
        return;
    }

    // Skip the headers in bin.txt (if present)
    fgets(line, sizeof(line), binFile); // Header row
    fgets(line, sizeof(line), binFile); // Separator row
    
    // Write headers to the temp file
    fprintf(tempFile, "%-35s\t%-15s\t%-30s\n", "Name", "Phone", "Email");
    fprintf(tempFile, "-------------------------------------------------------------------\n");

    // Display all the contacts in bin file
    displayContacts("bin.txt");

    printf("Enter the name or keyword to restore: ");
    scanf(" %[^\n]", keyword);

    // Read each line in the bin file
    while (fgets(line, sizeof(line), binFile)) {
        // Extract contact details from the line
        char *portion = strtok(line, "\t");
        if (portion != NULL) {
            strcpy(restoreContact.name, portion);   // Get name
            trimWhitespace(restoreContact.name);
        }

        portion = strtok(NULL, "\t");
        if (portion != NULL) {
            strcpy(restoreContact.phone, portion);  // Get phone
            trimWhitespace(restoreContact.phone);
        }

        portion = strtok(NULL, "\t");
        if (portion != NULL) {
            strcpy(restoreContact.email, portion);  // Get email
            trimWhitespace(restoreContact.email);
        }
        
        // Check if the keyword matches the name, phone, or email
        if (strstr(restoreContact.name, keyword) != NULL ||
            strstr(restoreContact.phone, keyword) != NULL ||
            strstr(restoreContact.email, keyword) != NULL) {
            found++;
            printf("Contact restored: %s\n", restoreContact.name);
            // Write the restored contact back to contact.txt
            fprintf(contactFile, "%-35s\t%-15s\t%-30s\n", restoreContact.name, restoreContact.phone, restoreContact.email);
        } else {
            // Keep the contact in the bin by writing to temp file
            fprintf(tempFile, "%-35s\t%-15s\t%-30s\n", restoreContact.name, restoreContact.phone, restoreContact.email);
        }
    }

    if (found == 0) {
        printf("No contact found with the keyword: %s\n", keyword);
    }

    // Close files
    fclose(binFile);
    fclose(contactFile);
    fclose(tempFile);

    // Replace the bin.txt with the temp.txt
    remove("bin.txt");
    rename("temp.txt", "bin.txt");
}


// Edit contact info
void editContact() {
    struct Info editContact;
    FILE *file, *tempFile;
    char line[200];
    char keyword[100];
    char tempInput[200]; // Buffer for user input
    int found = 0;

    // Open the original contact file in read mode
    file = fopen("contact.txt", "r");
    if (file == NULL) {
        printf("Error opening file or file does not exist!\n");
        return;
    }

    // Open a temporary file to store modified contacts
    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }


    // Ask for the keyword or contact name to edit
    printf("Enter the name or keyword of the contact to edit: ");
    scanf(" %[^\n]", keyword);
    getchar(); // Clear the newline character left by scanf

    // Skip header lines (if any)
    fgets(line, sizeof(line), file);  // Read header line
    fprintf(tempFile, "%s", line);    // Write header to temp file
    fgets(line, sizeof(line), file);  // Read separator line
    fprintf(tempFile, "%s", line);    // Write separator to temp file

    // Read each contact from the original file
    while (fgets(line, sizeof(line), file)) {
        char *portion;


        // Extract contact details from the line
        portion = strtok(line, "\t");
        if (portion != NULL) {
            strcpy(editContact.name, portion);
            trimWhitespace(editContact.name);
        }

        portion = strtok(NULL, "\t");
        if (portion != NULL) {
            strcpy(editContact.phone, portion);
            trimWhitespace(editContact.phone);
        }

        portion = strtok(NULL, "\t");
        if (portion != NULL) {
            strcpy(editContact.email, portion);
            trimWhitespace(editContact.email);
        }

        // Check if the keyword matches the contact name, phone, or email
        if (strstr(editContact.name, keyword) != NULL || 
            strstr(editContact.phone, keyword) != NULL || 
            strstr(editContact.email, keyword) != NULL) {
            found = 1;

            // Contact found, prompt user to edit the details
            printf("\nContact found:\n");
            printf("Name: %s, Phone: %s, Email: %s\n", editContact.name, editContact.phone, editContact.email);

            // Get new details from the user (retain old value if input is #)
            printf("Enter new name (press # to keep current: %s): ", editContact.name);
            fgets(tempInput, sizeof(tempInput), stdin);
            trimWhitespace(tempInput);
            if (strcmp(tempInput, "#") != 0 && strlen(tempInput) > 0) {
                strcpy(editContact.name, tempInput);
            } 

            // Display format to user in table form
            printf("\nPlease enter the phone number by following the format given below:\n");
            printf("\n-----------------------------------------------------------------------------------------\n");
            printf("UK Phone Format\t\t\t| Malaysia Phone Format\n");
            printf("-----------------------------------------------------------------------------------------\n");
            printf("Mobile    | 07x\t\t\t| Mobile    | 010x, 011x, 012x, 013x,\n");
            printf("          |   \t\t\t|           | 014x, 016x, 017x, 018x, 019x\n");
            printf("-----------------------------------------------------------------------------------------\n");
            printf("Landline  | 01x, 02x, 03x\t| Landline  | 03x, 04x, 05x, 06x, 07x\n");
            printf("-----------------------------------------------------------------------------------------\n\n");

            // Get the edited phone number
            do {
                printf("Enter new phone (press # to keep current: %s): ", editContact.phone);
                fgets(tempInput, sizeof(tempInput), stdin);
                trimWhitespace(tempInput);

                if (strcmp(tempInput, "#") != 0 && strlen(tempInput) > 0) {
                    if (!validatePhone(tempInput)) {
                        printf("Invalid phone format. Please try again:\n\a");
                    } else {
                        strcpy(editContact.phone, tempInput);
                        break; // Exit the loop once a valid email is entered
                    }
                }else {
                    break; // Exit if the user enters '#' or an empty string
                }

            } while(1);
        
            // Get the edited email
            do {
                printf("Enter new email (press # to keep current: %s): ", editContact.email);
                fgets(tempInput, sizeof(tempInput), stdin);
                trimWhitespace(tempInput);

                if (strcmp(tempInput, "#") != 0 && strlen(tempInput) > 0) {
                    if (!validateEmail(tempInput)) {
                        printf("Invalid email format. Please try again:\n\a");
                    } else {
                        strcpy(editContact.email, tempInput);
                        break; // Exit the loop once a valid email is entered
                    }
                } else {
                    break; // Exit if the user enters '#' or an empty string
                }
            } while (1); // Infinite loop until valid input is provided or '#' is entered

            printf("Contact updated successfully.\n");

            // Write the modified contact to the temporary file
            fprintf(tempFile, "");
            fprintf(tempFile, "%-35s\t%-15s\t%-30s\n", editContact.name, editContact.phone, editContact.email);
        } else {
            //Write the unmodified contact to the temporary file
            fprintf(tempFile, "%-35s\t%-15s\t%-30s\n", editContact.name, editContact.phone, editContact.email);
        }
    }

    if (!found) {
        printf("No contact found with the keyword: %s\n", keyword);
    }

    //Close both files
    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated temp file
    if (remove("contact.txt") == 0 && rename("temp.txt", "contact.txt") == 0) {
        printf("Contact updated successfully in contact.txt.\n");
    } else {
        printf("Error replacing the contact.txt file.\n");
    }
}


// Display all contacts in contact file
void displayContacts(const char *filename) {
    FILE *file;
    char line[255];

    // Open the file for reading
    file = fopen(filename, "r");

    // Check if the file was opened successfully
    if (file == NULL) {
        printf("No contacts recorded.\n");
        return;
    }

    // Read and display each line of the file
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    printf("End of contact list.\n\n");

    // Close the file
    fclose(file);
}


// Function to display sorted contacts
void displaySortedContacts(struct Info contacts[], int n) {
    printf("\n%-35s\t%-15s\t%-30s\n", "Name", "Phone", "Email");
    printf("---------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%-35s\t%-15s\t%-30s\n", contacts[i].name, contacts[i].phone, contacts[i].email);
    }

    printf("End of contact list.\n");
}


// Function to perform Bubble Sort
void bubbleSort(struct Info contacts[], int n, int sortOption) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int comparison = 0;

            if (sortOption == 1) {  // Sort by Name
                comparison = strcasecmp(contacts[j].name, contacts[j + 1].name);
            } else if (sortOption == 2) {  // Sort by Phone
                comparison = strcmp(contacts[j].phone, contacts[j + 1].phone);
            } else if (sortOption == 3) {  // Sort by Email
                comparison = strcasecmp(contacts[j].email, contacts[j + 1].email);
            }

            if (comparison > 0) {
                struct Info temp = contacts[j];
                contacts[j] = contacts[j + 1];
                contacts[j + 1] = temp;
            }
        }
    }
}


// Function to read contacts from a file
int readContacts(struct Info contacts[]) {
    FILE *file = fopen("contact.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open contact file.\n");
        return 0;
    }

    char line[200];
    int count = 0;

    // Skip header lines
    fgets(line, sizeof(line), file); // Header
    fgets(line, sizeof(line), file); // Separator

    // Read each line and parse the data
    while (fgets(line, sizeof(line), file) && count < maxContact) {
        struct Info tempContact;

        // Parse line into name, phone, and email
        if (sscanf(line, "%30[^\t]\t%11[^\t]\t%30[^\n]",
                   tempContact.name, tempContact.phone, tempContact.email) == 3) {
            // Ensure no duplicate entries
            int isDuplicate = 0;
            for (int i = 0; i < count; i++) {
                if (strcmp(contacts[i].name, tempContact.name) == 0 &&
                    strcmp(contacts[i].phone, tempContact.phone) == 0 &&
                    strcmp(contacts[i].email, tempContact.email) == 0) {
                    isDuplicate = 1;
                    break;
                }
            }

            if (!isDuplicate) {
                contacts[count++] = tempContact;
            }
        }
    }

    fclose(file);
    return count;
}


// Function to write contacts back to the file
void writeContacts(struct Info contacts[], int n) {
    FILE *file = fopen("contact.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    // Write headers
    fprintf(file, "%-35s\t%-15s\t%-30s\n", "Name", "Phone", "Email");
    fprintf(file, "---------------------------------------------------------------------\n");

    // Write each contact
    for (int i = 0; i < n; i++) {
        fprintf(file, "%-35s\t%-15s\t%-30s\n", contacts[i].name, contacts[i].phone, contacts[i].email);
    }

    fclose(file);
}


// Sort contacts according to name, phone or email
void sortContacts() {
    struct Info contacts[maxContact];
    int n = readContacts(contacts);

    if (n == 0) {
        printf("No contacts to sort. Please add contacts first.\n");
        return;
    }

    int sortOption;
    printf("Choose sorting criterion:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &sortOption);

    if (sortOption < 1 || sortOption > 3) {
        printf("Invalid choice. Please try again.\n");
        return;
    }

    // Sort contacts
    bubbleSort(contacts, n, sortOption);

    // Display sorted contacts
    printf("\nSorted Contacts:\n");
    displaySortedContacts(contacts, n);

    // Save sorted contacts back to the file
    writeContacts(contacts, n);

    printf("Contacts sorted and saved successfully.\n");
}
