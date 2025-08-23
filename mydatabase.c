#include "mydatabase.h"

void clearInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

unsigned short getId(){
    FILE *f = fopen("data/last_id.dat", "r+b");
    if (!f) {
        printf("Error opening file.\n");
        return 0;
    }

    unsigned short id = 0;
    fread(&id, sizeof(unsigned short), 1, f);
    fclose(f);
    return id;
}

unsigned short getNextBookID(){
    FILE *f = fopen("data/last_id.dat", "r+b");

    if (!f) {
        // Try to create the file if it does not exist
        f = fopen("data/last_id.dat", "w+b");
        if (!f) {
            printf("Error creating file.\n");
            return 0;
        }
        // Initialize the file with the first ID
        unsigned short initialID = 0;
        fwrite(&initialID, sizeof(unsigned short), 1, f);
        fflush(f);
        rewind(f);
    }

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
    printf("\nAuthor name: %s", book.author);
    printf("\nBook theme: %s", book.theme);
    printf("\nRelease year: %hu", book.releaseYear);
    printf("\nBook was read: %s\n", book.read ? "Yes" : "No");
}


void createBook(){
    Book tmp;
    printf("Enter book title: ");
    fgets(tmp.title, MAX_TITLE_LENGTH, stdin);
    tmp.title[strcspn(tmp.title, "\n")] = 0;
    printf("Enter author name: ");
    fgets(tmp.author, MAX_AUTHOR_NAME_LENGTH, stdin);
    tmp.author[strcspn(tmp.author, "\n")] = 0;
    printf("Enter book theme: ");
    fgets(tmp.theme, MAX_THEME_LENGTH, stdin);
    tmp.theme[strcspn(tmp.theme, "\n")] = 0;
    printf("Enter release year: ");
    scanf("%hu", &tmp.releaseYear);
    clearInputBuffer(); // Clear the input buffer after scanf
    tmp.read = false;
    tmp.active = true;
    tmp.id = getNextBookID();

    FILE *f = fopen("data/books.dat", "ab");
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
    FILE *f = fopen("data/books.dat", "rb");
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
    FILE *f = fopen("data/books.dat", "rb");
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
    FILE *f = fopen("data/books.dat", "rb");
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
    FILE *f = fopen("data/books.dat", "rb");
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
    FILE *f = fopen("data/books.dat", "rb");
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
    FILE *f = fopen("data/books.dat", "rb");
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
    FILE *f = fopen("data/books.dat", "rb");
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
    FILE *f = fopen("data/books.dat", "rb");
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
    FILE *f = fopen("data/books.dat", "rb");
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
    FILE *f = fopen("data/books.dat", "rb");
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

void listBooksTxt(){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return; 
    } 
    Book tmp = {0};

    char archivePath[150];
    char archiveName[100];

    printf("Enter the name of the file to export to (without extension): ");
    fgets(archiveName, sizeof(archiveName), stdin);

    archiveName[strcspn(archiveName, "\n")] = 0;

    snprintf(archivePath, sizeof(archivePath), "data/%s.txt", archiveName);

    FILE *exportFile = fopen(archivePath, "w");
    if (!exportFile) {
        printf("Error creating export file.\n");
        fclose(f);
        return;
    }

    fseek(f, 0, SEEK_SET);

    while (fread(&tmp, sizeof(Book), 1, f))
    {
        fprintf(exportFile, "ID: %hu\n", tmp.id);
        fprintf(exportFile, "Title: %s\n", tmp.title);
        fprintf(exportFile, "Author: %s\n", tmp.author);
        fprintf(exportFile, "Theme: %s\n", tmp.theme);
        fprintf(exportFile, "Read: %s\n", tmp.read ? "Yes" : "No");
        fprintf(exportFile, "Release Year: %hu\n", tmp.releaseYear);
        fprintf(exportFile, "Active: %s\n", tmp.active ? "Yes" : "No");
        fprintf(exportFile, "-------------------------\n");
    }

    fclose(exportFile);
    fclose(f);
}

void exportBooksToTxt(){
    listBooksTxt();
}

