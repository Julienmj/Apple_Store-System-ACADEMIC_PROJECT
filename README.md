# MY APPLE-STORE SYSTEM

A comprehensive C-based inventory management system for Apple products, split into two separate programs implementing different data structure operations and sorting algorithms.

## 📁 Project Structure

```
├── question1.c          # Question 1 Program (Record/Data Management System)
├── question2.c          # Question 2 Program (Multiple Sorting Algorithms)
├── question1_data.txt   # Data file for Question 1 (auto-generated)
├── question2_data.txt   # Data file for Question 2 (auto-generated)
├── apple_store_report_*.txt  # Exported stock reports (auto-generated)
├── FLOWCHARTS.md        # Detailed flowcharts documentation
└── README.md            # This file
```

---

## 📋 QUESTION 1: Record/Data Management System

### System Overview
Question 1 implements a complete record management system for Apple Store inventory with basic CRUD (Create, Read, Update, Delete) operations and sorting capabilities.

### Features
- ✅ Insert New Record (with category and product selection)
- ✅ Display All Records
- ✅ Search for a Record by ID
- ✅ Delete a Record by ID
- ✅ Sort Records (Highest -> Lowest by Price) using **Selection Sort**
- ✅ Update an Existing Record
- ✅ Export Stock Report
- ✅ Persistent file storage

### Compilation
```bash
gcc question1.c -o question1.exe
```

### Running
```bash
question1.exe
```

### Menu Options
1. Insert New Record
2. Display All Records
3. Search for a Record by ID
4. Delete a Record by ID
5. Sort Records (Highest -> Lowest by Price)
6. Update an Existing Record
7. Export Stock Report
8. Exit and Save

### Data Structure
```c
typedef struct {
    int id;                    // Unique product identifier
    char productName[50];      // Product name (from predefined list)
    int itemNumber;            // Item serial number
    float price;               // Product price in dollars
    char date[12];             // Date in DD/MM/YYYY format
    char category[20];         // Category: iPhones, MacBooks, or iWatch
} Record;
```

### Product Categories

#### 1. iPhones
- iPhone 15 Pro Max
- iPhone 15 Pro
- iPhone 15
- iPhone 14 Pro
- iPhone 14

#### 2. MacBooks
- MacBook Pro 16-inch M3
- MacBook Pro 14-inch M3
- MacBook Air 15-inch M2
- MacBook Air 13-inch M2
- MacBook Pro 13-inch M2

#### 3. iWatch
- Apple Watch Series 9
- Apple Watch Ultra 2
- Apple Watch SE
- Apple Watch Series 8
- Apple Watch Series 7

### Insert Record Process
1. Select category (iPhones, MacBooks, or iWatch)
2. Select specific product from submenu (1-5)
3. Enter quantity to add (1-5 items)
4. For each item:
   - Enter Product ID (unique)
   - Item Number is entered
   - Price is entered ($)
   - Date is entered (DD/MM/YYYY)
5. Product name is automatically set from selection

### Sorting Algorithm: Selection Sort
**Purpose**: Sort records by price in descending order (highest to lowest)

**Algorithm Explanation**:
1. Start from the first position (i = 0)
2. Find the record with maximum price in the remaining unsorted portion
3. Swap it with the current position
4. Move to next position and repeat
5. Continue until all records are sorted

**Time Complexity**: O(n²)
**Space Complexity**: O(1)

**Code Example**:
```c
for (int i = 0; i < recordCount - 1; i++) {
    maxIndex = i;
    // Find maximum price in remaining unsorted records
    for (int j = i + 1; j < recordCount; j++) {
        if (records[j].price > records[maxIndex].price) {
            maxIndex = j;
        }
    }
    // Swap if needed
    if (maxIndex != i) {
        temp = records[i];
        records[i] = records[maxIndex];
        records[maxIndex] = temp;
    }
}
```

---

## 📋 QUESTION 2: Multiple Sorting Algorithms

