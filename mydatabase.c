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

/* ─────────────────────────────────────────────────────────────────────────────
 * LZW Compression  (12-bit codes → up to 4096 dictionary entries)
 * Output file format:
 *   [4 bytes] magic  "LZW\x01"
 *   [8 bytes] original file size (uint64_t, little-endian)
 *   [n bytes] 12-bit codes packed two per three bytes
 *             (code_A[11:4] | code_A[3:0]<<4|code_B[11:8] | code_B[7:0])
 * ───────────────────────────────────────────────────────────────────────── */

#define LZW_MAX_CODES  4096   /* 2^12 */
#define LZW_BITS       12

/* Hash-table entry for the compression dictionary */
typedef struct {
    int  prefix;   /* code of the prefix string (-1 = empty slot) */
    int  byte;     /* the extension byte                           */
    int  code;     /* the code assigned to this entry              */
} LZWEntry;

static int lzw_hash(int prefix, int byte) {
    /* simple hash: mix prefix and byte */
    unsigned h = (unsigned)(prefix * 256 + byte);
    h = (h ^ (h >> 16)) * 0x45d9f3b;
    return (int)(h % LZW_MAX_CODES);
}

static int lzw_find(LZWEntry *dict, int prefix, int byte) {
    int h = lzw_hash(prefix, byte);
    while (dict[h].prefix != -1) {
        if (dict[h].prefix == prefix && dict[h].byte == byte)
            return dict[h].code;
        h = (h + 1) % LZW_MAX_CODES;
    }
    return -1;
}

static void lzw_insert(LZWEntry *dict, int prefix, int byte, int code) {
    int h = lzw_hash(prefix, byte);
    while (dict[h].prefix != -1)
        h = (h + 1) % LZW_MAX_CODES;
    dict[h].prefix = prefix;
    dict[h].byte   = byte;
    dict[h].code   = code;
}

/* Write 12-bit codes packed two per three bytes into buf; flush to file. */
typedef struct {
    FILE         *out;
    unsigned char buf[3];
    int           pending;   /* 0 or 1 half-stored code */
    int           pending_code;
} BitWriter;

static void bw_flush(BitWriter *bw) {
    if (bw->pending) {
        /* we have one code that was stored in the high nibble of buf[1]; buf[2] = 0 */
        bw->buf[1] = (unsigned char)((bw->pending_code & 0x0F) << 4);
        bw->buf[2] = 0;
        fwrite(bw->buf, 1, 3, bw->out);
        bw->pending = 0;
    }
}

static void bw_write_code(BitWriter *bw, int code) {
    if (!bw->pending) {
        /* first code of a pair: occupies buf[0] and high nibble of buf[1] */
        bw->buf[0] = (unsigned char)((code >> 4) & 0xFF);
        bw->pending_code = code;
        bw->pending = 1;
    } else {
        /* second code of a pair */
        bw->buf[1] = (unsigned char)(((bw->pending_code & 0x0F) << 4) | ((code >> 8) & 0x0F));
        bw->buf[2] = (unsigned char)(code & 0xFF);
        fwrite(bw->buf, 1, 3, bw->out);
        bw->pending = 0;
    }
}

