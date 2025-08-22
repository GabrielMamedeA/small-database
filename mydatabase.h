#ifndef MYDATABASE_H
#define MYDATABASE_H

#define MAX_AUTHOR_NAME_LENGTH 50
#define MAX_TITLE_LENGTH 80
#define MAX_THEME_LENGTH 30

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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


#endif // MYDATABASE_H