### System Overview
Question 2 implements the same Apple Store management system but focuses on demonstrating multiple sorting algorithms (Bubble Sort and Selection Sort) with re-sorting capabilities.

### Features
- ✅ Store Records (Minimum 10 Records) - Auto-initializes with 14 sample Apple products
- ✅ Display All Records
- ✅ Sort Records in Ascending Order using **Bubble Sort**
- ✅ Sort Records in Descending Order using **Selection Sort**
- ✅ Search for a Record by ID
- ✅ Allow Re-Sorting (switch between ascending/descending)
- ✅ Export Stock Report
- ✅ Persistent file storage

### Compilation
```bash
gcc question2.c -o question2.exe
```

### Running
```bash
question2.exe
```

### Menu Options
1. Insert New Record
2. Display All Records
3. Search for a Record by ID
4. Sort Records (Ascending - Bubble Sort)
5. Sort Records (Descending - Selection Sort)
6. Re-Sort Records
7. Export Stock Report
8. Exit and Save

### Data Structure
Same as Question 1:
```c
typedef struct {
    int id;
    char productName[50];
    int itemNumber;
    float price;
    char date[12];
    char category[20];
} Record;
```

### Sample Products (Auto-loaded)
The program automatically initializes with 14 sample Apple products if less than 10 records exist:
- iPhone 15 Pro Max, iPhone 15 Pro, iPhone 15, iPhone 14 Pro
- MacBook Pro 16-inch M3, MacBook Pro 14-inch M3, MacBook Air 15-inch M2, MacBook Air 13-inch M2, MacBook Pro 13-inch M2
- Apple Watch Series 9, Apple Watch Ultra 2, Apple Watch SE, Apple Watch Series 8

### Sorting Algorithms

#### 1. Bubble Sort (Ascending Order)
**Purpose**: Sort records by price in ascending order (lowest to highest)

**Algorithm Explanation**:
1. Compare adjacent elements (records[j] and records[j+1])
2. If current price is greater than next, swap them
3. After each pass, the largest element "bubbles" to the end
4. Repeat for remaining unsorted portion
5. Early exit optimization: if no swaps occur, array is sorted

**Time Complexity**: 
- Worst case: O(n²)
- Best case: O(n) (with optimization)
- Average case: O(n²)

**Space Complexity**: O(1)

**Code Example**:
```c
for (int i = 0; i < recordCount - 1; i++) {
    swapped = 0;
    for (int j = 0; j < recordCount - i - 1; j++) {
        if (records[j].price > records[j + 1].price) {
            // Swap records
            temp = records[j];
            records[j] = records[j + 1];
            records[j + 1] = temp;
            swapped = 1;
        }
    }
    if (!swapped) break;  // Early exit if sorted
}
```

#### 2. Selection Sort (Descending Order)
**Purpose**: Sort records by price in descending order (highest to lowest)

**Algorithm Explanation**:
1. Find the record with maximum price in unsorted portion
2. Swap it with the first position of unsorted portion
3. Move the boundary of sorted portion one position forward
4. Repeat until all records are sorted

**Time Complexity**: O(n²)
**Space Complexity**: O(1)

**Code Example**:
```c
for (int i = 0; i < recordCount - 1; i++) {
    maxIndex = i;
    for (int j = i + 1; j < recordCount; j++) {
        if (records[j].price > records[maxIndex].price) {
            maxIndex = j;
        }
    }
    if (maxIndex != i) {
        // Swap records
        temp = records[i];
        records[i] = records[maxIndex];
        records[maxIndex] = temp;
    }
}
```

### Re-Sort Feature
- If records are sorted in ascending order, re-sort switches to descending
- If records are sorted in descending order, re-sort switches to ascending
- If records are unsorted, prompts user to sort first

---

## 🔧 Compilation Instructions

