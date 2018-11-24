#include "assign7.h"

int main( ) {
    int go = 1;
    int scanf;
    int value;
    char buffer[10];
    char choice[1];

    while (go) {
        printf("Enter one of the following options or press CONTROL-D to exit.\n");
        printf("C - create a new item\n");
        printf("R - read an existing item\n");
        printf("U - update an existing item\n");
        printf("D - delete an existing item\n");


        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
            break;

        scanf = sscanf(buffer, "%s", choice);
        value = switchCase(choice);
    }

    return 0;
}

int switchCase(char choice[ ]) {
    switch (*choice) {
        case 'C':
        case 'c':
            C();
            break;
        case 'R':
        case 'r':
            R();
            break;
        case 'U':
        case 'u':
            U();
            break;
        case 'D':
        case 'd':
            D();
            break;
        default:
            printf("ERROR: invalid option\n");
            break;
    }
    return 0;
}

int C( ) {
    char buffer[BUFFER_SIZE];
    long lRBA;
    int itemNumber;
    int seek;
    int write;
    int read;
    Item newItem;
    Item checkItem;

    inventoryFile = fopen(inventoryFileName, "rb+");
    if (inventoryFile == NULL)
        inventoryFile = fopen(inventoryFileName, "wb");

    printf("Enter an item number: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%d", &itemNumber);
    fflush(stdin);

    lRBA = itemNumber * sizeof(Item);
    seek = fseek(inventoryFile, lRBA, SEEK_SET);
    read = fread(&checkItem, sizeof(Item), 1L, inventoryFile);
    if (read == 1 && checkItem.simpleName[0] != '\0') {
        printf("ERROR: item already exists\n");
        return 0;
    }

    printf("Enter a simple name: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%[^\t\n]", newItem.simpleName);
    fflush(stdin);

    printf("Enter an item name: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%[^\t\n]", newItem.itemName);
    fflush(stdin);

    printf("Enter a current quantity: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%d", &newItem.currentQuantity);
    fflush(stdin);

    printf("Enter a max quantity: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%d", &newItem.maxQuantity);
    fflush(stdin);

    printf("Enter a description: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%[^\t\n]", newItem.body);
    fflush(stdin);

    seek = fseek(inventoryFile, lRBA, SEEK_SET);
    write = fwrite(&newItem, sizeof(newItem), 1L, inventoryFile);

    fclose(inventoryFile);
    return 0;
}

int R( ) {
    char buffer[BUFFER_SIZE];
    int itemNumber;
    int seek;
    int read;
    int open;
    long lRBA;
    Item item;

    printf("Enter an item number: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%d", &itemNumber);
    fflush(stdin);

    lRBA = itemNumber * sizeof(Item);
    inventoryFile = fopen(inventoryFileName, "rb");
    seek = fseek(inventoryFile, lRBA, SEEK_SET);
    read = fread(&item, sizeof(Item), 1L, inventoryFile);
    if (read != 1 || item.itemName[0] == '\0') {
        printf("ERROR: item not found\n");
        return 0;
    }

    printf("Item name: %s\n", item.itemName);
    printf("Simple name: %s\n", item.simpleName);
    printf("Item number: %d\n", itemNumber);
    printf("Qty: %d/%d\n", item.currentQuantity, item.maxQuantity);
    printf("Description: %s\n", item.body);

    fclose(inventoryFile);
    return 0;
}

int U( ) {
    char buffer[150];
    int itemNumber;
    Item updatedItem;
    Item checkItem;
    long lRBA;
    int seek;
    int read;
    int write;

    inventoryFile = fopen(inventoryFileName, "rb+");
    if (inventoryFile == NULL)
        inventoryFile = fopen(inventoryFileName, "wb");

    printf("Enter an item number: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%d", &itemNumber);
    fflush(stdin);

    lRBA = itemNumber * sizeof(Item);
    seek = fseek(inventoryFile, lRBA, SEEK_SET);
    read = fread(&checkItem, sizeof(Item), 1L, inventoryFile);
    if (read == 0 || checkItem.itemName[0] == '\0') {
        printf("ERROR: item not found\n");
        return 0;
    }

    printf("Enter a simple name: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%[^\t\n]", updatedItem.simpleName);
    if (buffer[0] == '\n')
        strcpy(updatedItem.simpleName, checkItem.simpleName);
    fflush(stdin);

    printf("Enter an item name: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%[^\t\n]", updatedItem.itemName);
    if (buffer[0] == '\n')
        strcpy(updatedItem.itemName, checkItem.itemName);
    fflush(stdin);

    printf("Enter a current quantity: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%d", &updatedItem.currentQuantity);
    if (buffer[0] == '\n')
        updatedItem.currentQuantity = checkItem.currentQuantity;
    fflush(stdin);

    printf("Enter a maximum quantity: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%d", &updatedItem.maxQuantity);
    if (buffer[0] == '\n')
        updatedItem.maxQuantity = checkItem.maxQuantity;
    fflush(stdin);

    printf("Enter a description: \n");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%[^\t\n]", updatedItem.body);
    if (buffer[0] == '\n')
        strcpy(updatedItem.body, checkItem.body);
    fflush(stdin);

    seek = fseek(inventoryFile, lRBA, SEEK_SET);
    write = fwrite(&updatedItem, sizeof(Item), 1L, inventoryFile);

    return 0;
}

int D() {
    char buffer[150];
    int itemNumber;
    int quantity;
    int seek;
    int write;
    int read;
    long lRBA;
    Item deleteItem;
    Item checkItem;

    inventoryFile = fopen(inventoryFileName, "rb+");
    if (inventoryFile == NULL) {
        printf("ERROR: file not found\n");
        return 0;
    }

    printf("Enter an item number: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    sscanf(buffer, "%d", &itemNumber);
    fflush(stdin);

    lRBA = itemNumber * sizeof(Item);
    seek = fseek(inventoryFile, lRBA, SEEK_SET);
    read = fread(&checkItem, sizeof(Item), 1L, inventoryFile);
    if (read == 0 || checkItem.itemName[0] == '\0') {
        printf("ERROR: item not found\n");
        return 0;
    }

    strcpy(deleteItem.simpleName, checkItem.simpleName);
    strcpy(deleteItem.itemName, checkItem.itemName);
    deleteItem.currentQuantity = checkItem.currentQuantity;
    deleteItem.maxQuantity = 0;
    strcpy(deleteItem.body, checkItem.body);

    seek = fseek(inventoryFile, lRBA, SEEK_SET);
    write = fwrite(&deleteItem, sizeof(Item), 1L, inventoryFile);
    printf("%s was successfully deleted.\n", deleteItem.simpleName);

    return 0;
}
