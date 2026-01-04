# Multi-Brand Dashboard - Product Stock Management System
## Flowcharts Documentation

This document contains flowcharts for both Question 1 and Question 2 programs.

---

## QUESTION 1 - Overall System Flowchart

```
                    START
                      |
            [Display Welcome Header]
                      |
            [Load Data from File]
                      |
            [Display System Status]
                      |
            ┌─────────────────────────┐
            │   DISPLAY MAIN MENU    │
            └─────────────────────────┘
                      |
            [Get User Choice (1-7)]
                      |
        ┌─────────────┴─────────────┐
        |                            |
    [Validate Input]        [Invalid Input]
        |                            |
      Valid                  [Show Error]
        |                            |
        |         ┌──────────────────┴──────────────────┐
        |         |                                     |
    [Choice 1]  [Choice 2]  [Choice 3]  [Choice 4]  [Choice 5]  [Choice 6]  [Choice 7]
        |         |         |         |         |         |         |
    [Insert]  [Display]  [Search]  [Delete]  [Sort ↓]  [Update]  [Exit]
        |         |         |         |         |         |         |
        |         |         |         |         |         |    [Save to File]
        |         |         |         |         |         |         |
        └─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
                      |
            [Press Enter to Continue]
                      |
            [Clear Screen]
                      |
            ┌─────────────────────────┐
            │   Return to Main Menu  │
            └─────────────────────────┘
                      |
            [Loop until Choice = 7]
                      |
                    END
```

---

## QUESTION 1 - Insert New Record Flowchart

```
                    START
                      |
            [Check if Max Records Reached]
                      |
            ┌─────────┴─────────┐
            |                   |
          YES                  NO
            |                   |
    [Display Error]    [Prompt for Product ID]
            |                   |
            |            [Validate ID Format]
            |                   |
            |            ┌───────┴───────┐
            |            |               |
            |        Invalid          Valid
            |            |               |
            |    [Display Error]  [Check if ID Exists]
            |            |               |
            |            |        ┌──────┴──────┐
            |            |        |             |
            |            |      Exists      Not Exists
            |            |        |             |
            |            |  [Display Error] [Prompt for Name]
            |            |        |             |
            |            |        |     [Validate Name]
            |            |        |             |
            |            |        |     [Prompt for Marks]
            |            |        |             |
            |            |        |     [Validate Marks]
            |            |        |             |
            |            |        |     [Add to Array]
            |            |        |             |
            |            |        |     [Increment Count]
            |            |        |             |
            |            |        |     [Save to File]
            |            |        |             |
            |            |        |     [Display Success]
            |            |        |             |
            └────────────┴────────┴─────────────┘
                      |
                    END
```

---

## QUESTION 1 - Display All Records Flowchart

```
                    START
                      |
            [Check if Record Count = 0]
                      |
            ┌─────────┴─────────┐
            |                   |
          YES                  NO
            |                   |
    [Display "No Records"]  [Display Table Header]
            |                   |
            |            [Loop: i = 0 to Count-1]
            |                   |
            |            [Display Record[i]]
            |                   |
            |            [Next Iteration]
            |                   |
            |            [Display Total Count]
            |                   |
            └───────────────────┘
                      |
                    END
```

---

## QUESTION 1 - Search for Record by ID Flowchart

```
                    START
                      |
            [Prompt for Product ID]
                      |
            [Validate ID Format]
                      |
            ┌─────────┴─────────┐
            |                   |
        Invalid              Valid
            |                   |
    [Display Error]    [Initialize Found = False]
            |                   |
            |            [Loop: i = 0 to Count-1]
            |                   |
            |            ┌───────┴───────┐
            |            |               |
            |    [ID Matches?]      [Continue]
            |            |               |
            |          YES              |
            |            |               |
            |    [Set Found = True]     |
            |    [Display Record]       |
            |    [Break Loop]           |
            |            |               |
            |            └───────┬───────┘
            |                    |
            |            [Check if Found]
            |                    |
            |            ┌───────┴───────┐
            |            |               |
            |          YES              NO
            |            |               |
            |    [Already Displayed] [Display "Not Found"]
            |            |               |
            └────────────┴───────────────┘
                      |
                    END
```

---

## QUESTION 1 - Delete Record by ID Flowchart

