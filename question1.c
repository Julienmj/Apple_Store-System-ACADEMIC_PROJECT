

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>



#define MAX_RECORDS 500
#define MAX_NAME_LENGTH 50
#define MAX_DATE_LENGTH 12
#define FILENAME "question1_data.txt"

// Custom categories support
#define MAX_CUSTOM_CATEGORIES 20
#define MAX_PRODUCTS_PER_CUSTOM 20
#define MAX_CATEGORY_NAME 30

char customCategories[MAX_CUSTOM_CATEGORIES][MAX_CATEGORY_NAME];
int customCategoryCount = 0;
char customProducts[MAX_CUSTOM_CATEGORIES][MAX_PRODUCTS_PER_CUSTOM][MAX_NAME_LENGTH];
int customProductCount[MAX_CUSTOM_CATEGORIES];

// Structure definition for Product Record
typedef struct {
    int id;
    char productName[MAX_NAME_LENGTH];
    float price;
    char date[MAX_DATE_LENGTH];
    char category[20];
} Record;

// Global variables
Record records[MAX_RECORDS];
int recordCount = 0;

// Apple Product Lists (Shortened names for better display)
const char* iphones[] = {
    "iPhone 15 Pro Max",
    "iPhone 15 Pro",
    "iPhone 15",
    "iPhone 14 Pro",
    "iPhone 14"
};

const char* macbooks[] = {
    "MBP 16\" M3",
    "MBP 14\" M3",
    "MBA 15\" M2",
    "MBA 13\" M2",
    "MBP 13\" M2"
};

const char* iwatch[] = {
    "Watch Series 9",
    "Watch Ultra 2",
    "Watch SE",
    "Watch Series 8",
    "Watch Series 7"
};

// Function prototypes
void maximizeConsoleWindow();
void displayHeader();
void displayMenu();
void clearInputBuffer();
int getConsoleWidth();
void printCentered(const char* s);
void printCenteredNoNL(const char* s);
int validateID(int id);
int validatePrice(float price);
int validateDate(char* date);
void insertRecord();
void displayAllRecords();
void searchMenu();
void searchByID();
void searchByDate();
void searchByName();
void searchByCategory();
void deleteByID();
void updateRecord();
void sortRecords();
void displaySortMenu();
void sortByPriceAscending();
void sortByPriceDescending();
void sortByDateAscending();
void sortByDateDescending();
int compareDates(char* date1, char* date2);
void exportReport();
void loadFromFile();
void saveToFile();
void clearScreen();
void displayCategoryMenu();
int getCategoryChoice();
void displayProductSubmenu(int category);
int getProductChoice(int category);
void createCategoryInteractive();
void toLowerCase(const char* src, char* dest, size_t size);
int generateNextID(int categoryChoice);

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    int choice;
    
    // Set stdout to unbuffered mode for immediate output in debugger
    setvbuf(stdout, NULL, _IONBF, 0);
    
    // Maximize console window
    maximizeConsoleWindow();
    
    // Enable taskbar tab
    HWND hwnd = GetConsoleWindow();
    if (hwnd != NULL) {
        ShowWindow(hwnd, SW_NORMAL);
        SetForegroundWindow(hwnd);
    }
    
    // Display welcome message
    displayHeader();
    
    // Load existing data
    loadFromFile();
    
    // Main program loop
    while(1) {
        displayMenu();
        printCenteredNoNL(">>> Enter your choice (1-8): ");
        fflush(stdout);
        
        // Input validation
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            clearScreen();
            printf("\n[ERROR] Invalid input! Please enter a number.\n\n");
            continue;
        }
        
        clearInputBuffer();
        clearScreen();
        
        // Process user choice
        switch(choice) {
            case 1:
                insertRecord();
                break;
            case 2:
                displayAllRecords();
                break;
            case 3:
                searchMenu();
                break;
            case 4:
                deleteByID();
                break;
            case 5:
                sortRecords();
                break;
            case 6:
                updateRecord();
                break;
            case 7:
                exportReport();
                break;
            case 8:
                printf("\nSaving all data...\n");
                saveToFile();
                printf("[OK] Data saved successfully!\n");
                printf("\nThank you for using MY APPLE-STORE SYSTEM!\n");
                printf("   Program terminated.\n\n");
                return 0;
            default:
                printf("\n[ERROR] Invalid choice! Please select 1-8.\n\n");
        }
        
        // Pause before showing menu again
        if (choice != 8) {
            printf("\n--------------------------------------------------------\n");
            printf("Press Enter to return to main menu...");
            fflush(stdout);
            getchar();
            clearScreen();
        }
    }
    
    return 0;
}

// ============================================================================
// DISPLAY FUNCTIONS
// ============================================================================
void displayHeader() {
    clearScreen();
    printf("\n\n\n\n\n\n");
    printCentered("************************************************************");
    printCentered("*                                                          *");
    printCentered("*           MY APPLE-STORE SYSTEM                          *");
    printCentered("*                                                          *");
    printCentered("*         Record/Data Management System                    *");
    printCentered("*                                                          *");
    printCentered("************************************************************");
}

void displayMenu() {
    printf("\n");
    printCentered("************************************************************");
    printCentered("*                    MAIN MENU                             *");
    printCentered("************************************************************");
    printCentered("*                                                          *");
    printCentered("*   1. Insert New Record                                   *");
    printCentered("*   2. Display All Records                                 *");
    printCentered("*   3. Search Records                                      *");
    printCentered("*   4. Delete a Record by ID                               *");
    printCentered("*   5. Sort Records                                        *");
    printCentered("*   6. Update an Existing Record                           *");
    printCentered("*   7. Export Stock Report                                 *");
    printCentered("*   8. Exit and Save                                       *");
    printCentered("*                                                          *");
    printCentered("************************************************************");
    printf("\n\n\n\n");
}

// ============================================================================
// VALIDATION FUNCTIONS
// ============================================================================
int validateID(int id) {
    if (id <= 0) {
        printf("[ERROR] ID must be a positive number!\n");
        return 0;
    }
    
    // Check for duplicate ID
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == id) {
            printf("[ERROR] ID %d already exists!\n", id);
            return 0;
        }
    }
    return 1;
}