### Windows (using GCC/MinGW/TDM-GCC)
```bash
# Compile Question 1
gcc question1.c -o question1.exe

# Compile Question 2
gcc question2.c -o question2.exe
```

### Linux/Mac
```bash
# Compile Question 1
gcc question1.c -o question1

# Compile Question 2
gcc question2.c -o question2

# Run
./question1
./question2
```

---

## 📊 Data Structures Explained

### Record Structure
The `Record` structure stores all information about a single Apple product:

- **id**: Unique identifier for each product record (integer)
- **productName**: Name of the Apple product (string, max 50 chars)
- **itemNumber**: Serial/item number for inventory tracking (integer)
- **price**: Product price in US dollars (float)
- **date**: Date when record was added (string, format: DD/MM/YYYY)
- **category**: Product category - "iPhones", "MacBooks", or "iWatch" (string)

### Array Data Structure
- **Type**: Static array of structures
- **Size**: Maximum 500 records (`MAX_RECORDS = 500`)
- **Access**: Direct access using index (O(1))
- **Search**: Linear search (O(n))
- **Insertion**: O(1) at the end, O(n) if shifting needed
- **Deletion**: O(n) due to shifting elements

---

## 🎯 Key Features Explained

### 1. Input Validation
- **ID Validation**: 
  - Must be positive integer
  - No duplicate IDs allowed
  - Checks against existing records
  
- **Price Validation**:
  - Must be positive number
  - Maximum value: $100,000
  - Float type for decimal prices

- **Date Validation**:
  - Must not be empty
  - Format: DD/MM/YYYY
  - String length validation

- **Menu Choice Validation**:
  - Must be valid number within range
  - Handles non-numeric input gracefully

### 2. File Operations

#### Save to File
- Format: `ID|ProductName|ItemNumber|Price|Date|Category`
- Uses pipe (|) delimiter for reliable parsing
- Auto-saves after every modification
- Error handling for file write failures

#### Load from File
- Parses pipe-delimited format
- Handles missing or corrupted files gracefully
- Validates each field during parsing
- Sets default values for missing data

### 3. Export Report Feature
- Generates timestamped report files
- Format: `apple_store_report_YYYYMMDD_HHMMSS.txt`
- Includes:
  - Complete inventory table
  - Summary by category
  - Total value calculations
  - Generation timestamp

---

## 🔍 Algorithm Explanations

### Selection Sort (Question 1 & Question 2 - Descending)

**How it works**:
1. **Initialization**: Start with the first element as the maximum
2. **Search**: Find the element with maximum value in unsorted portion
3. **Swap**: Exchange it with the first element of unsorted portion
4. **Repeat**: Move boundary and repeat for remaining elements

**Visual Example** (sorting by price):
```
Initial: [999, 1199, 799, 1299, 399]
Pass 1:   [1299, 1199, 799, 999, 399]  (1299 moved to front)
Pass 2:   [1299, 1199, 799, 999, 399]  (1199 already in place)
Pass 3:   [1299, 1199, 999, 799, 399]  (999 moved)
Pass 4:   [1299, 1199, 999, 799, 399]  (799 moved)
Final:    [1299, 1199, 999, 799, 399]  (Sorted!)
```

**Advantages**:
- Simple to understand and implement
- In-place sorting (no extra memory)
- Stable performance (always O(n²))

**Disadvantages**:
- Slow for large datasets
- Always requires n² comparisons

### Bubble Sort (Question 2 - Ascending)

**How it works**:
1. **Compare**: Compare adjacent elements
2. **Swap**: If out of order, swap them
3. **Bubble**: Largest element "bubbles" to the end after each pass
4. **Optimize**: Stop early if no swaps occurred (already sorted)

**Visual Example** (sorting by price):
```
Initial: [999, 1199, 799, 1299, 399]
Pass 1:  [999, 799, 1199, 399, 1299]  (1299 bubbled to end)
Pass 2:  [799, 999, 399, 1199, 1299]  (1199 bubbled)
Pass 3:  [799, 399, 999, 1199, 1299]  (999 bubbled)
Pass 4:  [399, 799, 999, 1199, 1299]  (799 bubbled)
Final:   [399, 799, 999, 1199, 1299]  (Sorted!)
```

