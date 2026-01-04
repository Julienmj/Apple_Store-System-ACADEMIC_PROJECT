

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>


#define MAX_RECORDS 500
#define MAX_NAME_LENGTH 50
#define MAX_DATE_LENGTH 12
#define FILENAME "question2_data.txt"
#define MIN_RECORDS 10

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
// Track last sort performed: field (1=Price,2=ID), algorithm (1=Bubble,2=Selection), order (1=Ascending,0=Descending)
int lastSortField = 0;
int lastSortAlgorithm = 0;
int lastSortOrder = 1; // default ascending

// Apple Product Lists (5 categories, 2 products each - no inches)
const char* iphones[] = {
    "iPhone 15 Pro",
    "iPhone 15"
};

const char* macbooks[] = {
    "MacBook Pro M3",
    "MacBook Air M2"
};

const char* ipads[] = {
    "iPad Pro M2",
    "iPad Air M1"
};

const char* airpods[] = {
    "AirPods Pro 2",
    "AirPods 3"
};

const char* iwatch[] = {
    "Watch Series 9",
    "Watch SE"
};

// Function prototypes
void maximizeConsoleWindow();
void displayHeader();
void displayMenu();
void clearInputBuffer();
int getConsoleWidth();
void printCentered(const char* s);
void printCenteredNoNL(const char* s);
int confirm(const char* prompt);
int validateID(int id);
int validatePrice(float price);
int validateDate(char* date);
void initializeSampleData();
void insertRecord();
void displayAllRecords();
void searchByID();
void sortMenu();
void sortByPriceBubble(int ascending);
void sortByPriceSelection(int ascending);
void sortByIDBubble(int ascending);
void sortByIDSelection(int ascending);
void resortLastSort();
void exportReport();
void loadFromFile();
void saveToFile();
void clearScreen();
void displayCategoryMenu();
int getCategoryChoice();
void displayProductSubmenu(int category);
int getProductChoice(int category);
void createCategoryInteractive();
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
    
    // Initialize with sample data if less than minimum required
    if (recordCount < MIN_RECORDS) {
        initializeSampleData();
        saveToFile();
    }
    
    // Main program loop
    while(1) {
        displayMenu();
        printCenteredNoNL(">>> Enter your choice (1-6): ");
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
                searchByID();
                break;
            case 4:
                sortMenu();
                break;
            case 5:
                exportReport();
                break;
            case 6:
                if (!confirm("Are you sure you want to exit and save?")) {
                    printf("\n[INFO] Exit cancelled. Returning to main menu.\n");
                    break;
                }
                printf("\nSaving all data...\n");
                saveToFile();
                printf("[OK] Data saved successfully!\n");
                printf("\nThank you for using MY APPLE-STORE SYSTEM!\n");
                printf("   Program terminated.\n\n");
                return 0;
            default:
                printf("\n[ERROR] Invalid choice! Please select 1-6.\n\n");
        }
        
        // Pause before showing menu again
        if (choice != 6) {
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
    printCentered("*         Multiple Sorting Algorithms                      *");
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
    printCentered("*   3. Search by ID                                        *");
    printCentered("*   4. Sort Records                                        *");
    printCentered("*   5. Export Report                                       *");
    printCentered("*   6. Exit and Save                                       *");
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

// Simple confirmation helper: returns 1 for yes, 0 for no
int confirm(const char* prompt) {
    char input[10];
    printf("%s (Y/N): ", prompt);
    fflush(stdout);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    return (input[0] == 'Y' || input[0] == 'y');
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
    printf("  3. iPads                                                    \n");
    printf("  4. AirPods                                                  \n");
    printf("  5. iWatch                                                   \n");
    // Custom categories
    for (int i = 0; i < customCategoryCount; i++) {
        printf("  %d. %s\n", 6 + i, customCategories[i]);
    }
    printf("  C. Create New Category                                     \n");
    printf("  B. Back to Main Menu                                       \n");
    printf("--------------------------------------------------------------\n");
}

int getCategoryChoice() {
    char input[10];
    int choice;
    
    while (1) {
        printf("Select category (1-%d, C to create, B to go back): ", 5 + customCategoryCount);
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
            return -2; // Create new category
        }

        // Try to parse as number
        if (sscanf(input, "%d", &choice) == 1) {
            int maxChoice = 5 + customCategoryCount; // 5 default + custom
            if (choice >= 1 && choice <= maxChoice) return choice;
        }
        
        printf("[ERROR] Invalid choice! Please try again.\n");
    }
}