int validatePrice(float price) {
    if (price <= 0) {
        printf("[ERROR] Price must be a positive number!\n");
        return 0;
    }
    if (price > 100000) {
        printf("[ERROR] Price value too large (max $100,000)!\n");
        return 0;
    }
    return 1;
}

int validateDate(char* date) {
    int day, month, year;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Check if date string is empty
    if (date == NULL || strlen(date) == 0) {
        printf("[ERROR] Date cannot be empty!\n");
        return 0;
    }
    
    // Check format: should be DD/MM/YYYY (10 characters)
    if (strlen(date) != 10) {
        printf("[ERROR] Date format must be DD/MM/YYYY (e.g., 15/01/2024)!\n");
        return 0;
    }
    
    // Check if slashes are in correct positions
    if (date[2] != '/' || date[5] != '/') {
        printf("[ERROR] Date format must be DD/MM/YYYY (use '/' as separator)!\n");
        return 0;
    }
    
    // Parse date components
    if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3) {
        printf("[ERROR] Invalid date format! Use DD/MM/YYYY (e.g., 15/01/2024)!\n");
        return 0;
    }
    
    // Validate year (reasonable range: 2000-2100)
    if (year < 2000 || year > 2100) {
        printf("[ERROR] Year must be between 2000 and 2100!\n");
        return 0;
    }
    
    // Validate month
    if (month < 1 || month > 12) {
        printf("[ERROR] Month must be between 1 and 12!\n");
        return 0;
    }
    
    // Check for leap year (February has 29 days in leap year)
    if (month == 2) {
        int isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeapYear) {
            daysInMonth[1] = 29;  // February has 29 days in leap year
        } else {
            daysInMonth[1] = 28;  // February has 28 days in non-leap year
        }
    }
    
    // Validate day
    if (day < 1 || day > daysInMonth[month - 1]) {
        printf("[ERROR] Day must be between 1 and %d for month %d!\n", daysInMonth[month - 1], month);
        return 0;
    }
    
    return 1;  // Date is valid
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// ============================================================================
// MAXIMIZE CONSOLE WINDOW FUNCTION
// ============================================================================
void maximizeConsoleWindow() {
    HWND hwnd = GetConsoleWindow();
    
    if (hwnd != NULL) {
        ShowWindow(hwnd, SW_MAXIMIZE);
    }
}

// ============================================================================
// Console helpers for centering text
// ============================================================================
int getConsoleWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return 80;
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return (width > 0) ? width : 80;
#else
    return 80;
#endif
}

void printCentered(const char* s) {
    int w = getConsoleWidth();
    int len = (int)strlen(s);
    int pad = (w - len) / 2;
    if (pad < 0) pad = 0;
    for (int i = 0; i < pad; i++) putchar(' ');
    printf("%s\n", s);
}

void printCenteredNoNL(const char* s) {
    int w = getConsoleWidth();
    int len = (int)strlen(s);
    int pad = (w - len) / 2;
    if (pad < 0) pad = 0;
    for (int i = 0; i < pad; i++) putchar(' ');
    printf("%s", s);
}

// ============================================================================
// CATEGORY AND PRODUCT MENU FUNCTIONS
// ============================================================================
void displayCategoryMenu() {
    printf("\n--------------------------------------------------------------\n");
    printf("                    SELECT CATEGORY                            \n");
    printf("--------------------------------------------------------------\n");
    // Default categories
    printf("  1. iPhones                                                 \n");
    printf("  2. MacBooks                                                 \n");
    printf("  3. iWatch                                                   \n");
    // Custom categories (if any)
    for (int i = 0; i < customCategoryCount; i++) {
        printf("  %d. %s\n", 4 + i, customCategories[i]);
    }
    printf("  C. Create New Category                                     \n");
    printf("  B. Back to Main Menu                                       \n");
    printf("--------------------------------------------------------------\n");
}

int getCategoryChoice() {
    char input[10];
    int choice;
    
    while (1) {
        printf("Select category (1-%d, C to create, B to go back): ", 3 + customCategoryCount);
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        // Check for back
        if (input[0] == 'B' || input[0] == 'b') {
            return -1;  // Back signal
        }

        // Check for create new category
        if (input[0] == 'C' || input[0] == 'c') {
            return -2; // Create new category signal
        }

        // Try to parse as number
        if (sscanf(input, "%d", &choice) == 1) {
            int maxChoice = 3 + customCategoryCount; // 3 default + custom
            if (choice >= 1 && choice <= maxChoice) return choice;
        }
        
        printf("[ERROR] Invalid choice! Please try again.\n");
    }
}

void displayProductSubmenu(int category) {
    printf("\n--------------------------------------------------------------\n");
    printf("                    SELECT PRODUCT                            \n");
    printf("--------------------------------------------------------------\n");
    
    // Default categories
    if (category == 1) {  // iPhones
        for (int i = 0; i < 5; i++) {
            printf("  %d. %s\n", i + 1, iphones[i]);
        }
    } else if (category == 2) {  // MacBooks
        for (int i = 0; i < 5; i++) {
            printf("  %d. %s\n", i + 1, macbooks[i]);
        }
    } else if (category == 3) {  // iWatch
        for (int i = 0; i < 5; i++) {
            printf("  %d. %s\n", i + 1, iwatch[i]);
        }
    } else {
        // Custom category (category number > 3)
        int customIndex = category - 4; // 0-based
        if (customIndex >= 0 && customIndex < customCategoryCount) {
            if (customProductCount[customIndex] > 0) {
                for (int i = 0; i < customProductCount[customIndex]; i++) {
                    printf("  %d. %s\n", i + 1, customProducts[customIndex][i]);
                }
            } else {
                printf("  (No products defined - you will be prompted to enter a product name)\n");
            }
        } else {
            printf("  [ERROR] Invalid category selected.\n");
        }
    }
    
    printf("  B. Back to Category Menu                                  \n");
    printf("--------------------------------------------------------------\n");
}