void importBooksFromCsv(){
    char archivePath[150];
    char archiveName[100];

    printf("Enter the name of the CSV file to import from (without extension): ");
    fgets(archiveName, sizeof(archiveName), stdin);

    archiveName[strcspn(archiveName, "\n")] = 0;

    snprintf(archivePath, sizeof(archivePath), "data/%s.csv", archiveName);

    FILE *csvFile = fopen(archivePath, "r");
    if (!csvFile) {
        printf("Error opening CSV file.\n");
        return;
    }

    FILE *dbFile = fopen("data/books.dat", "ab");
    if (!dbFile) {
        printf("Error opening database file.\n");
        fclose(csvFile);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), csvFile)) {
        Book tmp;
        char *token;

        tmp.id = getNextBookID(); // Get a new ID for the book
        // Remove newline character from the end of the line
        line[strcspn(line, "\n")] = 0;

        // Get Title
        token = strtok(line, ",");
        if (token) strncpy(tmp.title, token, MAX_TITLE_LENGTH);
        else continue;

        // Get Author
        token = strtok(NULL, ",");
        if (token) strncpy(tmp.author, token, MAX_AUTHOR_NAME_LENGTH);
        else continue;

        // Get Theme
        token = strtok(NULL, ",");
        if (token) strncpy(tmp.theme, token, MAX_THEME_LENGTH);
        else continue;

        // Get Read status
        token = strtok(NULL, ",");
        if (token) tmp.read = (strcmp(token, "1") == 0);
        else continue;

        // Get Release Year
        token = strtok(NULL, ",");
        if (token) tmp.releaseYear = (unsigned short)atoi(token);
        else continue;

        // Set active to true for imported books
        tmp.active = true;

        // Write to database
        fwrite(&tmp, sizeof(Book), 1, dbFile);
    }

    fclose(csvFile);
    fclose(dbFile);
    printf("Books imported successfully from %s\n", archivePath);
}

void dataDirectoryCheck(){
    // Check if the "data" directory exists, if not create it
    #ifdef _WIN32
        system("if not exist data mkdir data");
    #else
        system("mkdir -p data");
    #endif

    //The "data" directory is now ensured to exist
}

void countBooks(){
    FILE *f = fopen("data/last_id.txt", "r");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    unsigned short lastID;
    fscanf(f, "%hu", &lastID);
    fclose(f);

    printf("Total number of books: %hu\n", lastID);
}

void countBooksByTheme(char theme[MAX_THEME_LENGTH]){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }
    unsigned short count = 0;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(strcmp(tmp.theme, theme) && tmp.active){
            count++;
        }
    }

    printf("Total number of books with theme '%s': %hu\n", theme, count);

    fclose(f);
}

void countBooksByAuthor(char author[MAX_AUTHOR_NAME_LENGTH]){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }
    unsigned short count = 0;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(strcmp(tmp.author, author) && tmp.active){
            count++;
        }
    }

    printf("Total number of books by author '%s': %hu\n", author, count);

    fclose(f);
}

void backupDatabase(){
    FILE *data = fopen("data/books.dat", "rb");

    if (!data){
        printf("Error opening file for backup   .\n");
        return;
    }

    FILE *backup = fopen("data/backup_books", "wb");

    if (!backup){
        printf("Error opening backup file.\n");
        fclose(data);
        return;
    }

    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, data)){
        fwrite(&tmp, sizeof(Book), 1, backup);
    }

    fclose(data);
    fclose(backup);
    printf("Backup created successfully.\n");
}

void restoreDatabase(){
    FILE *backup = fopen("data/backup_books", "rb");

    if (!backup){
        printf("Error opening backup file.\n");
        return;
    }

    FILE *data = fopen("data/books.dat", "wb");

    if (!data){
        printf("Error opening data file for restoration.\n");
        fclose(backup);
        return;
    }

    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, backup)){
        fwrite(&tmp, sizeof(Book), 1, data);
    }

    fclose(backup);
    fclose(data);
    printf("Database restored successfully from backup.\n");
}

void markBookAsRead(unsigned short id){
    FILE *f = fopen("data/books.dat", "rb");
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

void markBookAsUnread(unsigned short id){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(tmp.id == id && tmp.active){
            tmp.read = false;
            // Move the file pointer back to the start of the current record
            fseek(f, -sizeof(Book), SEEK_CUR);
            fwrite(&tmp, sizeof(Book), 1, f);
            printf("Book with ID %hu marked as unread.\n", id);
            flag = true;
            break;
        }
    }
    
    if (!flag){
        printf("Book with ID %hu not found or is inactive.\n", id);
    }
    fclose(f);
}

