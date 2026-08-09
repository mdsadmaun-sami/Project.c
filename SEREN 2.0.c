#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

struct Resource
{
    int id;
    char name[40];
    char type[25];
    char location[40];
    char phone[20];
    int priority;
};

struct Request
{
    int id;
    char patient[40];
    char need[40];
    char location[40];
    struct Request *next;
};

struct Resource resource[MAX];
struct Request *head = NULL, *tail = NULL;

int totalResource = 0;

char *hospital[5] =
{
    "Central Hospital",
    "City Medical",
    "Savar General",
    "Life Care",
    "Emergency Blood Bank"
};

int graph[5][5] =
{
    {0,1,1,0,0},
    {1,0,0,1,0},
    {1,0,0,1,1},
    {0,1,1,0,1},
    {0,0,1,1,0}
};

int visited[5];

void addResource()
{
    if(totalResource >= MAX)
    {
        printf("\nStorage Full!\n");
        return;
    }

    printf("\n===== ADD RESOURCE =====\n");

    printf("Resource ID: ");
    scanf("%d", &resource[totalResource].id);
    getchar();

    printf("Resource Name: ");
    fgets(resource[totalResource].name, 40, stdin);
    resource[totalResource].name[strcspn(resource[totalResource].name,"\n")] = 0;

    printf("Resource Type: ");
    fgets(resource[totalResource].type, 25, stdin);
    resource[totalResource].type[strcspn(resource[totalResource].type,"\n")] = 0;

    printf("Location: ");
    fgets(resource[totalResource].location, 40, stdin);
    resource[totalResource].location[strcspn(resource[totalResource].location,"\n")] = 0;

    printf("Phone: ");
    fgets(resource[totalResource].phone, 20, stdin);
    resource[totalResource].phone[strcspn(resource[totalResource].phone,"\n")] = 0;

    printf("Priority (1-10): ");
    scanf("%d", &resource[totalResource].priority);

    totalResource++;
    printf("\nResource Added Successfully!\n");
}

void displayResource()
{
    int i;
    if(totalResource == 0)
    {
        printf("\nNo Resource Available.\n");
        return;
    }
    printf("\n===== AVAILABLE RESOURCES =====\n");
    for(i = 0; i < totalResource; i++)
    {
        printf("\nID       : %d", resource[i].id);
        printf("\nName     : %s", resource[i].name);
        printf("\nType     : %s", resource[i].type);
        printf("\nLocation : %s", resource[i].location);
        printf("\nPhone    : %s", resource[i].phone);
        printf("\nPriority : %d\n", resource[i].priority);
    }
}

void searchResource()
{
    char name[40];
    int i, found = 0;
    if(totalResource == 0)
    {
        printf("\nNo Resource Available.\n");
        return;
    }
    getchar();

    printf("\nEnter Resource Name: ");
    fgets(name, 40, stdin);
    name[strcspn(name,"\n")] = 0;
    for(i = 0; i < totalResource; i++)
    {
        if(strcmp(resource[i].name, name) == 0)
        {
            printf("\n===== RESOURCE FOUND =====\n");
            printf("ID       : %d\n", resource[i].id);
            printf("Name     : %s\n", resource[i].name);
            printf("Type     : %s\n", resource[i].type);
            printf("Location : %s\n", resource[i].location);
            printf("Phone    : %s\n", resource[i].phone);
            printf("Priority : %d\n", resource[i].priority);
            found = 1;
            break;
        }
    }
    if(found == 0)
    {
        printf("\nResource Not Found.\n");
    }
}

void addRequest()
{
    struct Request *newNode;
    newNode = malloc(sizeof(struct Request));
    if(newNode == NULL)
    {
        printf("\nMemory Allocation Failed!\n");
        return;
    }

    printf("\n===== EMERGENCY REQUEST =====\n");
    printf("Request ID: ");
    scanf("%d", &newNode->id);
    getchar();

    printf("Patient Name: ");
    fgets(newNode->patient, 40, stdin);
    newNode->patient[strcspn(newNode->patient,"\n")] = 0;

    printf("Emergency Need: ");
    fgets(newNode->need, 40, stdin);
    newNode->need[strcspn(newNode->need,"\n")] = 0;

    printf("Location: ");
    fgets(newNode->location, 40, stdin);
    newNode->location[strcspn(newNode->location,"\n")] = 0;

    newNode->next = NULL;

    if(head == NULL)
        head = tail = newNode;
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
    printf("\nEmergency Request Added Successfully!\n");
}

void dfs(int node)
{
    int i;
    visited[node] = 1;
    printf(" -> %s", hospital[node]);
    for(i = 0; i < 5; i++)
    {
        if(graph[node][i] && !visited[i])
            dfs(i);
    }
}

void hospitalNetwork()
{
    int i;
    for(i = 0; i < 5; i++)
    {
        visited[i] = 0;
    }
    printf("\n===== HOSPITAL NETWORK =====\n");
    printf("Emergency Route using DFS:\n");
    dfs(0);
    printf("\n");
}

int main()
{
    int choice;
    printf("\n                *SEREN*\n");
    printf(" Smart Emergency Resource Exchange Network\n");
    printf("============================================\n");
    while(1)
    {
        printf("\n*Main Menu\n");
        printf("1. Add Resource\n");
        printf("2. Display Resources\n");
        printf("3. Search Resource\n");
        printf("4. Emergency Request\n");
        printf("5. Hospital Network (DFS)\n");
        printf("0. Exit\n");
        printf("---------------------------------------\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                addResource();
                break;

            case 2:
                displayResource();
                break;

            case 3:
                searchResource();
                break;

            case 4:
                addRequest();
                break;

            case 5:
                hospitalNetwork();
                break;

            case 0:
                printf("\nThank You For Using SEREN Lite!\n");
                return 0;

            default:
                printf("\nInvalid Choice!\n");
        }
    }
}
