#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Resource
{
    int id;
    char name[50];
    char type[30];
    char location[50];
    char phone[20];
    int priority;
};

struct Resource resource[MAX];

int totalResource = 0;
int welcome();
int menu();
int addResource();
int displayResource();
int searchResource();
int updateResource();
int deleteResource();
int sortByPriority();
int addRequest();
int showRequest();
int graphDemo();
int treeDemo();
int inorder(int root);
int dfs(int node);

struct Request
{
    int id;
    char patientName[50];
    char need[50];
    char location[50];

    struct Request *next;
};

struct Request *head = NULL;
struct Request *tail = NULL;

char hospitalName[5][30] =
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
int main()
{
    welcome();
    menu();
    return 0;
}

int welcome()
{
    printf("\n");
    printf("=====================================================\n");
    printf("            SEREN LITE\n");
    printf(" Smart Emergency Resource Exchange Network\n");
    printf("=====================================================\n");
    printf(" Version : 1.0\n");
    printf(" Platform: VS Code (GCC)\n");
    printf("=====================================================\n");
    return 0;
}

int menu()
{
    int choice;
    while (1)
    {
        printf("\n");
        printf("============= MAIN MENU =============\n");
        printf("1. Add Resource\n");
        printf("2. Display Resource\n");
        printf("3. Search Resource\n");
        printf("4. Update Resource\n");
        printf("5. Delete Resource\n");
        printf("6. Sort By Priority\n");
        printf("7. Emergency Request\n");
        printf("8. Show Requests\n");
        printf("9. Hospital Network\n");
        printf("10. Area Hierarchy\n");
        printf("0. Exit\n");
        printf("=====================================\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice)
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
            updateResource();
            break;
        case 5:
            deleteResource();
            break;
        case 6:
            sortByPriority();
            break;
        case 7:
            addRequest();
            break;
        case 8:
            showRequest();
            break;
        case 9:
            graphDemo();
            break;
        case 10:
            treeDemo();
            break;
        case 0:
            printf("\nThank You For Using SEREN Lite.\n");
            return 0;

        default:
            printf("\nInvalid Choice! Please Try Again.\n");
        }
    }
}

int addResource()
{
    if(totalResource >= MAX)
    {
        printf("\nStorage is Full!\n");
        return 0;
    }

    printf("\n========== ADD RESOURCE ==========\n");
    printf("Enter Resource ID : ");
    scanf("%d", &resource[totalResource].id);

    getchar();

    printf("Enter Resource Name : ");
    fgets(resource[totalResource].name, sizeof(resource[totalResource].name), stdin);
    resource[totalResource].name[strcspn(resource[totalResource].name, "\n")] = '\0';

    printf("Enter Resource Type : ");
    fgets(resource[totalResource].type, sizeof(resource[totalResource].type), stdin);
    resource[totalResource].type[strcspn(resource[totalResource].type, "\n")] = '\0';

    printf("Enter Location : ");
    fgets(resource[totalResource].location, sizeof(resource[totalResource].location), stdin);
    resource[totalResource].location[strcspn(resource[totalResource].location, "\n")] = '\0';

    printf("Enter Phone Number : ");
    fgets(resource[totalResource].phone, sizeof(resource[totalResource].phone), stdin);
    resource[totalResource].phone[strcspn(resource[totalResource].phone, "\n")] = '\0';

    printf("Enter Priority (1-10) : ");
    scanf("%d", &resource[totalResource].priority);

    totalResource++;
    printf("\nResource Added Successfully.\n");
    return 0;
}

int displayResource()
{
    int i;
    if(totalResource == 0)
    {
        printf("\nNo Resource Available.\n");
        return 0;
    }

    printf("\n========== RESOURCE LIST ==========\n");

    for(i = 0; i < totalResource; i++)
    {
        printf("\n----------------------------------\n");
        printf("Resource ID : %d\n", resource[i].id);
        printf("Name        : %s\n", resource[i].name);
        printf("Type        : %s\n", resource[i].type);
        printf("Location    : %s\n", resource[i].location);
        printf("Phone       : %s\n", resource[i].phone);
        printf("Priority    : %d\n", resource[i].priority);
    }

    printf("\n==================================\n");
    printf("Total Resource : %d\n", totalResource);
    return 0;
}

