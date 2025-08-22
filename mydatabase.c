#include "mydatabase.h"

void clearInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

unsigned short getNextBookID(){
    FILE *f = fopen("last_id.dat", "r+b");

    if (!f) {
        printf("Error opening file.\n");
        return 0;
    }

    // If the file does not exist, create it
    if (fseek(f, 0, SEEK_END) == 0 && ftell(f) == 0) {
        // Initialize the file with the first ID
        unsigned short initialID = 0;
        fwrite(&initialID, sizeof(unsigned short), 1, f);
    }

    // Move the file pointer to the beginning
    rewind(f);  

    unsigned short id = 0;

    //Gets the last id used
    fread(&id, sizeof(unsigned short), 1, f);
    id++;
    

    //Back to the start of the file
    rewind(f);

    //Write new id
    fwrite(&id, sizeof(unsigned short), 1, f);
    fclose(f);

    return id;
}

void printBook(Book book){
    printf("\nBook id: %hu", book.id);
    printf("\nBook title: %s", book.title);
    printf("Author name: %s", book.author);
    printf("Book theme: %s", book.theme);
    printf("Release year: %hu", book.releaseYear);
    printf("\nBook was read: %s\n", book.read ? "Yes" : "No");
}


void createBook(){
    Book tmp;
    printf("Enter book title: ");
    fgets(tmp.title, MAX_TITLE_LENGTH, stdin);
    printf("Enter author name: ");
    fgets(tmp.author, MAX_AUTHOR_NAME_LENGTH, stdin);
    printf("Enter book theme: ");
    fgets(tmp.theme, MAX_THEME_LENGTH, stdin);
    printf("Enter release year: ");
    scanf("%hu", &tmp.releaseYear);
    clearInputBuffer(); // Clear the input buffer after scanf
    tmp.read = false;
    tmp.active = true;
    tmp.id = getNextBookID();

    FILE *f = fopen("books.dat", "ab");
    if (f){
        fwrite(&tmp, sizeof(Book), 1, f);
        //Book added successfully
        printf("Book added successfully with ID: %hu\n", tmp.id);
    } else {
        //Error opening file
        printf("Error opening file to add book.\n");
    }
    fclose(f);
}

void readBook(unsigned short id){
    FILE *f = fopen("books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(tmp.id == id && tmp.active){
            printBook(tmp);
            flag = true;
            break;
        }
    }
    
    if (!flag){
        printf("Book with ID %hu not found or is inactive.\n", id);
    }
    fclose(f);
}

void deleteBook(unsigned short id){
    FILE *f = fopen("books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(tmp.id == id && tmp.active){
            tmp.active = false;
            // Move the file pointer back to the start of the current record
            fseek(f, -sizeof(Book), SEEK_CUR);
            fwrite(&tmp, sizeof(Book), 1, f);
            printf("Book with ID %hu marked as inactive.\n", id);
            flag = true;
            break;
        }
    }
    
    if (!flag){
        printf("Book with ID %hu not found or is inactive.\n", id);
    }
    fclose(f);
} 

void updateBook(unsigned short id){
    FILE *f = fopen("books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(tmp.id == id && tmp.active){
            tmp.read = true;
            // Move the file pointer back to the start of the current record
            fseek(f, -sizeof(Book), SEEK_CUR);
            fwrite(&tmp, sizeof(Book), 1, f);
            printf("Book with ID %hu marked as read.\n", id);
            flag = true;
            break;
        }
    }
    
    if (!flag){
        printf("Book with ID %hu not found or is inactive.\n", id);
    }
    fclose(f);

}

void searchBookID(unsigned short id){
    readBook(id);
    // Has the same logic of the readBook
}

void searchBookTitle(char title[MAX_TITLE_LENGTH]){
    FILE *f = fopen("books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(strcmp(tmp.title, title) && tmp.active){
            printBook(tmp);
            flag = true;
            break;
        }
    }

    if (!flag){
        printf("Book with name: %s not found or is inactive.\n", title);
    }

    fclose(f);
}

void searchBookAuthor(char author[MAX_AUTHOR_NAME_LENGTH]){
    FILE *f = fopen("books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }
    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(strcmp(tmp.author, author) && tmp.active){
            printBook(tmp);
            flag = true;
            // Do not break here to find all matching books because we want to list all books by the same author
        }
    }

    if (!flag){
        printf("Book with author: %s not found or is inactive.\n", author);
    }

    fclose(f);
}

void searchBookTheme(char theme[MAX_THEME_LENGTH]){
    FILE *f = fopen("books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }
    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(strcmp(tmp.theme, theme) && tmp.active){
            printBook(tmp);
            flag = true;
            // Same break logic of the author
        }
    }
    if (!flag){
        printf("Book with theme: %s not found or is inactive.\n", theme);
    }

    fclose(f);

}

void searchBookRead(bool read){
    FILE *f = fopen("books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }
    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(tmp.read && tmp.active){
            printBook(tmp);
            flag = true;
            // Do not break here 
        }
    }

    if (!flag){
        printf("No Book read are found \n");
    }

    fclose(f);
}

void searchBookYear(unsigned short year){
    FILE *f = fopen("books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }
    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(tmp.releaseYear == year && tmp.active){
            printBook(tmp);
            flag = true;
            // Do not break here
        }
    }

    if (!flag){
        printf("Books with year: %hu not found or are inactive.\n", year);
    }

    fclose(f);
}

void searchBookActive(){
    FILE *f = fopen("books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }
    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(tmp.active){
            printBook(tmp);
            flag = true;
            // Do not break here
        }
    }

    if (!flag){
        printf("No Books are inactive.\n");
    }

    fclose(f);
}

void listBooks(){
    FILE *f = fopen("books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
            printBook(tmp);
    }

    //Print all books even if is inactive

    fclose(f);
}