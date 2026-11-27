#include <stdio.h>
#include <string.h>

struct Patient {
    int id;
    char name[50], phone[20];
};

struct Doctor {
    int id;
    char name[50], specialty[30];
};

struct Appointment {
    int appId, patientId, doctorId;
    char date[15], time[10], status[15];
};

void registerPatient(void)
{
    FILE *fp;
    struct Patient p;

    printf("Patient ID: ");
    scanf("%d", &p.id);
    printf("Name: ");
    scanf(" %49[^\n]", p.name);
    printf("Phone: ");
    scanf("%19s", p.phone);

    fp = fopen("patients.txt", "a");
    if (fp == NULL) return;

    fprintf(fp, "%d|%s|%s\n", p.id, p.name, p.phone);
    fclose(fp);

    printf("Registration successful!\n");
}

void viewDoctors(void)
{
    FILE *fp = fopen("doctors.txt", "r");
    struct Doctor d;

    if (fp == NULL) {
        printf("No doctor record found.\n");
        return;
    }

    printf("\nID\tName\t\tSpecialty\n");

    while (fscanf(fp, "%d|%49[^|]|%29[^\n]",
                  &d.id, d.name, d.specialty) == 3)
        printf("%d\t%s\t\t%s\n", d.id, d.name, d.specialty);

    fclose(fp);
}

void bookAppointment(int patientId)
{
    FILE *fp;
    struct Appointment a, old;
    int found = 0;

    viewDoctors();

    a.patientId = patientId;

    printf("\nAppointment ID: ");
    scanf("%d", &a.appId);
    printf("Doctor ID: ");
    scanf("%d", &a.doctorId);
    printf("Date: ");
    scanf("%14s", a.date);
    printf("Time: ");
    scanf("%9s", a.time);

    strcpy(a.status, "Booked");

    fp = fopen("appointments.txt", "a+");
    if (fp == NULL) return;

    rewind(fp);

    while (fscanf(fp, "%d|%d|%d|%14[^|]|%9[^|]|%14[^\n]",
                  &old.appId, &old.patientId, &old.doctorId,
                  old.date, old.time, old.status) == 6) {

        if (old.doctorId == a.doctorId &&
            strcmp(old.date, a.date) == 0 &&
            strcmp(old.time, a.time) == 0 &&
            strcmp(old.status, "Booked") == 0) {
            found = 1;
            break;
        }
    }

    if (found)
        printf("This slot is already booked!\n");
    else {
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "%d|%d|%d|%s|%s|%s\n",
                a.appId, a.patientId, a.doctorId,
                a.date, a.time, a.status);
        printf("Appointment booked!\n");
    }

    fclose(fp);
}

void viewHistory(int patientId)
{
    FILE *fp = fopen("appointments.txt", "r");
    struct Appointment a;
    int found = 0;

    if (fp == NULL) {
        printf("No appointment found.\n");
        return;
    }

    printf("\nApp ID\tDoctor ID\tDate\t\tTime\tStatus\n");

    while (fscanf(fp, "%d|%d|%d|%14[^|]|%9[^|]|%14[^\n]",
                  &a.appId, &a.patientId, &a.doctorId,
                  a.date, a.time, a.status) == 6) {

        if (a.patientId == patientId) {
            printf("%d\t%d\t\t%s\t%s\t%s\n",
                   a.appId, a.doctorId, a.date, a.time, a.status);
            found = 1;
        }
    }

    if (!found) printf("No appointment history.\n");

    fclose(fp);
}

void patientMenu(void)
{
    FILE *fp;
    struct Patient p;
    int id, choice, found;

    printf("\n1. Register\n2. Enter Patient ID\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        registerPatient();
        return;
    }

    if (choice != 2) return;

    printf("Enter Patient ID: ");
    scanf("%d", &id);

    fp = fopen("patients.txt", "r");
    if (fp == NULL) {
        printf("Please register first.\n");
        return;
    }

    found = 0;

    while (fscanf(fp, "%d|%49[^|]|%19[^\n]",
                  &p.id, p.name, p.phone) == 3) {
        if (p.id == id) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Patient ID not found.\n");
        return;
    }

    do {
        printf("\n--- PATIENT MENU ---\n");
        printf("1. View Doctors\n2. Book Appointment\n");
        printf("3. View Appointment History\n0. Back\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) viewDoctors();
        else if (choice == 2) bookAppointment(id);
        else if (choice == 3) viewHistory(id);
        else if (choice != 0) printf("Wrong choice!\n");

    } while (choice != 0);
}

int main(void)
{
    patientMenu();
    return 0;
}