int searchResource()
{
    char searchName[50];
    int i;
    int found = 0;
    if(totalResource == 0)
    {
        printf("\nNo Resource Available.\n");
        return 0;
    }

    getchar();

    printf("\nEnter Resource Name : ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    for(i = 0; i < totalResource; i++)
    {
        if(strcmp(resource[i].name, searchName) == 0)
        {
            printf("\n========== RESOURCE FOUND ==========\n");
            printf("Resource ID : %d\n", resource[i].id);
            printf("Name        : %s\n", resource[i].name);
            printf("Type        : %s\n", resource[i].type);
            printf("Location    : %s\n", resource[i].location);
            printf("Phone       : %s\n", resource[i].phone);
            printf("Priority    : %d\n", resource[i].priority);

            found = 1;
            break;
        }
    }

    if(found == 0)
    {
        printf("\nResource Not Found.\n");
    }
    return 0;
}

int updateResource()
{
    int id;
    int i;
    int found = 0;
    if(totalResource == 0)
    {
        printf("\nNo Resource Available.\n");
        return 0;
    }
    printf("\nEnter Resource ID : ");
    scanf("%d", &id);

    getchar();

    for(i = 0; i < totalResource; i++)
    {
        if(resource[i].id == id)
        {
            found = 1;
            printf("\n----- Current Information -----\n");
            printf("Name     : %s\n", resource[i].name);
            printf("Type     : %s\n", resource[i].type);
            printf("Location : %s\n", resource[i].location);
            printf("Phone    : %s\n", resource[i].phone);
            printf("Priority : %d\n", resource[i].priority);
            printf("\n----- Enter New Information -----\n");
            printf("New Name : ");
            fgets(resource[i].name, sizeof(resource[i].name), stdin);
            resource[i].name[strcspn(resource[i].name, "\n")] = '\0';

            printf("New Type : ");
            fgets(resource[i].type, sizeof(resource[i].type), stdin);
            resource[i].type[strcspn(resource[i].type, "\n")] = '\0';

            printf("New Location : ");
            fgets(resource[i].location, sizeof(resource[i].location), stdin);
            resource[i].location[strcspn(resource[i].location, "\n")] = '\0';

            printf("New Phone : ");
            fgets(resource[i].phone, sizeof(resource[i].phone), stdin);
            resource[i].phone[strcspn(resource[i].phone, "\n")] = '\0';

            printf("New Priority : ");
            scanf("%d", &resource[i].priority);
            printf("\nResource Updated Successfully.\n");

            break;
        }
    }

    if(found == 0)
    {
        printf("\nResource ID Not Found.\n");
    }
    return 0;
}

int deleteResource()
{
    int id;
    int i;
    int found = 0;
    if(totalResource == 0)
    {
        printf("\nNo Resource Available.\n");
        return 0;
    }

    printf("\nEnter Resource ID to Delete : ");
    scanf("%d", &id);

    for(i = 0; i < totalResource; i++)
    {
        if(resource[i].id == id)
        {
            found = 1;
            int j;
            for(j = i; j < totalResource - 1; j++)
            {
                resource[j] = resource[j + 1];
            }
            totalResource--;
            printf("\nResource Deleted Successfully.\n");
            break;
        }
    }

    if(found == 0)
    {
        printf("\nResource ID Not Found.\n");
    }
    return 0;
}

int sortByPriority()
{
    int i, j;
    struct Resource temp;
    if(totalResource == 0)
    {
        printf("\nNo Resource Available.\n");
        return 0;
    }
    for(i = 0; i < totalResource - 1; i++)
    {
        for(j = 0; j < totalResource - i - 1; j++)
        {
            if(resource[j].priority < resource[j + 1].priority)
            {
                temp = resource[j];
                resource[j] = resource[j + 1];
                resource[j + 1] = temp;
            }
        }
    }

    printf("\nResources Sorted Successfully.\n");
    printf("Highest Priority Resources Are Now On Top.\n");
    return 0;
}

int addRequest()
{
    struct Request *newNode;
    newNode = (struct Request *)malloc(sizeof(struct Request));

    if(newNode == NULL)
    {
        printf("\nMemory Allocation Failed.\n");
        return 0;
    }

    printf("\n========== EMERGENCY REQUEST ==========\n");
    printf("Enter Request ID : ");
    scanf("%d", &newNode->id);

    getchar();

    printf("Patient Name : ");
    fgets(newNode->patientName, sizeof(newNode->patientName), stdin);
    newNode->patientName[strcspn(newNode->patientName, "\n")] = '\0';

    printf("Emergency Need : ");
    fgets(newNode->need, sizeof(newNode->need), stdin);
    newNode->need[strcspn(newNode->need, "\n")] = '\0';

    printf("Location : ");
    fgets(newNode->location, sizeof(newNode->location), stdin);
    newNode->location[strcspn(newNode->location, "\n")] = '\0';

    newNode->next = NULL;

    if(head == NULL)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
    printf("\nEmergency Request Added Successfully.\n");
    return 0;
}

int showRequest()
{
    struct Request *temp;
    if(head == NULL)
    {
        printf("\nNo Emergency Request Found.\n");
        return 0;
    }

    temp = head;
    printf("\n========== EMERGENCY REQUEST LIST ==========\n");
    while(temp != NULL)
    {
        printf("\n-----------------------------------------\n");
        printf("Request ID    : %d\n", temp->id);
        printf("Patient Name  : %s\n", temp->patientName);
        printf("Need          : %s\n", temp->need);
        printf("Location      : %s\n", temp->location);

        temp = temp->next;
    }

    printf("\n===========================================\n");
    return 0;
}

int graphDemo()
{
    int i;
    for(i=0;i<5;i++)
    {
        visited[i]=0;
    }
    printf("\nHospital Network (DFS)\n");
    printf("-------------------------\n");
    dfs(0);
    printf("\n");
    return 0;
}

int dfs(int node)
{
    int i;
    visited[node] = 1;
    printf("%s\n", hospitalName[node]);
    for(i=0;i<5;i++)
    {
        if(graph[node][i]==1 && visited[i]==0)
        {
            dfs(i);
        }
    }
    return 0;
}

char *areaTree[7] =
{
    "Bangladesh",
    "Dhaka",
    "Chattogram",
    "Savar",
    "Mirpur",
    "Agrabad",
    "Pahartali"
};

int inorder(int root)
{
    if(root >= 7)
    {
        return 0;
    }
    inorder((2 * root) + 1);
    printf("%s\n", areaTree[root]);
    inorder((2 * root) + 2);
    return 0;
}

int treeDemo()
{
    printf("\nArea Hierarchy (Inorder Traversal)\n");
    printf("---------------------------------\n\n");
    inorder(0);
    return 0;
}