```
                    START
                      |
            [Check if Record Count = 0]
                      |
            ┌─────────┴─────────┐
            |                   |
          YES                  NO
            |                   |
    [Display Error]    [Prompt for Product ID]
            |                   |
            |            [Validate ID Format]
            |                   |
            |            [Search for Record]
            |                   |
            |            ┌───────┴───────┐
            |            |               |
            |        Found            Not Found
            |            |               |
            |    [Display Record]  [Display "Not Found"]
            |    [Save Index]            |
            |            |               |
            |    [Shift Elements]        |
            |    [After Index]           |
            |            |               |
            |    [Decrement Count]       |
            |            |               |
            |    [Save to File]          |
            |            |               |
            |    [Display Success]       |
            |            |               |
            └────────────┴───────────────┘
                      |
                    END
```

---

## QUESTION 1 - Sort Records (Highest → Lowest) Flowchart

```
                    START
                      |
            [Check if Record Count = 0]
                      |
            ┌─────────┴─────────┐
            |                   |
          YES                  NO
            |                   |
    [Display Error]    [Selection Sort Algorithm]
            |                   |
            |            [Outer Loop: i = 0 to Count-2]
            |                   |
            |            [Set MaxIndex = i]
            |                   |
            |            [Inner Loop: j = i+1 to Count-1]
            |                   |
            |            ┌───────┴───────┐
            |            |               |
            |    [Value[j] > Value[MaxIndex]?]
            |            |               |
            |          YES              NO
            |            |               |
            |    [Set MaxIndex = j]  [Continue]
            |            |               |
            |            └───────┬───────┘
            |                    |
            |            [Check if MaxIndex != i]
            |                    |
            |            ┌───────┴───────┐
            |            |               |
            |          YES              NO
            |            |               |
            |    [Swap Records]    [Continue]
            |            |               |
            |            └───────┬───────┘
            |                    |
            |            [Next Outer Iteration]
            |                    |
            |            [Save to File]
            |                    |
            |            [Display Sorted Records]
            |                    |
            └────────────────────┘
                      |
                    END
```

---

## QUESTION 1 - Update Record Flowchart

```
                    START
                      |
            [Prompt for Product ID]
                      |
            [Search for Record]
                      |
            ┌─────────┴─────────┐
            |                   |
        Found            [Display "Not Found"]
            |                   |
    [Display Current Record]     |
            |                   |
    [Display Update Options]     |
            |                   |
    [Get User Choice (1-3)]      |
            |                   |
    ┌───────┴───────┐           |
    |               |           |
[Choice 1]    [Choice 2]  [Choice 3]
    |               |           |
[Update Name] [Update Marks] [Update Both]
    |               |           |
    └───────┬───────┘           |
            |                   |
    [Validate Input]            |
            |                   |
    [Update Record]             |
            |                   |
    [Save to File]              |
            |                   |
    [Display Success]           |
            |                   |
            └───────────────────┘
                      |
                    END
```

---

## QUESTION 2 - Overall System Flowchart

```
                    START
                      |
            [Display Welcome Header]
                      |
            [Load Data from File]
                      |
            [Check Record Count]
                      |
            ┌─────────┴─────────┐
            |                   |
    [Count < 10]          [Count >= 10]
            |                   |
    [Initialize Sample]         |
    [Data (14 records)]        |
            |                   |
            └─────────┬─────────┘
                      |
            [Display System Status]
                      |
            ┌─────────────────────────┐
            │   DISPLAY MAIN MENU    │
            │   (Show Sort Status)   │
            └─────────────────────────┘
                      |
            [Get User Choice (1-7)]
                      |
        ┌─────────────┴─────────────┐
        |                            |
    [Validate Input]        [Invalid Input]
        |                            |
      Valid                  [Show Error]
        |                            |
        |         ┌──────────────────┴──────────────────┐
        |         |                                     |
    [Choice 1]  [Choice 2]  [Choice 3]  [Choice 4]  [Choice 5]  [Choice 6]  [Choice 7]
        |         |         |         |         |         |         |
    [Insert]  [Display]  [Search]  [Sort ↑]  [Sort ↓]  [Re-Sort]  [Exit]
        |         |         |         |         |         |         |
        |         |         |         |         |         |    [Save to File]
        |         |         |         |         |         |         |
        |         |         |         |         |         |    [Mark Unsorted]
        |         |         |         |         |         |         |
        └─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
                      |
            [Press Enter to Continue]
                      |
            [Clear Screen]
                      |
            ┌─────────────────────────┐
            │   Return to Main Menu  │
            │   (Allow Re-Sorting)   │
            └─────────────────────────┘
                      |
            [Loop until Choice = 7]
                      |
                    END
```

---

## QUESTION 2 - Sort Records Ascending (Bubble Sort) Flowchart

