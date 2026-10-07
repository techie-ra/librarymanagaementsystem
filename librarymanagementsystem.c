#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct library
{
    char title[50];
    char author[50];
    char ISBN[50];
    int available;
    char borrow[10];
};
void addBook();
void searchBooks();
void returnBooks();
void borrowBooks();
void displayBooks();
int main()
{
    int choice;
    while (1)
    {
        printf("\nLibraray Management system Menu\n");
        printf("1. Add Book\n");
        printf("2. search Book\n");
        printf("3. borrow Book\n");
        printf("4.return book\n");
        printf("5. Display all Books\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input.\n");
            break;
        }

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            searchBooks();
            break;
        case 3:
            borrowBooks();
            break;
        case 4:
            returnBooks();
            break;
        case 5:
            displayBooks();
            break;
        case 6:
            exit(0);
            break;
        default:
            printf("\nInvalid choice! Please choose between 1 and 5.\n");
        }
    }
    return 0;
}
void addBook()
{
    struct library Book;
    FILE *ptr;
    ptr = fopen("library.txt", "ab");
    if (ptr == NULL)
    {
        printf("\nError opening file!\n");
        return;
    }
    printf("\n========== ADD BOOK ==========\n");
    printf("Enter book title: \n");
    fgets(Book.title, sizeof(Book.title), stdin);
    Book.title[strcspn(Book.title, "\n")] = '\0';
    printf("Enter author name: \n");
    fgets(Book.author, sizeof(Book.author), stdin);
    Book.author[strcspn(Book.author, "\n")] = '\0';
    printf("Enter ISBN: ");
    fgets(Book.ISBN, sizeof(Book.ISBN), stdin);
    Book.ISBN[strcspn(Book.ISBN, "\n")] = '\0';
    Book.available = 1;
    strcpy(Book.borrow, "None");
    fwrite(&Book, sizeof(Book), 1, ptr);
    fclose(ptr);
    printf("\nBook added successfully!\n");
}
void displayBooks()
{
    FILE *ptr = fopen("library.txt", "rb");
    if (ptr == NULL)
    {
        printf("No Books found!\n");
        return;
    }
    struct library Book;
    printf("%s %s %s %s %s\n", "Title", "Author", "ISBN", "status");
    while (fread(&Book, sizeof(Book), 1, ptr))
    {
        printf("%s %s %s %s %s\n", Book.title, Book.author, Book.ISBN, Book.available ? "Available" : "Borrow", Book.borrow);
    }
    fclose(ptr);
}
void searchBooks()
{
    FILE *ptr = fopen("library.txt", "rb");
    if (ptr == NULL)
    {
        printf("No books found!\n");
        return;
    }
    char key[100];
    printf("Enter title/author/ISBN to search: ");
    fgets(key, 100, stdin);
    key[strcspn(key, "\n")] = 0;

    struct library Book;
    int found = 0;
    while (fread(&Book, sizeof(Book), 1, ptr))
    {
        if (strstr(Book.title, key) || strstr(Book.author, key) || strstr(Book.ISBN, key))
        {
            printf("\nFound Book:\n");
            printf("Title: %s\nAuthor: %s\nISBN: %s\nStatus: %s\nBorrower: %s\n",
                   Book.title, Book.author, Book.ISBN,
                   Book.available ? "Available" : "Borrowed",
                   Book.borrow);
            found = 1;
        }
    }
    if (!found)
        printf("No matching book found.\n");
    fclose(ptr);
}
void borrowBooks()
{
    FILE *ptr = fopen("library.txt", "rb");
    if (ptr == NULL)
    {
        printf("No books found!\n");
        return;
    }

    char key[20];
    printf("Enter ISBN of book to borrow: ");
    fgets(key, 20, stdin);
    key[strcspn(key, "\n")] = 0;

    struct library Book;
    int found = 0;
    while (fread(&Book, sizeof(Book), 1, ptr))
    {
        if (strcmp(Book.ISBN, key) == 0)
        {
            found = 1;
            if (Book.available)
            {
                printf("Enter borrower name: ");
                fgets(Book.borrow, 100, stdin);
                Book.borrow[strcspn(Book.borrow, "\n")] = 0;
                Book.available = 0;
                fseek(ptr, -sizeof(Book), SEEK_CUR);
                fwrite(&Book, sizeof(Book), 1, ptr);
                printf("Book borrowed successfully!\n");
            }
            else
            {
                printf("Book is already borrowed by %s.\n", Book.borrow);
            }
            break;
        }
    }
    if (!found)
        printf("Book not found.\n");
    fclose(ptr);
}

void returnBooks()
{
    FILE *ptr = fopen("library.txt", "rb+");
    if (ptr == NULL)
    {
        printf("No books found!\n");
        return;
    }

    char key[20];
    printf("Enter ISBN of book to return: ");
    fgets(key, 20, stdin);
    key[strcspn(key, "\n")] = 0;

    struct library Book;
    int found = 0;
    while (fread(&Book, sizeof(Book), 1, ptr))
    {
        if (strcmp(Book.ISBN, key) == 0)
        {
            found = 1;
            if (!Book.available)
            {
                Book.available = 1;
                strcpy(Book.borrow, "None");
                fseek(ptr, -sizeof(Book), SEEK_CUR);
                fwrite(&Book, sizeof(Book), 1, ptr);
                printf("Book returned successfully!\n");
            }
            else
            {
                printf("Book is already available.\n");
            }
            break;
        }
    }
    if (!found)
        printf("Book not found.\n");
    fclose(ptr);
}