void compressDatabase() {
    /* 1. Open source file */
    FILE *src = fopen("data/books.dat", "rb");
    if (!src) {
        printf("Error: could not open data/books.dat for reading.\n");
        return;
    }

    /* Measure size */
    fseek(src, 0, SEEK_END);
    long src_size = ftell(src);
    fseek(src, 0, SEEK_SET);

    if (src_size == 0) {
        printf("Database is empty – nothing to compress.\n");
        fclose(src);
        return;
    }

    /* 2. Open destination file */
    FILE *dst = fopen("data/books.lzw", "wb");
    if (!dst) {
        printf("Error: could not create data/books.lzw.\n");
        fclose(src);
        return;
    }

    /* 3. Write header */
    const unsigned char magic[4] = {'L','Z','W','\x01'};
    fwrite(magic, 1, 4, dst);
    uint64_t orig_size = (uint64_t)src_size;
    fwrite(&orig_size, sizeof(orig_size), 1, dst);

    /* 4. Initialise dictionary (256 single-byte entries) */
    LZWEntry *dict = (LZWEntry *)malloc(sizeof(LZWEntry) * LZW_MAX_CODES);
    if (!dict) {
        printf("Memory allocation error.\n");
        fclose(src); fclose(dst);
        return;
    }
    memset(dict, -1, sizeof(LZWEntry) * LZW_MAX_CODES);
    /* We do NOT pre-fill the hash table with the 256 symbols; instead we handle
       single-byte codes directly (code == byte) and only hash multi-byte strings */
    int next_code = 256;

    BitWriter bw = { dst, {0}, 0, 0 };

    /* 5. LZW encoding loop */
    int c = fgetc(src);
    if (c == EOF) {
        /* Empty – already handled above */
        free(dict);
        fclose(src); fclose(dst);
        return;
    }
    int w = c; /* current prefix code */

    while ((c = fgetc(src)) != EOF) {
        int code = lzw_find(dict, w, c);
        if (code != -1) {
            /* w+c is already in the dictionary */
            w = code;
        } else {
            /* Emit w */
            bw_write_code(&bw, w);
            /* Add w+c to the dictionary if there is still room */
            if (next_code < LZW_MAX_CODES) {
                lzw_insert(dict, w, c, next_code++);
            }
            w = c;
        }
    }
    /* Emit the last code */
    bw_write_code(&bw, w);
    bw_flush(&bw);

    free(dict);
    fclose(src);
    fclose(dst);

    /* Report results */
    FILE *chk = fopen("data/books.lzw", "rb");
    long compressed_size = 0;
    if (chk) { fseek(chk,0,SEEK_END); compressed_size = ftell(chk); fclose(chk); }

    printf("Compression complete!\n");
    printf("  Original : %ld bytes\n", src_size);
    printf("  Compressed: %ld bytes\n", compressed_size);
    if (src_size > 0)
        printf("  Ratio    : %.1f%%\n", 100.0 * compressed_size / src_size);
    printf("  Saved to : data/books.lzw\n");
}

/* ─────────────────────────────────────────────────────────────────────────────
 * LZW Decompression
 * ───────────────────────────────────────────────────────────────────────── */

/* Decompression dictionary: each entry stores the full string as a chain */
typedef struct {
    int  prefix;  /* code of the prefix, or -1 for single-byte entries */
    int  byte;    /* the last byte of this entry                        */
} LZWDecEntry;

/* Recursively write the string represented by code to buf (reversed order),
   returns the number of bytes written. */
static int lzw_decode_string(LZWDecEntry *dict, unsigned char *buf, int code) {
    int len = 0;
    while (code >= 256) {
        buf[len++] = (unsigned char)dict[code].byte;
        code = dict[code].prefix;
    }
    buf[len++] = (unsigned char)code;
    /* buf is in reverse order – reverse it */
    for (int i = 0, j = len - 1; i < j; i++, j--) {
        unsigned char tmp = buf[i]; buf[i] = buf[j]; buf[j] = tmp;
    }
    return len;
}

/* Read the next 12-bit code from the packed stream; returns -1 on EOF */
typedef struct {
    FILE         *in;
    unsigned char buf[3];
    int           pending;   /* 0 or 1 code buffered from previous triple */
    int           pending_code;
} BitReader;

static int br_read_code(BitReader *br) {
    if (br->pending) {
        br->pending = 0;
        return br->pending_code;
    }
    unsigned char b[3];
    if (fread(b, 1, 3, br->in) < 3) return -1;
    int code_a = ((int)b[0] << 4) | ((b[1] >> 4) & 0x0F);
    int code_b = (((int)b[1] & 0x0F) << 8) | b[2];
    br->pending_code = code_b;
    br->pending = 1;
    return code_a;
}