```
                    START
                      |
            [Check if Record Count = 0]
                      |
            ┌─────────┴─────────┐
            |                   |
          YES                  NO
            |                   |
    [Display Error]    [Bubble Sort Algorithm]
            |                   |
            |            [Outer Loop: i = 0 to Count-2]
            |                   |
            |            [Set Swapped = False]
            |                   |
            |            [Inner Loop: j = 0 to Count-i-2]
            |                   |
            |            ┌───────┴───────┐
            |            |               |
            |    [Value[j] > Value[j+1]?]
            |            |               |
            |          YES              NO
            |            |               |
            |    [Swap Records]    [Continue]
            |    [Set Swapped = True]    |
            |            |               |
            |            └───────┬───────┘
            |                    |
            |            [Check if Swapped = False]
            |                    |
            |            ┌───────┴───────┐
            |            |               |
            |          YES              NO
            |            |               |
            |    [Break Loop]    [Continue]
            |    (Optimization)          |
            |            |               |
            |            └───────┬───────┘
            |                    |
            |            [Next Outer Iteration]
            |                    |
            |            [Set isSorted = 1]
            |            [Save to File]
            |            [Display Sorted Records]
            |                    |
            └────────────────────┘
                      |
                    END
```

---

## QUESTION 2 - Sort Records Descending (Selection Sort) Flowchart

```
                    START
                      |
            [Check if Record Count = 0]
                      |
            ┌─────────┴─────────┐
            |                   |
          YES                  NO
            |                   |
    [Display Error]    [Selection Sort Algorithm]
            |                   |
            |            [Outer Loop: i = 0 to Count-2]
            |                   |
            |            [Set MaxIndex = i]
            |                   |
            |            [Inner Loop: j = i+1 to Count-1]
            |                   |
            |            ┌───────┴───────┐
            |            |               |
            |    [Value[j] > Value[MaxIndex]?]
            |            |               |
            |          YES              NO
            |            |               |
            |    [Set MaxIndex = j]  [Continue]
            |            |               |
            |            └───────┬───────┘
            |                    |
            |            [Check if MaxIndex != i]
            |                    |
            |            ┌───────┴───────┐
            |            |               |
            |          YES              NO
            |            |               |
            |    [Swap Records]    [Continue]
            |            |               |
            |            └───────┬───────┘
            |                    |
            |            [Next Outer Iteration]
            |                    |
            |            [Set isSorted = 2]
            |            [Save to File]
            |            [Display Sorted Records]
            |                    |
            └────────────────────┘
                      |
                    END
```

---

## QUESTION 2 - Re-Sort Records Flowchart

```
                    START
                      |
            [Check Sort Status]
                      |
            ┌─────────┴─────────┐
            |                   |
    [isSorted = 1]      [isSorted = 2]      [isSorted = 0]
    (Ascending)       (Descending)        (Unsorted)
            |                   |               |
    [Call Sort Descending] [Call Sort Ascending] [Display Error]
            |                   |               |
            |                   |        "Please sort first"
            |                   |               |
            └─────────┬─────────┘               |
                      |                        |
            [Display Re-Sorted Records]        |
                      |                        |
                      └────────────────────────┘
                                |
                              END
```

---

## File I/O Flowchart (Both Programs)

```
                    START
                      |
            ┌─────────┴─────────┐
            |                   |
    [LOAD FROM FILE]    [SAVE TO FILE]
            |                   |
    [Open File in Read Mode]  [Open File in Write Mode]
            |                   |
    ┌───────┴───────┐   [Loop through All Records]
    |               |           |
  Success        Failure    [Write Each Record]
    |               |           |
[Read Each Line] [Set Count=0] [Close File]
    |               |           |
[Parse Data]       |           |
    |               |           |
[Add to Array]     |           |
    |               |           |
[Increment Count]  |           |
    |               |           |
[Close File]       |           |
    |               |           |
    └───────┬───────┘           |
            |                   |
            └─────────┬─────────┘
                      |
                    END
```

---

## Notes

### Question 1
- Uses **Selection Sort** for sorting (Highest → Lowest)
- All operations include comprehensive input validation
- Simple, easy-to-explain code structure

### Question 2
- Uses **Bubble Sort** for ascending order (Lowest → Highest)
- Uses **Selection Sort** for descending order (Highest → Lowest)
- Tracks sort status to enable re-sorting
- Automatically initializes with 14 sample records if less than 10 exist
- Re-sorting feature allows switching between ascending and descending

### Common Features
- Both programs use persistent file storage
- All operations automatically save to respective files
- Comprehensive input validation on all user inputs
- User-friendly menu interface with clear formatting
- Error handling for edge cases