void displayProductSubmenu(int category) {
    printf("\n--------------------------------------------------------------\n");
    printf("                    SELECT PRODUCT                            \n");
    printf("--------------------------------------------------------------\n");
    
    if (category == 1) {  // iPhones
        for (int i = 0; i < 2; i++) {
            printf("  %d. %s\n", i + 1, iphones[i]);
        }
    } else if (category == 2) {  // MacBooks
        for (int i = 0; i < 2; i++) {
            printf("  %d. %s\n", i + 1, macbooks[i]);
        }
    } else if (category == 3) {  // iPads
        for (int i = 0; i < 2; i++) {
            printf("  %d. %s\n", i + 1, ipads[i]);
        }
    } else if (category == 4) {  // AirPods
        for (int i = 0; i < 2; i++) {
            printf("  %d. %s\n", i + 1, airpods[i]);
        }
    } else if (category == 5) {  // iWatch
        for (int i = 0; i < 2; i++) {
            printf("  %d. %s\n", i + 1, iwatch[i]);
        }
    } else {
        int customIndex = category - 6; // 0-based for question2 (default 5 + 1 offset)
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
        int maxProducts = 2;
        if (category > 5) {
            int ci = category - 6;
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
  // Assume unsorted after loading
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
// INITIALIZE SAMPLE DATA (Minimum 10 Records)
// ============================================================================
void initializeSampleData() {
    // Sample Apple products - 2 samples per category (10 total records)
    // IDs: iPhones 1001+, MacBooks 2001+, iPads 3001+, AirPods 4001+, iWatch 5001+
    Record sampleData[] = {
        // iPhones (2 products)
        {1001, "iPhone 15 Pro", 999.00, "15/01/2024", "iPhones"},
        {1002, "iPhone 15", 799.00, "15/01/2024", "iPhones"},
        // MacBooks (2 products)
        {2001, "MacBook Pro M3", 1999.00, "20/01/2024", "MacBooks"},
        {2002, "MacBook Air M2", 1299.00, "20/01/2024", "MacBooks"},
        // iPads (2 products)
        {3001, "iPad Pro M2", 1099.00, "25/01/2024", "iPads"},
        {3002, "iPad Air M1", 599.00, "25/01/2024", "iPads"},
        // AirPods (2 products)
        {4001, "AirPods Pro 2", 249.00, "30/01/2024", "AirPods"},
        {4002, "AirPods 3", 179.00, "30/01/2024", "AirPods"},
        // iWatch (2 products)
        {5001, "Watch Series 9", 399.00, "05/02/2024", "iWatch"},
        {5002, "Watch SE", 249.00, "05/02/2024", "iWatch"}
    };
    
    int sampleCount = sizeof(sampleData) / sizeof(sampleData[0]);
    
    // Add sample data to records array
    for (int i = 0; i < sampleCount && recordCount < MAX_RECORDS; i++) {
        records[recordCount++] = sampleData[i];
    }
    
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
        baseID = 3000;  // iPads: 3001, 3002, 3003...
    } else if (categoryChoice == 4) {
        baseID = 4000;  // AirPods: 4001, 4002, 4003...
    } else if (categoryChoice == 5) {
        baseID = 5000;  // iWatch: 5001, 5002, 5003...
    } else if (categoryChoice >= 6) {
        int customIndex = categoryChoice - 6; // custom categories start at choice 6
        if (customIndex >= 0 && customIndex < customCategoryCount) {
            baseID = 6000 + customIndex * 1000; // reserve 1000 IDs per custom category
        } else {
            return -1; // Invalid custom category
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
            strcpy(newRecord.productName, ipads[productChoice - 1]);
            strcpy(newRecord.category, "iPads");
        } else if (categoryChoice == 4) {
            strcpy(newRecord.productName, airpods[productChoice - 1]);
            strcpy(newRecord.category, "AirPods");
        } else if (categoryChoice == 5) {
            strcpy(newRecord.productName, iwatch[productChoice - 1]);
            strcpy(newRecord.category, "iWatch");
        } else {
            int customIndex = categoryChoice - 6;
            if (customIndex >= 0 && customIndex < customCategoryCount) {
                if (customProductCount[customIndex] > 0) {
                    strcpy(newRecord.productName, customProducts[customIndex][productChoice - 1]);
                } else {
                    char pname[MAX_NAME_LENGTH];
                    printf("Enter Product Name: ");
                    fflush(stdout);
                    if (fgets(pname, sizeof(pname), stdin) == NULL) pname[0] = '\0';
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
        printf("   [WARNING] Note: Records are now unsorted. Use sort option to organize.\n");
        
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

// 3. SEARCH FOR A RECORD BY ID
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

// 4. SORT MENU - Submenu for sorting options
void sortMenu() {
    int choice;
    int subchoice;
    int ascending;

    if (recordCount == 0) {
        printf("\n[ERROR] No records to sort!\n");
        return;
    }

    while (1) {
        printf("\n==============================================================\n");
        printf("                    SORT MENU                                 \n");
        printf("--------------------------------------------------------------\n");
        printf("  1. Ascending                                              \n");
        printf("  2. Descending                                             \n");
        printf("  3. Resort (reapply last sort)                             \n");
        printf("  4. Back to Main Menu                                      \n");
        printf("==============================================================\n");
        printf("Enter your choice (1-4): ");
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

        if (choice == 1 || choice == 2) {
            ascending = (choice == 1) ? 1 : 0;

            while (1) {
                printf("\n--------------------------------------------------------------\n");
                printf("                 %s SORT OPTIONS                         \n", ascending ? "ASCENDING" : "DESCENDING");
                printf("--------------------------------------------------------------\n");
                printf("  1. Sort by Price (Bubble Sort)                              \n");
                printf("  2. Sort by Price (Selection Sort)                           \n");
                printf("  3. Sort by ID (Bubble Sort)                                 \n");
                printf("  4. Sort by ID (Selection Sort)                              \n");
                printf("  5. Back to Sort Menu                                         \n");
                printf("--------------------------------------------------------------\n");
                printf("Enter your choice (1-5): ");
                fflush(stdout);

                if (scanf("%d", &subchoice) != 1) {
                    clearInputBuffer();
                    printf("[ERROR] Invalid input! Please enter a number.\n");
                    printf("Press Enter to continue...");
                    fflush(stdout);
                    getchar();
                    continue;
                }

                clearInputBuffer();

                switch (subchoice) {
                    case 1:
                        // Price Bubble
                        lastSortField = 1; // Price
                        lastSortAlgorithm = 1; // Bubble
                        lastSortOrder = ascending;
                        sortByPriceBubble(ascending);
                        break;
                    case 2:
                        // Price Selection
                        lastSortField = 1; // Price
                        lastSortAlgorithm = 2; // Selection
                        lastSortOrder = ascending;
                        sortByPriceSelection(ascending);
                        break;
                    case 3:
                        // ID Bubble
                        lastSortField = 2; // ID
                        lastSortAlgorithm = 1; // Bubble
                        lastSortOrder = ascending;
                        sortByIDBubble(ascending);
                        break;
                    case 4:
                        // ID Selection
                        lastSortField = 2; // ID
                        lastSortAlgorithm = 2; // Selection
                        lastSortOrder = ascending;
                        sortByIDSelection(ascending);
                        break;
                    case 5:
                        goto back_to_sort_menu;
                    default:
                        printf("[ERROR] Invalid choice! Please select 1-5.\n");
                        printf("Press Enter to continue...");
                        fflush(stdout);
                        getchar();
                        continue;
                }

                // after doing a sort, return to main sort menu
                return;
            }
        } else if (choice == 3) {
            // Resort (reapply last sort)
            resortLastSort();
            return;
        } else if (choice == 4) {
            return; // back to main menu
        } else {
            printf("[ERROR] Invalid choice! Please select 1-4.\n");
        }
        
        back_to_sort_menu: ;
    }
}

// SORT BY PRICE (BUBBLE SORT)
void sortByPriceBubble(int ascending) {
    if (recordCount == 0) {
        printf("\n[ERROR] No records to sort!\n");
        return;
    }
    printf("\n==============================================================\n");
    printf("              SORT BY PRICE (Bubble Sort)                      \n");
    printf("==============================================================\n\n");

    // Bubble Sort Algorithm (respecting ascending flag)
    Record temp;
    int swapped;

    for (int i = 0; i < recordCount - 1; i++) {
        swapped = 0;
        for (int j = 0; j < recordCount - i - 1; j++) {
            int cmp = (records[j].price > records[j + 1].price);
            if (!ascending) cmp = !cmp; // if descending, reverse comparison
            if (cmp) {
                temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break;  // Early exit if sorted
    }

    saveToFile();

    printf("[OK] Records sorted successfully!\n");
    printf("   Algorithm: Bubble Sort\n");
    printf("   Sort By: Price\n");
    printf("   Order: %s\n\n", ascending ? "Ascending" : "Descending");

    displayAllRecords();
}

// 5. SORT BY PRICE (SELECTION SORT)
void sortByPriceSelection(int ascending) {
    if (recordCount == 0) {
        printf("\n[ERROR] No records to sort!\n");
        return;
    }
    printf("\n==============================================================\n");
    printf("            SORT BY PRICE (Selection Sort)                     \n");
    printf("==============================================================\n\n");

    // Selection Sort Algorithm (respecting ascending flag)
    Record temp;
    int selectedIndex;

    for (int i = 0; i < recordCount - 1; i++) {
        selectedIndex = i;
        for (int j = i + 1; j < recordCount; j++) {
            if (ascending) {
                if (records[j].price < records[selectedIndex].price) {
                    selectedIndex = j;
                }
            } else {
                if (records[j].price > records[selectedIndex].price) {
                    selectedIndex = j;
                }
            }
        }
        if (selectedIndex != i) {
            temp = records[i];
            records[i] = records[selectedIndex];
            records[selectedIndex] = temp;
        }
    }

    saveToFile();

    printf("[OK] Records sorted successfully!\n");
    printf("   Algorithm: Selection Sort\n");
    printf("   Sort By: Price\n");
    printf("   Order: %s\n\n", ascending ? "Ascending" : "Descending");

    displayAllRecords();
}

// 6. SORT BY ID (BUBBLE SORT)
void sortByIDBubble(int ascending) {
    if (recordCount == 0) {
        printf("\n[ERROR] No records to sort!\n");
        return;
    }
    printf("\n==============================================================\n");
    printf("              SORT BY ID (Bubble Sort)                          \n");
    printf("==============================================================\n\n");

    // Bubble Sort Algorithm (respecting ascending flag)
    Record temp;
    int swapped;

    for (int i = 0; i < recordCount - 1; i++) {
        swapped = 0;
        for (int j = 0; j < recordCount - i - 1; j++) {
            int cmp = (records[j].id > records[j + 1].id);
            if (!ascending) cmp = !cmp;
            if (cmp) {
                temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break;  // Early exit if sorted
    }

    saveToFile();

    printf("[OK] Records sorted successfully!\n");
    printf("   Algorithm: Bubble Sort\n");
    printf("   Sort By: ID\n");
    printf("   Order: %s\n\n", ascending ? "Ascending" : "Descending");

    displayAllRecords();
}

// 7. SORT BY ID (SELECTION SORT)
void sortByIDSelection(int ascending) {
    if (recordCount == 0) {
        printf("\n[ERROR] No records to sort!\n");
        return;
    }
    printf("\n==============================================================\n");
    printf("            SORT BY ID (Selection Sort)                         \n");
    printf("==============================================================\n\n");

    // Selection Sort Algorithm (respecting ascending flag)
    Record temp;
    int selectedIndex;

    for (int i = 0; i < recordCount - 1; i++) {
        selectedIndex = i;
        for (int j = i + 1; j < recordCount; j++) {
            if (ascending) {
                if (records[j].id < records[selectedIndex].id) {
                    selectedIndex = j;
                }
            } else {
                if (records[j].id > records[selectedIndex].id) {
                    selectedIndex = j;
                }
            }
        }
        if (selectedIndex != i) {
            temp = records[i];
            records[i] = records[selectedIndex];
            records[selectedIndex] = temp;
        }
    }

    saveToFile();

    printf("[OK] Records sorted successfully!\n");
    printf("   Algorithm: Selection Sort\n");
    printf("   Sort By: ID\n");
    printf("   Order: %s\n\n", ascending ? "Ascending" : "Descending");

    displayAllRecords();
}

// Reapply last sort performed (if any)
void resortLastSort() {
    if (lastSortField == 0) {
        printf("\n[ERROR] No previous sort to reapply!\n");
        return;
    }

    printf("\n[OK] Reapplying last sort...\n");
    if (lastSortField == 1) { // Price
        if (lastSortAlgorithm == 1) {
            sortByPriceBubble(lastSortOrder);
        } else {
            sortByPriceSelection(lastSortOrder);
        }
    } else if (lastSortField == 2) { // ID
        if (lastSortAlgorithm == 1) {
            sortByIDBubble(lastSortOrder);
        } else {
            sortByIDSelection(lastSortOrder);
        }
    } else {
        printf("[ERROR] Unknown last sort parameters.\n");
    }
}

// 8. EXPORT STOCK REPORT
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
        int iphoneCount = 0, macbookCount = 0, ipadCount = 0, airpodCount = 0, iwatchCount = 0;
        float iphoneValue = 0, macbookValue = 0, ipadValue = 0, airpodValue = 0, iwatchValue = 0;
        
        for (int i = 0; i < recordCount; i++) {
            totalValue += records[i].price;
            if (strcmp(records[i].category, "iPhones") == 0) {
                iphoneCount++;
                iphoneValue += records[i].price;
            } else if (strcmp(records[i].category, "MacBooks") == 0) {
                macbookCount++;
                macbookValue += records[i].price;
            } else if (strcmp(records[i].category, "iPads") == 0) {
                ipadCount++;
                ipadValue += records[i].price;
            } else if (strcmp(records[i].category, "AirPods") == 0) {
                airpodCount++;
                airpodValue += records[i].price;
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
        fprintf(reportFile, "iPads    : %d items, Total Value: $%.2f\n", ipadCount, ipadValue);
        fprintf(reportFile, "AirPods  : %d items, Total Value: $%.2f\n", airpodCount, airpodValue);
        fprintf(reportFile, "iWatch   : %d items, Total Value: $%.2f\n", iwatchCount, iwatchValue);
        fprintf(reportFile, "--------------------------------------------------------------\n");
        fprintf(reportFile, "TOTAL    : %d items, Total Value: $%.2f\n", recordCount, totalValue);
    }
    
    fclose(reportFile);
    
    printf("\n[OK] Stock report exported successfully!\n");
    printf("   File: %s\n", filename);
}
