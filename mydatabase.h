#ifndef MYDATABASE_H
#define MYDATABASE_H

#define MAX_AUTHOR_NAME_LENGTH 50
#define MAX_TITLE_LENGTH 80
#define MAX_THEME_LENGTH 30

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    unsigned short id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_NAME_LENGTH];
    char theme[MAX_THEME_LENGTH];
    bool read;
    unsigned short releaseYear;
    bool active;
} Book;

// Utility functions
void clearInputBuffer();
unsigned short getNextBookID();
void printBook(Book book);
void dataDirectoryCheck();
unsigned short getId();

//Basic CRUD;
void createBook();
void readBook(unsigned short id);
void updateBook(unsigned short id);
void deleteBook(unsigned short id);

//Search Books by their atributtes
void searchBookID(unsigned short id);
void searchBookTitle(char title[MAX_TITLE_LENGTH]);
void searchBookAuthor(char author[MAX_AUTHOR_NAME_LENGTH]);
void searchBookTheme(char theme[MAX_THEME_LENGTH]);
void searchBookRead(bool read);
void searchBookYear(unsigned short year);
void searchBookActive();

//List All books in the database
void listBooks();
void listBooksTxt();
void exportBooksToTxt();
void importBooksFromCsv();
void compressDatabase();
void decompressDatabase();

//Sort books by their attributes
void sortBooksByTitle();
void sortBooksByAuthor();
void sortBooksByYear();
void sortBooksByTheme();
void sortBooksByID();

int compareBooksByReleaseYear(const void *a, const void *b);
int compareBooksByTitle(const void *a, const void *b);
int compareBooksByAuthor(const void *a, const void *b);
int compareBooksByTheme(const void *a, const void *b);
int compareBooksByID(const void *a, const void *b);

//Additional functionalities
void markBookAsRead(unsigned short id);
void markBookAsUnread(unsigned short id);

// Count books in the database
void countBooks();
void countBooksByTheme(char theme[MAX_THEME_LENGTH]);
void countBooksByAuthor(char author[MAX_AUTHOR_NAME_LENGTH]);

// Backup and restore functionalities
void backupDatabase();
void restoreDatabase();

// Get oldest and newest book
void getOldestBook();
void getNewestBook();

//Database statistics
void getDatabaseStatistics();
long getDatabaseSizeInBytes();
void printDatabaseSize();

// Search books with multiple criteria
void searchBooksMultipleCriteria(const char *title, const char *author, const char *theme, unsigned short year, bool read, bool active);

#endif // MYDATABASE_H