**Advantages**:
- Very simple to understand
- Can detect if array is already sorted (optimization)
- Stable algorithm (maintains relative order)

**Disadvantages**:
- Very slow for large datasets
- Many unnecessary comparisons

---

## 💾 File Storage Format

### Question 1 & Question 2
- **File**: `question1_data.txt` / `question2_data.txt`
- **Format**: `ID|ProductName|ItemNumber|Price|Date|Category`
- **Example**: `1001|iPhone 15 Pro Max|15001|1199.00|15/01/2024|iPhones`

### Export Report Format
- **File**: `apple_store_report_YYYYMMDD_HHMMSS.txt`
- **Content**:
  - Header with system name and timestamp
  - Complete inventory table
  - Summary statistics by category
  - Total inventory value

---

## 📖 Usage Examples

### Question 1 Example Flow
1. Run the program: `question1.exe`
2. Select option 1 (Insert New Record)
3. Choose category: 1 (iPhones)
4. Select product: 1 (iPhone 15 Pro Max)
5. Enter quantity: 2
6. For item 1:
   - Product ID: 1001
   - Item Number: 15001
   - Price: 1199.00
   - Date: 15/01/2024
7. For item 2:
   - Product ID: 1002
   - Item Number: 15002
   - Price: 1199.00
   - Date: 15/01/2024
8. Select option 2 to display all records
9. Select option 5 to sort by price (highest to lowest)
10. Select option 7 to export stock report
11. Select option 8 to exit and save

### Question 2 Example Flow
1. Run the program: `question2.exe`
2. Program auto-loads 14 sample products (if < 10 records)
3. Select option 4 to sort in ascending order (Bubble Sort)
4. View sorted results (lowest to highest price)
5. Select option 5 to sort in descending order (Selection Sort)
6. View sorted results (highest to lowest price)
7. Select option 6 to re-sort (switches order)
8. Select option 7 to export stock report
9. Select option 8 to exit and save

---

## 🎨 Design Features

### User Interface
- **Clean Header**: "MY APPLE-STORE SYSTEM" as main title
- **Subtitle**: Program-specific subtitle (Record Management / Multiple Sorting Algorithms)
- **Menu System**: Numbered options with clear descriptions
- **Table Format**: Well-structured ASCII tables for data display
- **Status Indicators**: Clear feedback for all operations ([OK], [ERROR], [WARNING])

### Navigation
- **Back Button**: Press 'B' to go back to previous menu
- **Continue**: Press Enter to return to main menu
- **Input Cursor**: Appears immediately next to prompts (no newline before input)

### Error Handling
- Comprehensive input validation
- Clear error messages
- Graceful handling of file operations
- Duplicate ID detection
- Range validation for all numeric inputs

---

## 🔬 Code Structure Explanation

### Function Organization

#### Display Functions
- `displayHeader()`: Shows system title and subtitle
- `displayMenu()`: Displays main menu options
- `displayCategoryMenu()`: Shows category selection menu
- `displayProductSubmenu()`: Shows product selection for chosen category

#### Validation Functions
- `validateID()`: Checks ID is positive and unique
- `validatePrice()`: Validates price is within acceptable range

#### Core Operations
- `insertRecord()`: Adds new product records with category/product selection
- `displayAllRecords()`: Shows all records in table format
- `searchByID()`: Linear search for record by ID
- `deleteByID()`: Removes record and shifts array
- `updateRecord()`: Modifies existing record fields
- `sortRecords()`: Sorts by price (Question 1: Selection Sort descending)
- `sortAscending()`: Bubble Sort ascending (Question 2)
- `sortDescending()`: Selection Sort descending (Question 2)
- `exportReport()`: Generates timestamped stock report file