int getProductChoice(int category) {
    char input[10];
    int choice;
    
    while (1) {
        int maxProducts = 5;
        if (category > 3) {
            int ci = category - 4;
            if (ci >= 0 && ci < customCategoryCount) maxProducts = customProductCount[ci] > 0 ? customProductCount[ci] : 0;
        }
        if (maxProducts > 0)
            printf("Select product (1-%d, B to go back): ", maxProducts);
        else
            printf("No predefined products (you will be prompted), press B to go back or Enter to continue: ");
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        // Check for back
        if (input[0] == 'B' || input[0] == 'b') {
            return -1;  // Back signal
        }
        
        // Try to parse as number
        if (sscanf(input, "%d", &choice) == 1) {
            if (maxProducts > 0 && choice >= 1 && choice <= maxProducts) {
                return choice;
            }
        }
        
        printf("[ERROR] Invalid choice! Please try again.\n");
    }
}

// Interactive creation of a custom category and its products
void createCategoryInteractive() {
    if (customCategoryCount >= MAX_CUSTOM_CATEGORIES) {
        printf("[ERROR] Maximum custom categories reached (%d)!\n", MAX_CUSTOM_CATEGORIES);
        return;
    }

    char name[MAX_CATEGORY_NAME];
    printf("Enter new category name: ");
    fflush(stdout);
    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("[ERROR] Failed to read category name.\n");
        return;
    }
    name[strcspn(name, "\n")] = 0;
    if (strlen(name) == 0) {
        printf("[ERROR] Category name cannot be empty.\n");
        return;
    }

    // Check for duplicates
    for (int i = 0; i < customCategoryCount; i++) {
        if (strcmp(customCategories[i], name) == 0) {
            printf("[ERROR] Category '%s' already exists.\n", name);
            return;
        }
    }

    strncpy(customCategories[customCategoryCount], name, MAX_CATEGORY_NAME - 1);
    customCategories[customCategoryCount][MAX_CATEGORY_NAME - 1] = '\0';

    // Ask number of products to add
    int pcount = 0;
    printf("How many products to add for '%s' (0-%d): ", name, MAX_PRODUCTS_PER_CUSTOM);
    fflush(stdout);
    if (scanf("%d", &pcount) != 1) {
        clearInputBuffer();
        pcount = 0;
    }
    if (pcount < 0) pcount = 0;
    if (pcount > MAX_PRODUCTS_PER_CUSTOM) pcount = MAX_PRODUCTS_PER_CUSTOM;
    clearInputBuffer();

    for (int i = 0; i < pcount; i++) {
        char pname[MAX_NAME_LENGTH];
        printf("Enter name for product %d: ", i + 1);
        fflush(stdout);
        if (fgets(pname, sizeof(pname), stdin) == NULL) {
            pname[0] = '\0';
        }
        pname[strcspn(pname, "\n")] = 0;
        if (strlen(pname) == 0) {
            printf("[WARN] Empty name - skipping.\n");
            continue;
        }
        strncpy(customProducts[customCategoryCount][customProductCount[customCategoryCount]], pname, MAX_NAME_LENGTH - 1);
        customProducts[customCategoryCount][customProductCount[customCategoryCount]][MAX_NAME_LENGTH - 1] = '\0';
        customProductCount[customCategoryCount]++;
    }

    printf("[OK] Category '%s' created with %d product(s).\n", customCategories[customCategoryCount], customProductCount[customCategoryCount]);
    customCategoryCount++;
}

// ============================================================================
// FILE OPERATIONS
// ============================================================================
void loadFromFile() {
    FILE *file = fopen(FILENAME, "r");
    
    if (file == NULL) {
        recordCount = 0;
        return;
    }
    
    recordCount = 0;
    char line[300];
    while (recordCount < MAX_RECORDS && fgets(line, sizeof(line), file) != NULL) {
        // Parse: ID|ProductName|ItemNumber|Price|Date|Category
        char *p = line;
        char *end;
        
        // Get ID
        records[recordCount].id = (int)strtol(p, &end, 10);
        if (end == p || *end != '|') continue;
        p = end + 1;
        
        // Get Product Name
        end = strchr(p, '|');
        if (end == NULL) continue;
        *end = '\0';
        strncpy(records[recordCount].productName, p, MAX_NAME_LENGTH - 1);
        records[recordCount].productName[MAX_NAME_LENGTH - 1] = '\0';
        p = end + 1;
        
        // Get Price
        records[recordCount].price = (float)strtod(p, &end);
        if (end == p || *end != '|') continue;
        p = end + 1;
        
        // Get Date
        end = strchr(p, '|');
        if (end == NULL) continue;
        *end = '\0';
        strncpy(records[recordCount].date, p, MAX_DATE_LENGTH - 1);
        records[recordCount].date[MAX_DATE_LENGTH - 1] = '\0';
        p = end + 1;
        
        // Get Category (rest of line, remove newline)
        end = strchr(p, '\n');
        if (end != NULL) *end = '\0';
        strncpy(records[recordCount].category, p, 19);
        records[recordCount].category[19] = '\0';
        
        recordCount++;
    }
    
    fclose(file);
}

void saveToFile() {
    FILE *file = fopen(FILENAME, "w");
    
    if (file == NULL) {
        printf("[ERROR] Cannot save to file!\n");
        return;
    }
    
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "%d|%s|%.2f|%s|%s\n", 
                records[i].id,
                records[i].productName,
                records[i].price,
                records[i].date,
                records[i].category);
    }
    
    fclose(file);
}

