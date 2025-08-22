#include "mydatabase.h"

int main(){
    int option;

    printf("Welcome to the Book Database!\n");
    do {
        printf("\nMenu:\n");
        printf("1. Add Book\n");
        printf("2. View Book by ID\n");
        printf("3. Update Book by ID\n");
        printf("4. Delete Book by ID\n");
        printf("5. Search Book by ID\n");
        printf("6. Search Book by Title\n");
        printf("7. Search Book by Author\n");
        printf("8. Search Book by Theme\n");
        printf("9. Search Books by Read Status\n");
        printf("10. Search Books by Release Year\n");
        printf("11. List All Active Books\n");
        printf("12. List All Books\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);
        clearInputBuffer(); // Clear the input buffer after scanf

        switch (option) {
            case 1:
                createBook();
                break;
            case 2: {
                unsigned short id;
                printf("Enter Book ID: ");
                scanf("%hu", &id);
                clearInputBuffer();
                readBook(id);
                break;
            }
            case 3: {
                unsigned short id;
                int updateOption;
                printf("Enter Book ID to update: ");
                scanf("%hu", &id);
                clearInputBuffer();
                printf("The book with ID %hu will be marked as read.\n", id);
                updateBook(id);
                break;
            }
            case 4: {
                unsigned short id;
                printf("Enter Book ID to delete: ");
                scanf("%hu", &id);
                clearInputBuffer();
                deleteBook(id);
                break;
            }
            case 5: {
                unsigned short id;
                printf("Enter Book ID to search: ");
                scanf("%hu", &id);
                clearInputBuffer();
                searchBookID(id);
                break;
            }
            case 6: {
                char title[MAX_TITLE_LENGTH];
                printf("Enter Book Title to search: ");
                fgets(title, MAX_TITLE_LENGTH, stdin);
                searchBookTitle(title);
                break;
            }
            case 7: {
                char author[MAX_AUTHOR_NAME_LENGTH];
                printf("Enter Author Name to search: ");
                fgets(author, MAX_AUTHOR_NAME_LENGTH, stdin);
                searchBookAuthor(author);
                break;
            }
            case 8: {
                char theme[MAX_THEME_LENGTH];
                printf("Enter Book Theme to search: ");
                fgets(theme, MAX_THEME_LENGTH, stdin);
                searchBookTheme(theme);
                break;
            }
            case 9: {
                int readOption;
                printf("Search for books that are (1) Read or (0) Unread? ");
                scanf("%d", &readOption);
                clearInputBuffer();
                searchBookRead(readOption ? true : false);
                break;
            }
            case 10: {
                unsigned short year;
                printf("Enter Release Year to search: ");
                scanf("%hu", &year);
                clearInputBuffer();
                searchBookYear(year);
                break;
            }
            case 11:
                searchBookActive();
                break;
            case 12:
                listBooks();
                break;
            case 0:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    } while (option != 0);

    return 0;

}