void decompressDatabase() {
    /* 1. Open compressed file */
    FILE *src = fopen("data/books.lzw", "rb");
    if (!src) {
        printf("Error: data/books.lzw not found. Run compression first.\n");
        return;
    }

    /* 2. Read and validate header */
    unsigned char magic[4];
    if (fread(magic, 1, 4, src) < 4 ||
        magic[0] != 'L' || magic[1] != 'Z' || magic[2] != 'W' || magic[3] != '\x01') {
        printf("Error: data/books.lzw is not a valid LZW file.\n");
        fclose(src);
        return;
    }
    uint64_t orig_size = 0;
    fread(&orig_size, sizeof(orig_size), 1, src);

    /* 3. Open output file */
    FILE *dst = fopen("data/books.dat", "wb");
    if (!dst) {
        printf("Error: could not open data/books.dat for writing.\n");
        fclose(src);
        return;
    }

    /* 4. Initialise decode dictionary */
    LZWDecEntry *dict = (LZWDecEntry *)malloc(sizeof(LZWDecEntry) * LZW_MAX_CODES);
    if (!dict) {
        printf("Memory allocation error.\n");
        fclose(src); fclose(dst);
        return;
    }
    for (int i = 0; i < 256; i++) { dict[i].prefix = -1; dict[i].byte = i; }
    int next_code = 256;

    unsigned char *str_buf = (unsigned char *)malloc(LZW_MAX_CODES);
    if (!str_buf) {
        printf("Memory allocation error.\n");
        free(dict); fclose(src); fclose(dst);
        return;
    }

    BitReader br = { src, {0}, 0, 0 };

    /* 5. LZW decoding loop */
    int old_code = br_read_code(&br);
    if (old_code == -1) {
        printf("Error: compressed file is empty.\n");
        free(dict); free(str_buf); fclose(src); fclose(dst);
        return;
    }

    unsigned char first_byte = (unsigned char)old_code;
    fputc((unsigned char)old_code, dst);

    int code;
    while ((code = br_read_code(&br)) != -1) {
        int len;
        if (code < next_code) {
            /* code is in the dictionary */
            len = lzw_decode_string(dict, str_buf, code);
            first_byte = str_buf[0];
        } else {
            /* Special case: code == next_code */
            str_buf[0] = first_byte;
            len = lzw_decode_string(dict, str_buf + 1, old_code);
            str_buf[len] = first_byte;  /* append first_byte */
            len++;
            first_byte = str_buf[0];
        }
        fwrite(str_buf, 1, (size_t)len, dst);

        /* Add old_code + first_byte to the dictionary */
        if (next_code < LZW_MAX_CODES) {
            dict[next_code].prefix = old_code;
            dict[next_code].byte   = (int)first_byte;
            next_code++;
        }
        old_code = code;
    }

    free(dict);
    free(str_buf);
    fclose(src);
    fclose(dst);

    printf("Decompression complete!\n");
    printf("  Restored : data/books.dat  (%llu bytes expected)\n", (unsigned long long)orig_size);
}