// ============================================================================
// GENERATE NEXT AVAILABLE ID BASED ON CATEGORY
// ============================================================================
int generateNextID(int categoryChoice) {
    int baseID, maxID = 0;
    
    // Determine base ID based on category
    if (categoryChoice == 1) {
        baseID = 1000;  // iPhones: 1001, 1002, 1003...
    } else if (categoryChoice == 2) {
        baseID = 2000;  // MacBooks: 2001, 2002, 2003...
    } else if (categoryChoice == 3) {
        baseID = 3000;  // iWatch: 3001, 3002, 3003...
    } else if (categoryChoice >= 4) {
        // Custom categories: map to 4000 + index*1000
        int customIndex = categoryChoice - 4;
        if (customIndex >= 0 && customIndex < customCategoryCount) {
            baseID = 4000 + customIndex * 1000;
        } else {
            return -1; // invalid
        }
    } else {
        return -1;  // Invalid category
    }
    
    // Find the maximum ID in the same category range
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id >= baseID + 1 && records[i].id < baseID + 1000) {
            if (records[i].id > maxID) {
                maxID = records[i].id;
            }
        }
    }
    
    // Return next available ID
    return maxID + 1;
}

// ============================================================================
// CORE OPERATIONS
// ============================================================================

// 1. INSERT NEW RECORD
void insertRecord() {
    Record newRecord;
    int categoryChoice;
    int productChoice;
    int itemsToAdd;
    
    while (1) {
        printf("\n==============================================================\n");
        printf("              INSERT NEW RECORD                               \n");
        printf("==============================================================\n\n");
    
    // Check if array is full
    if (recordCount >= MAX_RECORDS) {
            printf("[ERROR] Maximum records limit reached (%d)!\n", MAX_RECORDS);
            printf("Press Enter to continue...");
            fflush(stdout);
            getchar();
        return;
    }
    
        // Display category menu
        displayCategoryMenu();
        categoryChoice = getCategoryChoice();
        
        if (categoryChoice == -1) {
            return;  // User chose to go back
        }
        if (categoryChoice == -2) {
            // Create new category
            createCategoryInteractive();
            continue; // show category menu again
        }
        
        // Display product submenu for selected category
        displayProductSubmenu(categoryChoice);
        productChoice = getProductChoice(categoryChoice);
        
        if (productChoice == -1) {
            continue;  // Go back to category menu
        }
        
        // Get quantity to add (1-5)
        printf("Enter quantity to add (1-5): ");
        fflush(stdout);
        if (scanf("%d", &itemsToAdd) != 1) {
        clearInputBuffer();
            printf("[ERROR] Invalid quantity format!\n");
            printf("Press Enter to continue...");
            fflush(stdout);
            getchar();
            continue;
        }
        
        if (itemsToAdd < 1 || itemsToAdd > 5) {
            printf("[ERROR] Quantity must be between 1 and 5!\n");
            printf("Press Enter to continue...");
            fflush(stdout);
            getchar();
            continue;
        }
        
    clearInputBuffer();
        
        // Set product name and category (common for all items)
        if (categoryChoice == 1) {
            strcpy(newRecord.productName, iphones[productChoice - 1]);
            strcpy(newRecord.category, "iPhones");
        } else if (categoryChoice == 2) {
            strcpy(newRecord.productName, macbooks[productChoice - 1]);
            strcpy(newRecord.category, "MacBooks");
        } else if (categoryChoice == 3) {
            strcpy(newRecord.productName, iwatch[productChoice - 1]);
            strcpy(newRecord.category, "iWatch");
        } else {
            // Custom category
            int customIndex = categoryChoice - 4;
            if (customIndex >= 0 && customIndex < customCategoryCount) {
                if (customProductCount[customIndex] > 0) {
                    // user selected a listed product
                    strcpy(newRecord.productName, customProducts[customIndex][productChoice - 1]);
                } else {
                    // No predefined products - prompt for product name
                    char pname[MAX_NAME_LENGTH];
                    printf("Enter Product Name: ");
                    fflush(stdout);
                    if (fgets(pname, sizeof(pname), stdin) == NULL) {
                        pname[0] = '\0';
                    }
                    pname[strcspn(pname, "\n")] = 0;
                    if (strlen(pname) == 0) {
                        printf("[ERROR] Product name cannot be empty!\n");
                        continue;
                    }
                    strncpy(newRecord.productName, pname, MAX_NAME_LENGTH - 1);
                    newRecord.productName[MAX_NAME_LENGTH - 1] = '\0';
                }
                strncpy(newRecord.category, customCategories[customIndex], 19);
                newRecord.category[19] = '\0';
            } else {
                printf("[ERROR] Invalid category selected!\n");
                continue;
            }
        }
        
        // Get Price (common for all items)
        printf("Enter Price ($): ");
        fflush(stdout);
        if (scanf("%f", &newRecord.price) != 1) {
            clearInputBuffer();
            printf("[ERROR] Invalid price format!\n");
            printf("Press Enter to continue...");
            fflush(stdout);
            getchar();
            continue;
        }
        
        if (!validatePrice(newRecord.price)) {
            printf("Press Enter to continue...");
            fflush(stdout);
            getchar();
            continue;
        }
        
        // Get Date (common for all items)
        clearInputBuffer();
        printf("Enter Date (DD/MM/YYYY): ");
        fflush(stdout);
        if (fgets(newRecord.date, MAX_DATE_LENGTH, stdin) == NULL) {
            printf("[ERROR] Failed to read date!\n");
            printf("Press Enter to continue...");
            fflush(stdout);
            getchar();
            continue;
    }
    
    // Remove newline character
        newRecord.date[strcspn(newRecord.date, "\n")] = 0;
        
        // Validate date format and values
        if (!validateDate(newRecord.date)) {
            printf("Press Enter to continue...");
            fflush(stdout);
            getchar();
            continue;
        }
        
        // Generate unique ID based on category
        int nextID = generateNextID(categoryChoice);
        if (nextID == -1) {
            printf("[ERROR] Invalid category for ID generation!\n");
            printf("Press Enter to continue...");
            fflush(stdout);
            getchar();
            continue;
        }
        
        // Add items based on quantity - auto-generate unique IDs for each item
        for (int item = 0; item < itemsToAdd; item++) {
            printf("\n--- Adding Item %d of %d ---\n", item + 1, itemsToAdd);
            
            // Auto-generate unique ID
            newRecord.id = nextID + item;
            
            // Verify the generated ID is unique (in case of gaps)
            while (!validateID(newRecord.id)) {
                newRecord.id++;
            }
            
            // Update nextID for next iteration
            nextID = newRecord.id + 1;
    
            // Add record to array
            records[recordCount] = newRecord;
            recordCount++;
            
            printf("[OK] Item %d added successfully! (ID: %d)\n", item + 1, newRecord.id);
        }
    
    // Save to file
    saveToFile();
    
        printf("\n[OK] All %d record(s) inserted successfully!\n", itemsToAdd);
        
        // Ask if user wants to add more
        printf("\nPress Enter to return to main menu, or type 'B' to add more items: ");
        fflush(stdout);
        char response[10];
        if (fgets(response, sizeof(response), stdin) != NULL) {
            response[strcspn(response, "\n")] = 0;
            if (response[0] != 'B' && response[0] != 'b') {
                break;  // Return to main menu
            }
        } else {
            break;
        }
    }
}

