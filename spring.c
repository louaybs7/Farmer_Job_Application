
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_WORKERS_PER_DAY 3
// Function to print the contents of the file with all workers and their days of work
void printFileContents(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", fileName);
        return;
    }

    char currentLine[100];

    while (fgets(currentLine, 100, file) != NULL)
    {
        printf("%s", currentLine);
    }

    fclose(file);
}
// take a file name and a worker name to delete
void deleteLineFromFile(char *fileName, char *nameToDelete)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", fileName);
        return;
    }

    // Create a temporary file to write the modified contents
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        printf("Failed to create temporary file\n");
        fclose(file);
        return;
    }

    char currentLine[100];
    int lineFound = 0;

    while (fgets(currentLine, 100, file) != NULL)
    {
        if (strstr(currentLine, nameToDelete) == currentLine)
        {
            lineFound = 1;
        }
        else
        {
            fprintf(tempFile, "%s", currentLine);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (lineFound)
    {
        remove(fileName);
        rename("temp.txt", fileName);
        
    }
    else
    {
        remove("temp.txt");
        printf("Line with name '%s' not found in file %s\n", nameToDelete, fileName);
    }
}
// add a worker to the file using name and day of work
void addWorkerToFile(char *fileName, char *workerName, char *day)
{
    FILE *file = fopen(fileName, "a");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", fileName);
        return;
    }

    // Check if the name exist before
    char currentLine[100];
    while (fgets(currentLine, 100, file) != NULL)
    {
        if (strstr(currentLine, workerName) != NULL)
        {
            printf("Worker %s is already present in the file\n", workerName);
            fclose(file);
            return;
        }
    }

    // Add the new worker to the file
    fprintf(file, "%s %s\n", workerName, day);
    printf("Worker %s added to file %s\n", workerName, fileName);

    fclose(file);
}
// a function to count the number of workers in a given day
int countWorkers(char *fileName, char *word)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file '%s' for reading.\n", fileName);
        return -1;
    }

    int count = 0;
    char line[100];

    while (fgets(line, sizeof(line), file))
    {
        char *p = line;
        while ((p = strstr(p, word)))
        {
            count++;
            p += strlen(word);
        }
    }

    fclose(file);
    return count;
}
// This function is used to check if the number of workers in a given day is greater than the maximum number of workers allowed
int addAndCheck(char *day, int max)
{
#define MAX_WORKERS_PER_DAY 3
    char input_copy[100];
    strcpy(input_copy, day);

    // Split the input string into words using strtok
    char *token = strtok(input_copy, " ");
    while (token != NULL)
    {
        // check number of workers for each day if its >= max
        int count = countWorkers("workers.txt", token);
        if (count >= max)
        {
            return 1;
        }
        token = strtok(NULL, " ");
    }

    return 0;
}

// Main function
int main()
{
    char fileName[] = "workers.txt";
    int choice;
    char name[100];
    char day[100];
    char lineToDelete[100];

    while (1)
    {

        printf("Enter your choice:\n");
        printf("----------------------------------\n");

        printf("0- Add new worker\n1- Delete worker \n2- Print all workers\n3- Exit program\n4- Delete all workers\n5-Change worker days\n");
        printf("\n");
        printf("----------------------------------\n");

        scanf("%d", &choice);
        getchar(); // consume the newline character left in the input buffer

        switch (choice)
        {
        case 0:

            printf("Enter worker name: ");
            fgets(name, 100, stdin);
            name[strcspn(name, "\n")] = 0; // remove the newline character after the name  at the end
            printf("Enter the days of the week separated by spaces (e.g. Monday Wednesday Thursday): ");
            fgets(day, 100, stdin);
            day[strcspn(day, "\n")] = 0; // remove the newline character after the day  at the end
            if (addAndCheck(day, MAX_WORKERS_PER_DAY) == 1)
            {
                printf("================================\n");
                printf("Sorry we can not accept you \n");
                printf("================================\n ");
                printf("\n");
                break;
            }
            else
            {
                addWorkerToFile(fileName, name, day);
            }
            break;

        case 1:
            printf("Enter worker name to delete: ");
            fgets(name, 100, stdin);
            name[strcspn(name, "\n")] = 0; // remove the newline character at the end of the taken name from input
            deleteLineFromFile(fileName, name);

            break;
        case 2:
            printf("=====================Workers Registered===============\n");
            printFileContents(fileName);
            printf("======================================================\n");
            break;
        case 3:
            exit(0);
            break;
        case 4:
            // Clear the contents of the file
            fclose(fopen(fileName, "w"));
            printf("All workers deleted from file %s\n", fileName);
            break;
        case 5:
            // edit the list throw delete then add
            printf("Enter worker name to Edit: ");
            fgets(name, 100, stdin);
            name[strcspn(name, "\n")] = 0;
            deleteLineFromFile(fileName, name);
            printf("Enter the days of the week separated by spaces(e.g. Monday Wednesday Thursday):");
            fgets(day, 100, stdin);
            day[strcspn(day, "\n")] = 0;
            if (addAndCheck(day, MAX_WORKERS_PER_DAY) == 1)
            {
                printf("================================\n");
                printf("Sorry we can not accept you \n");
                printf("================================\n ");
                printf("\n");
                break;
            }
            else
            {
                addWorkerToFile(fileName, name, day);
            }

            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }
    return 0;
}