#### File Operations
- `loadFromFile()`: Reads records from text file
- `saveToFile()`: Writes records to text file

#### Utility Functions
- `clearInputBuffer()`: Clears stdin buffer
- `clearScreen()`: Clears console (platform-specific)
- `getCategoryChoice()`: Gets user category selection
- `getProductChoice()`: Gets user product selection

### Memory Management
- **Static Arrays**: Fixed-size arrays for predictable memory usage
- **No Dynamic Allocation**: Simple, easy-to-explain code
- **Stack-based**: All variables on stack for simplicity

---

## 📚 Data Structure Concepts Used

### 1. Structures (Struct)
- **Purpose**: Group related data together
- **Example**: Record structure contains all product information
- **Benefit**: Organized data, easy to pass as single unit

### 2. Arrays
- **Type**: Static array of structures
- **Size**: Fixed at compile time (500 records max)
- **Access**: Direct index access (O(1))
- **Operations**: Insert, Delete, Search, Sort

### 3. Linear Search
- **Used in**: Search by ID, Duplicate ID checking
- **Time Complexity**: O(n)
- **Method**: Iterate through array until found

### 4. Selection Sort
- **Used in**: Question 1 (descending), Question 2 (descending)
- **Time Complexity**: O(n²)
- **Method**: Find maximum, swap to front, repeat

### 5. Bubble Sort
- **Used in**: Question 2 (ascending)
- **Time Complexity**: O(n²) worst case, O(n) best case
- **Method**: Compare adjacent, swap if needed, bubble largest to end

---

## 🎓 Learning Points

### For Students

1. **Data Structures**:
   - Understanding structures (struct) in C
   - Working with arrays of structures
   - Static vs dynamic memory allocation

2. **Algorithms**:
   - Selection Sort implementation and analysis
   - Bubble Sort implementation and analysis
   - Comparison of sorting algorithms
   - Time complexity understanding

3. **File Handling**:
   - Reading from text files
   - Writing to text files
   - Parsing delimited data
   - Error handling in file operations

4. **Input/Output**:
   - Formatted output with printf
   - Input validation techniques
   - Buffer management
   - User-friendly interfaces

5. **Programming Practices**:
   - Modular code design
   - Function organization
   - Error handling
   - Code readability and comments

---

## 📝 Notes

- Both programs are independent and can be run separately
- Data files are created automatically on first run
- Maximum capacity: 500 records per program
- All changes are automatically saved to file
- Question 2 automatically ensures minimum 10 records
- Export reports are timestamped for easy tracking
- Product names are selected from predefined lists (no manual entry)
- Quantity limited to 1-5 items per insertion operation
- All prices stored as float for decimal support
- Date format: DD/MM/YYYY (e.g., 15/01/2024)

---

## ✅ Requirements Checklist

### Question 1
- [x] Overall System Flowchart
- [x] Insert New Record (with category/product selection)
- [x] Display All Records
- [x] Search for a Record by ID
- [x] Delete a Record by ID
- [x] Sort Records (Highest -> Lowest by Price) using Selection Sort
- [x] Update an Existing Record
- [x] Export Stock Report
- [x] Persistent file storage

### Question 2
- [x] Overall System Flowchart
- [x] Store Records (Minimum 10 Records)
- [x] Display All Records
- [x] Sort Records in Ascending Order (Bubble Sort)
- [x] Sort Records in Descending Order (Selection Sort)
- [x] Search for a Record by ID
- [x] Allow Re-Sorting
- [x] Export Stock Report
- [x] Persistent file storage

---

## 🔍 Flowcharts

Detailed flowcharts for all operations are available in `FLOWCHARTS.md`.

---

## 👨‍💻 Author

MY APPLE-STORE SYSTEM
Created for DSA Individual Assignment
Data Structures and Algorithms Course

---

## 📄 License

Educational Project - For Academic Use Only