// 2. DISPLAY ALL RECORDS
void displayAllRecords() {
    printf("\n==============================================================\n");
    printf("              ALL RECORDS                                     \n");
    printf("==============================================================\n\n");
    
    if (recordCount == 0) {
        printf("No records found in the system.\n");
        return;
    }
    
    // Display table header
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    printf("| Product ID | Product Name                         | Price      | Date       | Category   |\n");
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    
    // Display all records
    for (int i = 0; i < recordCount; i++) {
        printf("| %-10d | %-36s | $%-9.2f | %-10s | %-10s |\n", 
               records[i].id, records[i].productName, 
               records[i].price, records[i].date, records[i].category);
    }
    
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    printf("\nTotal Records: %d\n", recordCount);
}

// 3. SEARCH MENU - Allows user to choose search criteria
void searchMenu() {
    int choice;
    
    while (1) {
        printf("\n==============================================================\n");
        printf("                    SEARCH MENU                               \n");
        printf("--------------------------------------------------------------\n");
        printf("  1. Search by ID                                            \n");
        printf("  2. Search by Date                                          \n");
        printf("  3. Search by Name                                          \n");
        printf("  4. Search by Category                                      \n");
        printf("  5. Back to Main Menu                                       \n");
        printf("==============================================================\n");
        printCenteredNoNL("Enter your choice (1-5): ");
        fflush(stdout);
        
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("[ERROR] Invalid input! Please enter a number.\n");
            printf("Press Enter to continue...");
            fflush(stdout);
            getchar();
            continue;
        }
        
        clearInputBuffer();
        
        switch(choice) {
            case 1:
                searchByID();
                return;
            case 2:
                searchByDate();
                return;
            case 3:
                searchByName();
                return;
            case 4:
                searchByCategory();
                return;
            case 5:
                return;  // Back to main menu
            default:
                printf("[ERROR] Invalid choice! Please select 1-5.\n");
                printf("Press Enter to continue...");
                fflush(stdout);
                getchar();
        }
    }
}

// SEARCH FOR A RECORD BY ID
void searchByID() {
    int searchID;
    int found = 0;
    
    printf("\n==============================================================\n");
    printf("              SEARCH RECORD BY ID                             \n");
    printf("==============================================================\n\n");
    
    printf("Enter Product ID to search: ");
    fflush(stdout);
    if (scanf("%d", &searchID) != 1) {
        clearInputBuffer();
        printf("[ERROR] Invalid ID format!\n");
        return;
    }
    
    clearInputBuffer();
    
    // Search for the record
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == searchID) {
            printf("\n[OK] Record Found!\n");
            printf("+--------------------------------------------------------+\n");
            printf("| Product ID    : %-40d |\n", records[i].id);
            printf("| Product Name  : %-40s |\n", records[i].productName);
            printf("| Price         : $%-38.2f |\n", records[i].price);
            printf("| Date          : %-40s |\n", records[i].date);
            printf("| Category      : %-40s |\n", records[i].category);
            printf("+--------------------------------------------------------+\n");
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("\n[ERROR] Record with ID %d not found!\n", searchID);
    }
}

// SEARCH FOR RECORDS BY DATE
void searchByDate() {
    char searchDate[MAX_DATE_LENGTH];
    int found = 0;
    int foundCount = 0;
    
    printf("\n==============================================================\n");
    printf("              SEARCH RECORD BY DATE                           \n");
    printf("==============================================================\n\n");
    
    printf("Enter Date to search (DD/MM/YYYY): ");
    fflush(stdout);
    if (fgets(searchDate, MAX_DATE_LENGTH, stdin) == NULL) {
        printf("[ERROR] Failed to read date!\n");
        return;
    }
    
    // Remove newline character
    searchDate[strcspn(searchDate, "\n")] = 0;
    
    // Validate date format
    if (!validateDate(searchDate)) {
        return;
    }
    
    // Search for records matching the date
    printf("\nSearching for records with date: %s\n", searchDate);
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    printf("| Product ID | Product Name                         | Price      | Date       | Category   |\n");
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(records[i].date, searchDate) == 0) {
            printf("| %-10d | %-36s | $%-9.2f | %-10s | %-10s |\n", 
                   records[i].id, records[i].productName, 
                   records[i].price, records[i].date, records[i].category);
            found = 1;
            foundCount++;
        }
    }
    
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    
    if (!found) {
        printf("\n[ERROR] No records found with date %s!\n", searchDate);
    } else {
        printf("\n[OK] Found %d record(s) with date %s!\n", foundCount, searchDate);
    }
}

