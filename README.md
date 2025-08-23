# Book Database in C

A simple book management system in C using **binary files** for storage.  
Supports **CRUD operations** (Create, Read, Update, Delete) and searching and sorting by all book attributes.

## Features

- Add new books with automatic ID assignment
- List all books
- Search books by ID, title, author, theme, read status, release year and your own custom filters
- Update book information
- Soft delete books (mark as inactive)
- Persistent storage using binary files (`books.dat` and `last_id.dat`)
- Import data from `.CSV` files and export to `.TXT`
- Sort your books by any attribute (`quicksort`)
- Database statistics *(future implementation)*
- Compression *(future implementation)*

## Book Structure

Each book contains the following attributes:

```c
typedef struct {
    unsigned short id;
    char title[100];
    char author[100];
    char theme[50];
    bool read;
    unsigned short releaseYear;
    bool active;
} Book;
```
## Structure
```
project/
│── main.c          # Main program with menu
│── mydatabase.c    # Functions for CRUD operations
│── mydatabase.h    # Function declarations and struct definitions
│── data/           # Folder to store .dat files
│   ├── books.dat
│   ├── last_id.dat
│   ├── backup_books.dat
│   ├── books_import.csv
│   └── books_export.txt

```