void getOldestBook(){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    Book tmp = {0};
    fread(&tmp, sizeof(Book), 1, f);
    Book oldest = tmp;

    while (fread(&tmp, sizeof(Book), 1, f)){
        if (oldest.releaseYear > tmp.releaseYear && tmp.active){
            oldest = tmp;
        }
    }

    printf("The oldest book in your library is %s and was release in %hu.\n", tmp.title, tmp.releaseYear);

    fclose(f);
}

void getNewestBook(){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    Book tmp = {0};
    fread(&tmp, sizeof(Book), 1, f);
    Book newest = tmp;

    while (fread(&tmp, sizeof(Book), 1, f)){
        if (newest.releaseYear < tmp.releaseYear && tmp.active){
            newest = tmp;
        }
    }

    printf("The newest book in your library is %s and was release in %hu.\n", tmp.title, tmp.releaseYear);

    fclose(f);
}

// Comparison function for qsort
int compareBooksByTitle(const void *a, const void *b){
    const Book *bookA = (const Book *)a;
    const Book *bookB = (const Book *)b;
    return strcmp(bookA->title, bookB->title);
}

void sortBooksByTitle(){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    unsigned short id = getId();

    //Quicksort
    Book *books = malloc(sizeof(Book) * id);
    if (!books) {
        printf("Memory allocation error.\n");
        fclose(f);
        return;
    }

    size_t count = 0;
    while (fread(&books[count], sizeof(Book), 1, f) && count < id) {
        count++;
    }

    fclose(f);

    // Sort the books array using quicksort
    qsort(books, count, sizeof(Book), compareBooksByTitle);

    // Write the sorted books back to the file
    f = fopen("data/books.dat", "wb");
    if (!f) {
        printf("Error opening file for writing.\n");
        free(books);
        return;
    }

    fwrite(books, sizeof(Book), count, f);
    fclose(f);
    free(books);
}

int compareBooksByReleaseYear(const void *a, const void *b) {
    const Book *bookA = (const Book *)a;
    const Book *bookB = (const Book *)b;
    return bookA->releaseYear - bookB->releaseYear;
}

void sortBooksByYear(){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    unsigned short id = getId();
    
    //Quicksort
    Book *books = malloc(sizeof(Book) * id);
    if (!books) {
        printf("Memory allocation error.\n");
        fclose(f);
        return;
    }

    size_t count = 0;
    while (fread(&books[count], sizeof(Book), 1, f) && count < id) {
        count++;
    }

    fclose(f);

    // Sort the books array using quicksort
    qsort(books, count, sizeof(Book), compareBooksByReleaseYear);

    // Write the sorted books back to the file
    f = fopen("data/books.dat", "wb");
    if (!f) {
        printf("Error opening file for writing.\n");
        free(books);
        return;
    }

    fwrite(books, sizeof(Book), count, f);
    fclose(f);
    free(books);
}

int compareBooksByID(const void *a, const void *b){
    const Book *bookA = (const Book *)a;
    const Book *bookB = (const Book *)b;
    return bookA->id - bookB->id;
}

void sortBooksByID(){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    unsigned short id = getId();
    
    //Quicksort
    Book *books = malloc(sizeof(Book) * id);
    if (!books) {
        printf("Memory allocation error.\n");
        fclose(f);
        return;
    }

    size_t count = 0;
    while (fread(&books[count], sizeof(Book), 1, f) && count < id) {
        count++;
    }

    fclose(f);

    // Sort the books array using quicksort
    qsort(books, count, sizeof(Book), compareBooksByID);

    // Write the sorted books back to the file
    f = fopen("data/books.dat", "wb");
    if (!f) {
        printf("Error opening file for writing.\n");
        free(books);
        return;
    }

    fwrite(books, sizeof(Book), count, f);
    fclose(f);
    free(books);
}