// SEARCH FOR RECORDS BY NAME
void searchByName() {
    char searchName[MAX_NAME_LENGTH];
    int found = 0;
    int foundCount = 0;
    
    printf("\n==============================================================\n");
    printf("              SEARCH RECORD BY NAME                           \n");
    printf("==============================================================\n\n");
    
    printf("Enter Product Name to search: ");
    fflush(stdout);
    if (fgets(searchName, MAX_NAME_LENGTH, stdin) == NULL) {
        printf("[ERROR] Failed to read product name!\n");
        return;
    }
    
    // Remove newline character
    searchName[strcspn(searchName, "\n")] = 0;
    
    if (strlen(searchName) == 0) {
        printf("[ERROR] Product name cannot be empty!\n");
        return;
    }
    
    // Search for records matching the name (case-insensitive partial match)
    printf("\nSearching for records containing: %s\n", searchName);
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    printf("| Product ID | Product Name                         | Price      | Date       | Category   |\n");
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    
    for (int i = 0; i < recordCount; i++) {
        // Case-insensitive search using strstr
        if (strstr(records[i].productName, searchName) != NULL) {
            printf("| %-10d | %-36s | $%-9.2f | %-10s | %-10s |\n", 
                   records[i].id, records[i].productName, 
                   records[i].price, records[i].date, records[i].category);
            found = 1;
            foundCount++;
        }
    }
    
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    
    if (!found) {
        printf("\n[ERROR] No records found containing '%s'!\n", searchName);
    } else {
        printf("\n[OK] Found %d record(s) containing '%s'!\n", foundCount, searchName);
    }
}

// SEARCH FOR RECORDS BY CATEGORY
void searchByCategory() {
    char searchCategory[20];
    char categoryLower[20];
    char recordLower[20];
    int found = 0;
    int foundCount = 0;
    
    printf("\n==============================================================\n");
    printf("            SEARCH RECORD BY CATEGORY                         \n");
    printf("==============================================================\n\n");
    
    printf("Enter Category to search (e.g., iPhones, MacBooks, iWatch): ");
    fflush(stdout);
    if (fgets(searchCategory, sizeof(searchCategory), stdin) == NULL) {
        printf("[ERROR] Failed to read category!\n");
        return;
    }
    
    searchCategory[strcspn(searchCategory, "\n")] = 0;
    
    if (strlen(searchCategory) == 0) {
        printf("[ERROR] Category cannot be empty!\n");
        return;
    }
    
    toLowerCase(searchCategory, categoryLower, sizeof(categoryLower));
    
    printf("\nSearching for records in category: %s\n", searchCategory);
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    printf("| Product ID | Product Name                         | Price      | Date       | Category   |\n");
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    
    for (int i = 0; i < recordCount; i++) {
        toLowerCase(records[i].category, recordLower, sizeof(recordLower));
        if (strcmp(recordLower, categoryLower) == 0) {
            printf("| %-10d | %-36s | $%-9.2f | %-10s | %-10s |\n", 
                   records[i].id, records[i].productName, 
                   records[i].price, records[i].date, records[i].category);
            found = 1;
            foundCount++;
        }
    }
    
    printf("+------------+--------------------------------------+------------+------------+------------+\n");
    
    if (!found) {
        printf("\n[ERROR] No records found in category '%s'!\n", searchCategory);
    } else {
        printf("\n[OK] Found %d record(s) in category '%s'!\n", foundCount, searchCategory);
    }
}

