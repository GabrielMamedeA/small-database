#include "mydatabase.h"

int main(){
    int option;
    dataDirectoryCheck(); // Ensure the data directory exists

    printf("Welcome to the Book Database!\n");
    do {
        printf("\nMenu:\n");
        printf("1. Add Book\n");
        printf("2. Update Book by ID\n");
        printf("3. Delete Book by ID\n");
        printf("4. Search Menu\n");
        printf("5. List All Active Books\n");
        printf("6. List All Books\n");
        printf("7. Import Books from CSV\n");
        printf("8. Export Books to TXT\n");
        printf("9. Sort Books Menu\n");
        printf("10. Mark Book as Read\n");
        printf("11. Mark a Book as Unread\n");
        printf("12. Backup Menu\n");
        printf("13. Database Statistics\n");

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
                int updateOption;
                printf("Enter Book ID to update: ");
                scanf("%hu", &id);
                clearInputBuffer();
                printf("The book with ID %hu will be marked as read.\n", id);
                updateBook(id);
                break;
            }
            case 3: {
                unsigned short id;
                printf("Enter Book ID to delete: ");
                scanf("%hu", &id);
                clearInputBuffer();
                deleteBook(id);
                break;
            }
            case 4: {
                int searchOption;
                printf("Search by:\n");
                printf("1. ID\n");
                printf("2. Title\n");
                printf("3. Author\n");
                printf("4. Theme\n");
                printf("5. Read Status\n");
                printf("6. Release Year\n");
                printf("7. Active Status\n");
                printf("8. Multiple Criteria\n");
                printf("0. Cancel\n");
                printf("Choose an option: ");
                scanf("%d", &searchOption);
                clearInputBuffer();

                switch (searchOption) {
                    case 1: {
                        unsigned short id;
                        printf("Enter Book ID to search: ");
                        scanf("%hu", &id);
                        clearInputBuffer();
                        searchBookID(id);
                        break;
                    }
                    case 2: {
                        char title[MAX_TITLE_LENGTH];
                        printf("Enter Book Title to search: ");
                        fgets(title, MAX_TITLE_LENGTH, stdin);
                        searchBookTitle(title);
                        break;
                    }
                    case 3: {
                        char author[MAX_AUTHOR_NAME_LENGTH];
                        printf("Enter Author Name to search: ");
                        fgets(author, MAX_AUTHOR_NAME_LENGTH, stdin);
                        searchBookAuthor(author);
                        break;
                    }
                    case 4: {
                        char theme[MAX_THEME_LENGTH];
                        printf("Enter Book Theme to search: ");
                        fgets(theme, MAX_THEME_LENGTH, stdin);
                        searchBookTheme(theme);
                        break;
                    }
                    case 5: {
                        int readOption;
                        printf("Search for books that are (1) Read or (0) Unread? ");
                        scanf("%d", &readOption);
                        clearInputBuffer();
                        searchBookRead(readOption ? true : false);
                        break;
                    }
                    case 6: {
                        unsigned short year;
                        printf("Enter Release Year to search: ");
                        scanf("%hu", &year);
                        clearInputBuffer();
                        searchBookYear(year);
                        break;
                    }
                    case 7:
                        searchBookActive();
                        break;
                    case 8: {
                        char title[MAX_TITLE_LENGTH];
                        char author[MAX_AUTHOR_NAME_LENGTH];
                        char theme[MAX_THEME_LENGTH];
                        unsigned short year;
                        bool read;
                        bool active;

                        printf("Enter Book Title (or leave blank): ");
                        fgets(title, MAX_TITLE_LENGTH, stdin);
                        title[strcspn(title, "\n")] = 0;  // Remove newline

                        printf("Enter Author Name (or leave blank): ");
                        fgets(author, MAX_AUTHOR_NAME_LENGTH, stdin);
                        author[strcspn(author, "\n")] = 0;

                        printf("Enter Book Theme (or leave blank): ");
                        fgets(theme, MAX_THEME_LENGTH, stdin);
                        theme[strcspn(theme, "\n")] = 0;

                        printf("Enter Release Year (or 0 to ignore): ");
                        scanf("%hu", &year);
                        clearInputBuffer();

                        printf("Search for Read Books? (1 for Yes, 0 for No): ");
                        scanf("%d", &read);
                        clearInputBuffer();

                        printf("Search for Active Books? (1 for Yes, 0 for No): ");
                        scanf("%d", &active);
                        clearInputBuffer();

                        searchBooksMultipleCriteria(
                            strlen(title) > 0 ? title : NULL,
                            strlen(author) > 0 ? author : NULL,
                            strlen(theme) > 0 ? theme : NULL,
                            year > 0 ? year : 0,
                            read,
                            active
                        );
                        break;
                    }
                    case 0:
                        printf("Search cancelled.\n");
                        break;
                    default:
                        printf("Invalid search option.\n");
                }
                break;
            }
            case 5: {
                searchBookActive();
                break;
            }
            case 6: {
                listBooks();
                break;
            }

            case 7: {
                importBooksFromCsv();
                break;
            }
            case 8: {
                exportBooksToTxt();
                break;
            }
            case 9: {
                printf("Sort books by:\n");
                printf("1. Id\n");
                printf("2. Author\n");
                printf("3. Theme\n");
                printf("4. Title\n");
                printf("5. Release Year\n");
                printf("0. Cancel\n");
                printf("Choose an option: ");
                int sortOption;
                scanf("%d", &sortOption);
                clearInputBuffer();

                switch (sortOption) {
                    case 1:
                        sortBooksByID();
                        break;
                    case 2:
                        sortBooksByAuthor();
                        break;
                    case 3:
                        sortBooksByTheme();
                        break;
                    case 4:
                        sortBooksByTitle();
                        break;
                    case 5:
                        sortBooksByYear();
                        break;
                    case 0:
                        printf("Sorting cancelled.\n");
                        break;
                    default:
                        printf("Invalid sort option.\n");
                }
                break;
            }
            case 10:
                printf("Enter Book ID to mark as read: ");
                int bookId;
                scanf("%d", &bookId);
                clearInputBuffer();
                markBookAsRead(bookId);
                break;
            case 11:
                printf("Enter Book ID to mark as unread: ");
                scanf("%d", &bookId);
                clearInputBuffer();
                markBookAsUnread(bookId);
                break;
            case 12:
                printf("Backup Menu:\n");
                printf("1. Backup database\n");
                printf("2. Restore database\n");
                printf("0. Cancel\n");
                printf("Choose an option: ");
                int backupOption;
                scanf("%d", &backupOption);
                clearInputBuffer();

                switch (backupOption) {
                    case 1:
                        backupDatabase();
                        break;
                    case 2:
                        restoreDatabase();
                        break;
                    case 0:
                        printf("Backup cancelled.\n");
                        break;
                    default:
                        printf("Invalid backup option.\n");
                }
                break;
            case 13:
                printf("Database Statistics:\n");
                printDatabaseSize();
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
