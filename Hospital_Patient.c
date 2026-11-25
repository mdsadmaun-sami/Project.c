#include <stdio.h>
#include <string.h>

typedef struct 
{
    int id;
    char name[50], phone[20];
} Patient;

typedef struct 
{
    int id;
    char name[50], specialty[30];
} Doctor;

typedef struct 
{
    int appId, patientId, doctorId;
    char date[15], time[10], status[15];
} Appointment;

void registerPatient(void)
{
    FILE *fp;
    Patient p;
    printf("Patient ID: ");
    scanf("%d", &p.id);
    printf("Name: ");
    scanf(" %50[^\n]", p.name);
    printf("Phone: ");
    scanf("%13s", p.phone);
    fp = fopen("patients.dat", "ab");
    if (fp == NULL) 
    {
        printf("File error!\n");
        return;
    }
    fwrite(&p, sizeof(p), 1, fp);
    fclose(fp);
    printf("Registration successful!\n");
}

void viewDoctors(void)
{
    FILE *fp;
    Doctor d;
    fp = fopen("doctors.dat", "rb");
    if (fp == NULL) {
        printf("No doctor record found.\n");
        return;
    }
    printf("\nID\tName\t\tSpecialty\n");
    while (fread(&d, sizeof(d), 1, fp) == 1)
        printf("%d\t%s\t\t%s\n", d.id, d.name, d.specialty);

    fclose(fp);
}

void bookAppointment(int patientId)
{
    FILE *fp;
    Appointment a, old;
    viewDoctors();
    a.patientId = patientId;
    printf("\nAppointment ID: ");
    scanf("%d", &a.appId);
    printf("Doctor ID: ");
    scanf("%d", &a.doctorId);
    printf("Date (DD-MM-YYYY): ");
    scanf("%14s", a.date);
    printf("Time (HH:MM): ");
    scanf("%9s", a.time);
    strcpy(a.status, "Booked");

    fp = fopen("appointments.dat", "ab+");
    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    rewind(fp);
    while (fread(&old, sizeof(old), 1, fp) == 1) {
        if (old.doctorId == a.doctorId &&
            strcmp(old.date, a.date) == 0 &&
            strcmp(old.time, a.time) == 0 &&
            strcmp(old.status, "Booked") == 0) {
            printf("This slot is already booked!\n");
            fclose(fp);
            return;
        }
    }

    fseek(fp, 0, SEEK_END);
    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);
    printf("Appointment booked!\n");
}

void viewHistory(int patientId)
{
    FILE *fp;
    Appointment a;
    int found = 0;
    fp = fopen("appointments.dat", "rb");
    if (fp == NULL) {
        printf("No appointment found.\n");
        return;
    }
    printf("\nApp ID\tDoctor ID\tDate\t\tTime\tStatus\n");
    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.patientId == patientId) {
            printf("%d\t%d\t\t%s\t%s\t%s\n",
                   a.appId, a.doctorId, a.date, a.time, a.status);
            found = 1;
        }
    }

    if (!found)
        printf("No appointment history.\n");

    fclose(fp);
}

void patientMenu(void)
{
    int id, choice;
    Patient p;
    FILE *fp;

    printf("\n1. Register\n2. Enter Patient ID\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        registerPatient();
        return;
    }
    if (choice != 2)
        return;

    printf("Enter Patient ID: ");
    scanf("%d", &id);

    fp = fopen("patients.dat", "rb");
    if (fp == NULL) {
        printf("Please register first.\n");
        return;
    }

    while (fread(&p, sizeof(p), 1, fp) == 1) {
        if (p.id == id)
            break;
    }
    fclose(fp);

    if (p.id != id) {
        printf("Patient ID not found.\n");
        return;
    }

    do {
        printf("\n--- PATIENT MENU ---\n");
        printf("1. View Doctors\n2. Book Appointment\n");
        printf("3. View Appointment History\n0. Back\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1)
            viewDoctors();
        else if (choice == 2)
            bookAppointment(id);
        else if (choice == 3)
            viewHistory(id);
        else if (choice != 0)
            printf("Wrong choice!\n");

    } while (choice != 0);
}

/* For separate testing only.
   Remove this main() when joining with the group project. */
int main(void)
{
    patientMenu();
    return 0;
}