// Helper to convert string to lowercase safely
void toLowerCase(const char* src, char* dest, size_t size) {
    size_t i;
    if (size == 0) return;
    for (i = 0; i + 1 < size && src[i] != '\0'; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

// 4. DELETE A RECORD BY ID
void deleteByID() {
    int deleteID;
    int found = 0;
    int index = -1;
    
    printf("\n==============================================================\n");
    printf("              DELETE RECORD BY ID                             \n");
    printf("==============================================================\n\n");
    
    if (recordCount == 0) {
        printf("[ERROR] No records to delete!\n");
        return;
    }
    
    printf("Enter Product ID to delete: ");
    fflush(stdout);
    if (scanf("%d", &deleteID) != 1) {
        clearInputBuffer();
        printf("[ERROR] Invalid ID format!\n");
        return;
    }
    
    clearInputBuffer();
    
    // Find the record
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == deleteID) {
            index = i;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("\n[ERROR] Record with ID %d not found!\n", deleteID);
        return;
    }
    
    // Display record to be deleted
    printf("\n[WARNING] Record to be deleted:\n");
    printf("+--------------------------------------------------------+\n");
    printf("| Product ID    : %-40d |\n", records[index].id);
    printf("| Product Name  : %-40s |\n", records[index].productName);
    printf("| Price         : $%-38.2f |\n", records[index].price);
    printf("| Date          : %-40s |\n", records[index].date);
    printf("| Category      : %-40s |\n", records[index].category);
    printf("+--------------------------------------------------------+\n");
    
    // Shift all elements after the deleted one
    for (int i = index; i < recordCount - 1; i++) {
        records[i] = records[i + 1];
    }
    
    recordCount--;
    saveToFile();
    
    printf("\n[OK] Record deleted successfully!\n");
    printf("   Remaining records: %d\n", recordCount);
}

// 5. SORT RECORDS - Shows sub-menu for sorting options
void sortRecords() {
    int sortChoice;
    
    if (recordCount == 0) {
        printf("\n[ERROR] No records to sort!\n");
        return;
    }
    
    while (1) {
        displaySortMenu();
        printCenteredNoNL("Enter your choice (1-5): ");
        fflush(stdout);
        
        if (scanf("%d", &sortChoice) != 1) {
            clearInputBuffer();
            printf("[ERROR] Invalid input! Please enter a number.\n");
            printf("Press Enter to continue...");
            fflush(stdout);
            getchar();
            continue;
        }
        
        clearInputBuffer();
        
        switch(sortChoice) {
            case 1:
                sortByPriceAscending();
                return;
            case 2:
                sortByPriceDescending();
                return;
            case 3:
                sortByDateAscending();
                return;
            case 4:
                sortByDateDescending();
                return;
            case 5:
                return;  // Back to main menu
            default:
                printf("[ERROR] Invalid choice! Please select 1-5.\n");
                printf("Press Enter to continue...");
                fflush(stdout);
                getchar();
        }
    }
}

// Display sorting sub-menu
void displaySortMenu() {
    printf("\n==============================================================\n");
    printf("                    SORT RECORDS                               \n");
    printf("--------------------------------------------------------------\n");
    printf("  1. Sort by Price (Ascending - Lowest to Highest)           \n");
    printf("  2. Sort by Price (Descending - Highest to Lowest)         \n");
    printf("  3. Sort by Date (Ascending - Oldest to Newest)            \n");
    printf("  4. Sort by Date (Descending - Newest to Oldest)           \n");
    printf("  5. Back to Main Menu                                       \n");
    printf("==============================================================\n");
}

// Helper function to compare dates (DD/MM/YYYY format)
// Returns: -1 if date1 < date2, 0 if equal, 1 if date1 > date2
int compareDates(char* date1, char* date2) {
    int day1, month1, year1;
    int day2, month2, year2;
    
    // Parse date1 (DD/MM/YYYY)
    if (sscanf(date1, "%d/%d/%d", &day1, &month1, &year1) != 3) {
        return 0;  // Invalid date format
    }
    
    // Parse date2 (DD/MM/YYYY)
    if (sscanf(date2, "%d/%d/%d", &day2, &month2, &year2) != 3) {
        return 0;  // Invalid date format
    }
    
    // Compare years
    if (year1 < year2) return -1;
    if (year1 > year2) return 1;
    
    // Compare months
    if (month1 < month2) return -1;
    if (month1 > month2) return 1;
    
    // Compare days
    if (day1 < day2) return -1;
    if (day1 > day2) return 1;
    
    return 0;  // Dates are equal
}

// Sort by Price (Ascending - Bubble Sort)
void sortByPriceAscending() {
    printf("\n==============================================================\n");
    printf("     SORTING RECORDS BY PRICE (Ascending)                     \n");
    printf("==============================================================\n\n");
    
    // Bubble Sort Algorithm
    Record temp;
    int swapped;
    
    for (int i = 0; i < recordCount - 1; i++) {
        swapped = 0;
        for (int j = 0; j < recordCount - i - 1; j++) {
            if (records[j].price > records[j + 1].price) {
                temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break;  // Early exit if sorted
    }
    
    saveToFile();
    
    printf("[OK] Records sorted successfully by Price (Ascending)!\n");
    printf("   Algorithm: Bubble Sort\n");
    printf("   Order: Lowest -> Highest (by Price)\n\n");
    
    displayAllRecords();
}

// Sort by Price (Descending - Selection Sort)
void sortByPriceDescending() {
    printf("\n==============================================================\n");
    printf("     SORTING RECORDS BY PRICE (Descending)                    \n");
    printf("==============================================================\n\n");
    
    // Selection Sort Algorithm
    Record temp;
    int maxIndex;
    
    for (int i = 0; i < recordCount - 1; i++) {
        maxIndex = i;
        for (int j = i + 1; j < recordCount; j++) {
            if (records[j].price > records[maxIndex].price) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            temp = records[i];
            records[i] = records[maxIndex];
            records[maxIndex] = temp;
        }
    }
    
    saveToFile();
    
    printf("[OK] Records sorted successfully by Price (Descending)!\n");
    printf("   Algorithm: Selection Sort\n");
    printf("   Order: Highest -> Lowest (by Price)\n\n");
    
    displayAllRecords();
}

// Sort by Date (Ascending - Bubble Sort)
void sortByDateAscending() {
    printf("\n==============================================================\n");
    printf("     SORTING RECORDS BY DATE (Ascending)                      \n");
    printf("==============================================================\n\n");
    
    // Bubble Sort Algorithm
    Record temp;
    int swapped;
    
    for (int i = 0; i < recordCount - 1; i++) {
        swapped = 0;
        for (int j = 0; j < recordCount - i - 1; j++) {
            if (compareDates(records[j].date, records[j + 1].date) > 0) {
                temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break;  // Early exit if sorted
    }
    
    saveToFile();
    
    printf("[OK] Records sorted successfully by Date (Ascending)!\n");
    printf("   Algorithm: Bubble Sort\n");
    printf("   Order: Oldest -> Newest (by Date)\n\n");
    
    displayAllRecords();
}

// Sort by Date (Descending - Selection Sort)
void sortByDateDescending() {
    printf("\n==============================================================\n");
    printf("     SORTING RECORDS BY DATE (Descending)                     \n");
    printf("==============================================================\n\n");
    
    // Selection Sort Algorithm
    Record temp;
    int maxIndex;
    
    for (int i = 0; i < recordCount - 1; i++) {
        maxIndex = i;
        for (int j = i + 1; j < recordCount; j++) {
            if (compareDates(records[j].date, records[maxIndex].date) > 0) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            temp = records[i];
            records[i] = records[maxIndex];
            records[maxIndex] = temp;
        }
    }
    
    saveToFile();
    
    printf("[OK] Records sorted successfully by Date (Descending)!\n");
    printf("   Algorithm: Selection Sort\n");
    printf("   Order: Newest -> Oldest (by Date)\n\n");
    
    displayAllRecords();
}

// 6. UPDATE AN EXISTING RECORD
void updateRecord() {
    int updateID;
    int found = 0;
    int index = -1;
    int choice;
    float newPrice;
    char newDate[MAX_DATE_LENGTH];
    
    printf("\n==============================================================\n");
    printf("              UPDATE RECORD                                 \n");
    printf("==============================================================\n\n");
    
    printf("Enter Product ID to update: ");
    fflush(stdout);
    if (scanf("%d", &updateID) != 1) {
        clearInputBuffer();
        printf("[ERROR] Invalid ID format!\n");
        return;
    }
    
    clearInputBuffer();
    
    // Find the record
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == updateID) {
            index = i;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("\n[ERROR] Record with ID %d not found!\n", updateID);
        return;
    }
    
    // Display current record
    printf("\nCurrent Record Details:\n");
    printf("+--------------------------------------------------------+\n");
    printf("| Product ID    : %-40d |\n", records[index].id);
    printf("| Product Name  : %-40s |\n", records[index].productName);
    printf("| Price         : $%-38.2f |\n", records[index].price);
    printf("| Date          : %-40s |\n", records[index].date);
    printf("| Category      : %-40s |\n", records[index].category);
    printf("+--------------------------------------------------------+\n");
    
    // Get update choice
    printf("\nWhat would you like to update?\n");
    printf("  1. Price\n");
    printf("  2. Date\n");
    printf("  3. Both (Price and Date)\n");
    printf("Enter choice (1-3): ");
    fflush(stdout);
    
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf("[ERROR] Invalid choice!\n");
        return;
    }
    
    clearInputBuffer();
    
    // Process update
    switch(choice) {
        case 1:
            printf("Enter new Price ($): ");
            fflush(stdout);
            if (scanf("%f", &newPrice) != 1) {
                clearInputBuffer();
                printf("[ERROR] Invalid price format!\n");
                return;
            }
            if (!validatePrice(newPrice)) {
                return;
            }
            records[index].price = newPrice;
            break;
            
        case 2:
            printf("Enter new Date (DD/MM/YYYY): ");
            fflush(stdout);
            if (fgets(newDate, MAX_DATE_LENGTH, stdin) == NULL) {
                printf("[ERROR] Failed to read date!\n");
                return;
            }
            newDate[strcspn(newDate, "\n")] = 0;
            if (!validateDate(newDate)) {
                return;
            }
            strcpy(records[index].date, newDate);
            break;
            
        case 3:
            printf("Enter new Price ($): ");
            fflush(stdout);
            if (scanf("%f", &newPrice) != 1) {
                clearInputBuffer();
                printf("[ERROR] Invalid price format!\n");
                return;
            }
            if (!validatePrice(newPrice)) {
                return;
            }
            records[index].price = newPrice;
            
                clearInputBuffer();
            printf("Enter new Date (DD/MM/YYYY): ");
            fflush(stdout);
            if (fgets(newDate, MAX_DATE_LENGTH, stdin) == NULL) {
                printf("[ERROR] Failed to read date!\n");
                return;
            }
            newDate[strcspn(newDate, "\n")] = 0;
            if (!validateDate(newDate)) {
                return;
            }
            strcpy(records[index].date, newDate);
            break;
            
        default:
            printf("[ERROR] Invalid choice!\n");
            return;
    }
    
    clearInputBuffer();
    saveToFile();
    
    printf("\n[OK] Record updated successfully!\n");
    printf("+--------------------------------------------------------+\n");
    printf("| Product ID    : %-40d |\n", records[index].id);
    printf("| Product Name  : %-40s |\n", records[index].productName);
    printf("| Price         : $%-38.2f |\n", records[index].price);
    printf("| Date          : %-40s |\n", records[index].date);
    printf("| Category      : %-40s |\n", records[index].category);
    printf("+--------------------------------------------------------+\n");
}

// 7. EXPORT STOCK REPORT
void exportReport() {
    FILE *reportFile;
    char filename[100];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    
    // Generate filename with timestamp
    sprintf(filename, "apple_store_report_%04d%02d%02d_%02d%02d%02d.txt",
            tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
            tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    
    reportFile = fopen(filename, "w");
    
    if (reportFile == NULL) {
        printf("[ERROR] Cannot create report file!\n");
        return;
    }
    
    // Write report header
    fprintf(reportFile, "==============================================================\n");
    fprintf(reportFile, "            MY APPLE-STORE SYSTEM                             \n");
    fprintf(reportFile, "              STOCK REPORT                                    \n");
    fprintf(reportFile, "==============================================================\n\n");
    fprintf(reportFile, "Generated on: %s", asctime(tm_info));
    fprintf(reportFile, "Total Records: %d\n\n", recordCount);
    
    if (recordCount == 0) {
        fprintf(reportFile, "No records in stock.\n");
    } else {
        // Write table header
        fprintf(reportFile, "+------------+--------------------------------------+------------+------------+------------+\n");
        fprintf(reportFile, "| Product ID | Product Name                         | Price      | Date       | Category   |\n");
        fprintf(reportFile, "+------------+--------------------------------------+------------+------------+------------+\n");
        
        // Write all records
        for (int i = 0; i < recordCount; i++) {
            fprintf(reportFile, "| %-10d | %-36s | $%-9.2f | %-10s | %-10s |\n", 
                   records[i].id, records[i].productName, 
                   records[i].price, records[i].date, records[i].category);
        }
        
        fprintf(reportFile, "+------------+--------------------------------------+------------+------------+------------+\n");
        
        // Calculate totals by category
        float totalValue = 0;
        int iphoneCount = 0, macbookCount = 0, iwatchCount = 0;
        float iphoneValue = 0, macbookValue = 0, iwatchValue = 0;
        
        for (int i = 0; i < recordCount; i++) {
            totalValue += records[i].price;
            if (strcmp(records[i].category, "iPhones") == 0) {
                iphoneCount++;
                iphoneValue += records[i].price;
            } else if (strcmp(records[i].category, "MacBooks") == 0) {
                macbookCount++;
                macbookValue += records[i].price;
            } else if (strcmp(records[i].category, "iWatch") == 0) {
                iwatchCount++;
                iwatchValue += records[i].price;
            }
        }
        
        fprintf(reportFile, "\n--------------------------------------------------------------\n");
        fprintf(reportFile, "                    SUMMARY BY CATEGORY                       \n");
        fprintf(reportFile, "--------------------------------------------------------------\n");
        fprintf(reportFile, "iPhones  : %d items, Total Value: $%.2f\n", iphoneCount, iphoneValue);
        fprintf(reportFile, "MacBooks : %d items, Total Value: $%.2f\n", macbookCount, macbookValue);
        fprintf(reportFile, "iWatch    : %d items, Total Value: $%.2f\n", iwatchCount, iwatchValue);
        fprintf(reportFile, "--------------------------------------------------------------\n");
        fprintf(reportFile, "TOTAL    : %d items, Total Value: $%.2f\n", recordCount, totalValue);
    }
    
    fclose(reportFile);
    
    printf("\n[OK] Stock report exported successfully!\n");
    printf("   File: %s\n", filename);
}

