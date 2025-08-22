#include <mydatabase.h>

void clearInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

unsigned short getNextBookID(){
    FILE *f = fopen("last_id.dat", "r+b");
    unsigned short id = 0;

    //Gets the last id used
    fread(&id, sizeof(unsigned short), 1, f);
    id++;
    
    //Back to the start of the file
    rewind(f);

    //Write new id
    fwrite(id, sizeof(unsigned short), 1, f);
    fclose(f);

    return id;
}

void printBook(Book book){
    printf("Book id: %hu\n", book.id);
    printf("Book title: %s\n", book.title);
    printf("Author name: %s\n", book.author);
    printf("Book theme: %s\n", book.theme);
    printf("Release year: %hu\n", book.releaseYear);
    printf("Book was read: %s\n", book.read ? "Yes" : "No");
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
    bool flag = false;
    Book tmp = {0};

    while (fread(&tmp, sizeof(Book), 1, f)){
        if(tmp.id == id && tmp.active){
            printBook(tmp);
            flag = true;
        }
    }
    
    if (!flag){
        printf("Book with ID %hu not found or is inactive.\n", id);
    }
    fclose(f);
}