int compareBooksByAuthor(const void *a, const void *b){
    const Book *bookA = (const Book *)a;
    const Book *bookB = (const Book *)b;
    return strcmp(bookA->author, bookB->author);
}

void sortBooksByAuthor(){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    unsigned short id = getId();

    //Quicksort
    Book *books = malloc(sizeof(Book) * id);
    if (!books) {
        printf("Memory allocation error.\n");
        fclose(f);
        return;
    }


    size_t count = 0;
    while (fread(&books[count], sizeof(Book), 1, f) && count < id) {
        count++;
    }

    fclose(f);

    // Sort the books array using quicksort
    qsort(books, count, sizeof(Book), compareBooksByAuthor);

    // Write the sorted books back to the file
    f = fopen("data/books.dat", "wb");
    if (!f) {
        printf("Error opening file for writing.\n");
        free(books);
        return;
    }

    fwrite(books, sizeof(Book), count, f);
    fclose(f);
    free(books);
}

int compareBooksByTheme(const void *a, const void *b){
    const Book *bookA = (const Book *)a;
    const Book *bookB = (const Book *)b;
    return strcmp(bookA->theme, bookB->theme);
}

void sortBooksByTheme(){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    unsigned short id = getId();

    //Quicksort
    Book *books = malloc(sizeof(Book) * id);
    if (!books) {
        printf("Memory allocation error.\n");
        fclose(f);
        return;
    }

    size_t count = 0;
    while (fread(&books[count], sizeof(Book), 1, f) && count < id) {
        count++;
    }

    fclose(f);

    // Sort the books array using quicksort
    qsort(books, count, sizeof(Book), compareBooksByTheme);

    // Write the sorted books back to the file
    f = fopen("data/books.dat", "wb");
    if (!f) {
        printf("Error opening file for writing.\n");
        free(books);
        return;
    }

    fwrite(books, sizeof(Book), count, f);
    fclose(f);
    free(books);
}

void compressDatabase(){
    // Placeholder for future implementation
    //LZW Compression
    printf("Database compression functionality is not yet implemented.\n");
}

void decompressDatabase(){
    // Placeholder for future implementation
    //LZW Decompression
    printf("Database decompression functionality is not yet implemented.\n");
}

void getDatabaseStatistics(){
    // Placeholder for future implementation
    printf("Database statistics functionality is not yet implemented.\n");
}

long getDatabaseSizeInBytes() {
    FILE *file = fopen("data/books.dat", "rb");
    if (!file) {
        printf("Failed to open file: data/books.dat\n");
        return -1;
    }

    fseek(file, 0, SEEK_END);     // Move the cursor to the end of the file
    long size = ftell(file);      // Get the current position = file size
    fclose(file);

    return size;
}

void printDatabaseSize() {
    long size = getDatabaseSizeInBytes();

    printf("What measure unit would you like to use?\n");
    printf("1. Bytes\n");
    printf("2. Kilobytes (KB)\n");
    printf("3. Megabytes (MB)\n");
    printf("Please enter your choice (1-3): ");

    int choice;
    scanf("%d", &choice);
    clearInputBuffer();

    if (size != -1) {
        switch (choice) {
        case 1:
            printf("Database size: %ld bytes\n", size);
            break;
        case 2:
            printf("Database size: %.2f KB\n", size / 1024.0);
            break;
        case 3:
            printf("Database size: %.2f MB\n", size / (1024.0 * 1024.0));
            break;
        default:
            printf("Invalid choice.\n");
            break;
        }
    } else {
        printf("Could not retrieve database size.\n");
    }
}

void searchBooksMultipleCriteria(const char *title, const char *author, const char *theme, unsigned short year, bool read, bool active){
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    Book tmp = {0};
    while (fread(&tmp, sizeof(Book), 1, f)) {
        if ((title == NULL || strcmp(tmp.title, title) == 0) &&
            (author == NULL || strcmp(tmp.author, author) == 0) &&
            (theme == NULL || strcmp(tmp.theme, theme) == 0) &&
            (year == 0 || tmp.releaseYear == year) &&
            (read == tmp.read) &&
            (active == tmp.active)) {
            // Print the matching book
            printf("Found matching book: %hu ID: %s by %s\n", tmp.id, tmp.title, tmp.author);
        }
    }

    fclose(f);
}