void getDatabaseStatistics() {
    FILE *f = fopen("data/books.dat", "rb");
    if (!f) {
        printf("Error: could not open data/books.dat.\n");
        return;
    }

    /* ── Counters ─────────────────────────────────────────────────── */
    unsigned int total    = 0;
    unsigned int active   = 0;
    unsigned int inactive = 0;
    unsigned int read_count   = 0;
    unsigned int unread_count = 0;

    unsigned short oldest_year = 9999;
    unsigned short newest_year = 0;
    char oldest_title[MAX_TITLE_LENGTH]  = "";
    char newest_title[MAX_TITLE_LENGTH]  = "";

    /* For top-author / top-theme we keep a small dynamic list */
    typedef struct { char name[MAX_AUTHOR_NAME_LENGTH]; unsigned int count; } AuthorStat;
    typedef struct { char name[MAX_THEME_LENGTH];       unsigned int count; } ThemeStat;

    AuthorStat *authors = NULL;
    ThemeStat  *themes  = NULL;
    unsigned int author_count = 0;
    unsigned int theme_count  = 0;

    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)) {
        total++;
        if (tmp.active)   active++;
        else              inactive++;
        if (tmp.read)     read_count++;
        else              unread_count++;

        /* Oldest / Newest (only active books) */
        if (tmp.active) {
            if (tmp.releaseYear < oldest_year) {
                oldest_year = tmp.releaseYear;
                strncpy(oldest_title, tmp.title, MAX_TITLE_LENGTH - 1);
            }
            if (tmp.releaseYear > newest_year) {
                newest_year = tmp.releaseYear;
                strncpy(newest_title, tmp.title, MAX_TITLE_LENGTH - 1);
            }
        }

        /* ── Author frequency ───────────────────────────────────── */
        bool found_author = false;
        for (unsigned int i = 0; i < author_count; i++) {
            if (strcmp(authors[i].name, tmp.author) == 0) {
                authors[i].count++;
                found_author = true;
                break;
            }
        }
        if (!found_author) {
            AuthorStat *tmp_a = realloc(authors, (author_count + 1) * sizeof(AuthorStat));
            if (tmp_a) {
                authors = tmp_a;
                strncpy(authors[author_count].name, tmp.author, MAX_AUTHOR_NAME_LENGTH - 1);
                authors[author_count].name[MAX_AUTHOR_NAME_LENGTH - 1] = '\0';
                authors[author_count].count = 1;
                author_count++;
            }
        }

        /* ── Theme frequency ────────────────────────────────────── */
        bool found_theme = false;
        for (unsigned int i = 0; i < theme_count; i++) {
            if (strcmp(themes[i].name, tmp.theme) == 0) {
                themes[i].count++;
                found_theme = true;
                break;
            }
        }
        if (!found_theme) {
            ThemeStat *tmp_t = realloc(themes, (theme_count + 1) * sizeof(ThemeStat));
            if (tmp_t) {
                themes = tmp_t;
                strncpy(themes[theme_count].name, tmp.theme, MAX_THEME_LENGTH - 1);
                themes[theme_count].name[MAX_THEME_LENGTH - 1] = '\0';
                themes[theme_count].count = 1;
                theme_count++;
            }
        }
    }
    fclose(f);

    /* ── Find top author & theme ───────────────────────────────── */
    char top_author[MAX_AUTHOR_NAME_LENGTH] = "N/A";
    unsigned int top_author_count = 0;
    for (unsigned int i = 0; i < author_count; i++) {
        if (authors[i].count > top_author_count) {
            top_author_count = authors[i].count;
            strncpy(top_author, authors[i].name, MAX_AUTHOR_NAME_LENGTH - 1);
        }
    }

    char top_theme[MAX_THEME_LENGTH] = "N/A";
    unsigned int top_theme_count = 0;
    for (unsigned int i = 0; i < theme_count; i++) {
        if (themes[i].count > top_theme_count) {
            top_theme_count = themes[i].count;
            strncpy(top_theme, themes[i].name, MAX_THEME_LENGTH - 1);
        }
    }

    free(authors);
    free(themes);

    /* ── File size ─────────────────────────────────────────────── */
    long db_size = getDatabaseSizeInBytes();

    /* ── Print dashboard ───────────────────────────────────────── */
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║          DATABASE STATISTICS             ║\n");
    printf("╠══════════════════════════════════════════╣\n");

    if (total == 0) {
        printf("║  No books found in the database.         ║\n");
        printf("╚══════════════════════════════════════════╝\n");
        return;
    }

    float pct_active = total ? 100.0f * active   / total : 0.0f;
    float pct_read   = total ? 100.0f * read_count / total : 0.0f;

    printf("║  Total books      : %-5u                ║\n", total);
    printf("║  Active           : %-5u  (%.1f%%)        ║\n", active,   pct_active);
    printf("║  Inactive         : %-5u  (%.1f%%)        ║\n", inactive, 100.0f - pct_active);
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  Read             : %-5u  (%.1f%%)        ║\n", read_count,   pct_read);
    printf("║  Unread           : %-5u  (%.1f%%)        ║\n", unread_count, 100.0f - pct_read);
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  Unique authors   : %-5u                ║\n", author_count);
    printf("║  Unique themes    : %-5u                ║\n", theme_count);
    printf("╠══════════════════════════════════════════╣\n");

    if (oldest_year != 9999)
        printf("║  Oldest book  (%4hu): %-20.20s║\n", oldest_year, oldest_title);
    if (newest_year != 0)
        printf("║  Newest book  (%4hu): %-20.20s║\n", newest_year, newest_title);

    printf("╠══════════════════════════════════════════╣\n");
    printf("║  Top author  : %-26.26s║\n", top_author);
    printf("║    (%u book%s)                              ║\n",
           top_author_count, top_author_count == 1 ? " " : "s");
    printf("║  Top theme   : %-26.26s║\n", top_theme);
    printf("║    (%u book%s)                              ║\n",
           top_theme_count, top_theme_count == 1 ? " " : "s");

    printf("╠══════════════════════════════════════════╣\n");
    if (db_size >= 0)
        printf("║  File size        : %-8ld bytes      ║\n", db_size);

    printf("╚══════════════════════════════════════════╝\n");
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