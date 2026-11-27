#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Persistent File Storage Definitions */
#define PATIENTS_FILE     "patients.txt"
#define DOCTORS_FILE      "doctors.txt"
#define APPOINTMENTS_FILE "appointments.txt"
#define LOG_FILE          "audit_log.txt"

/* Structure Definitions */
typedef struct {
    int id;
    char name[50];
    char username[50];
    char password[50];
    char phone[20];
} Patient;

typedef struct {
    int id;
    char name[50];
    char username[50];
    char password[50];
    char specialty[50];
    char working_days[60];
    char working_hours[40];
} Doctor;

typedef struct {
    int appId;
    int patientId;
    int doctorId;
    char date[15];
    char time[30];
    char status[15];
} Appointment;

/* Helper: Read full file content into a string buffer for state tracking */
void get_file_summary(const char *filename, char *buffer, size_t buffer_size) {
    FILE *fp = fopen(filename, "r");
    char line[256];
    buffer[0] = '\0';

    if (fp == NULL) {
        snprintf(buffer, buffer_size, "(File empty or does not exist)");
        return;
    }

    size_t current_len = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Strip trailing newline for clean formatting */
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) > 0) {
            if (current_len > 0 && current_len + 3 < buffer_size) {
                strncat(buffer, " ; ", buffer_size - current_len - 1);
                current_len += 3;
            }
            strncat(buffer, line, buffer_size - current_len - 1);
            current_len = strlen(buffer);
        }
    }
    fclose(fp);

    if (strlen(buffer) == 0) {
        snprintf(buffer, buffer_size, "(No records)");
    }
}

/* Helper: Audit log file update states (Before and After) */
void log_file_update(const char *action, const char *before_state, const char *after_state) {
    FILE *logFp = fopen(LOG_FILE, "a");
    if (logFp != NULL) {
        fprintf(logFp, "=== ACTION: %s ===\n", action);
        fprintf(logFp, "[BEFORE UPDATION]: %s\n", before_state);
        fprintf(logFp, "[AFTER UPDATION ]: %s\n", after_state);
        fprintf(logFp, "-----------------------------------------------------\n");
        fclose(logFp);
    }
}

/* Helper: Initialize seed records for initial system access */
void init_seed_files(void) {
    FILE *fp;
    
    /* Seed Default Doctor if DOCTORS_FILE does not exist */
    fp = fopen(DOCTORS_FILE, "r");
    if (fp == NULL) {
        fp = fopen(DOCTORS_FILE, "w");
        if (fp != NULL) {
            /* Doctor ID 101, Dr. Muntasir, user: Muntasir, pass: 1234 */
            fprintf(fp, "101|Dr. Muntasir|Muntasir|1234|Cardiology|Sun-Tue-Thu|09:00 AM - 01:00 PM\n");
            fclose(fp);
        }
    } else {
        fclose(fp);
    }

    /* Seed Default Patient if PATIENTS_FILE does not exist */
    fp = fopen(PATIENTS_FILE, "r");
    if (fp == NULL) {
        fp = fopen(PATIENTS_FILE, "w");
        if (fp != NULL) {
            /* Patient ID 1, Sami, user: Sami, pass: 78910 */
            fprintf(fp, "1|Sami|Sami|78910|01700000000\n");
            fclose(fp);
        }
    } else {
        fclose(fp);
    }
}

/* =====================================================================================
 *                                ADMIN ROLE MODULE
 * ===================================================================================== */

void admin_addPatient(void) {
    Patient p;
    char before_state[1024];
    char after_state[1024];

    /* 1. Capture BEFORE file state */
    get_file_summary(PATIENTS_FILE, before_state, sizeof(before_state));

    printf("\n--- Admin: Add Patient ---\n");
    printf("Patient ID: ");
    scanf("%d", &p.id);
    printf("Full Name: ");
    scanf(" %49[^\n]", p.name);
    printf("Username: ");
    scanf(" %49s", p.username);
    printf("Password: ");
    scanf(" %49s", p.password);
    printf("Phone: ");
    scanf(" %19s", p.phone);

    FILE *fp = fopen(PATIENTS_FILE, "a");
    if (fp == NULL) {
        printf("Error opening %s file!\n", PATIENTS_FILE);
        return;
    }

    fprintf(fp, "%d|%s|%s|%s|%s\n", p.id, p.name, p.username, p.password, p.phone);
    fclose(fp);

    /* 2. Capture AFTER file state */
    get_file_summary(PATIENTS_FILE, after_state, sizeof(after_state));

    /* 3. Display BEFORE & AFTER state on console */
    printf("\n================ FILE STATE AUDIT ================\n");
    printf("[BEFORE RECORD ENTRY FILE CONTENT]:\n%s\n", before_state);
    printf("\n[AFTER RECORD ENTRY FILE CONTENT]:\n%s\n", after_state);
    printf("==================================================\n");

    /* 4. Log to audit_log.txt */
    log_file_update("Admin Add Patient", before_state, after_state);

    printf("SUCCESS: Patient record added successfully!\n");
}

void admin_viewPatients(void) {
    FILE *fp = fopen(PATIENTS_FILE, "r");
    Patient p;
    if (fp == NULL) {
        printf("No patient records found in %s!\n", PATIENTS_FILE);
        return;
    }
    printf("\n-------------------------------- PATIENT RECORDS --------------------------------\n");
    printf("%-8s %-20s %-15s %-15s %-15s\n", "ID", "Name", "Username", "Password", "Phone");
    printf("---------------------------------------------------------------------------------\n");
    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%49[^|]|%19[^\n]\n", 
                  &p.id, p.name, p.username, p.password, p.phone) == 5) {
        printf("%-8d %-20s %-15s %-15s %-15s\n", p.id, p.name, p.username, p.password, p.phone);
    }
    fclose(fp);
}

void admin_addDoctor(void) {
    Doctor d;
    char before_state[1024];
    char after_state[1024];

    /* 1. Capture BEFORE file state */
    get_file_summary(DOCTORS_FILE, before_state, sizeof(before_state));

    printf("\n--- Admin: Add Doctor ---\n");
    printf("Doctor ID: ");
    scanf("%d", &d.id);
    printf("Full Name: ");
    scanf(" %49[^\n]", d.name);
    printf("Username: ");
    scanf(" %49s", d.username);
    printf("Password: ");
    scanf(" %49s", d.password);
    printf("Specialty: ");
    scanf(" %49[^\n]", d.specialty);
    printf("Working Days (e.g., Sun-Tue-Thu): ");
    scanf(" %59[^\n]", d.working_days);
    printf("Working Hours (e.g., 09:00 AM - 01:00 PM): ");
    scanf(" %39[^\n]", d.working_hours);

    FILE *fp = fopen(DOCTORS_FILE, "a");
    if (fp == NULL) {
        printf("Error opening %s file!\n", DOCTORS_FILE);
        return;
    }

    fprintf(fp, "%d|%s|%s|%s|%s|%s|%s\n", d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours);
    fclose(fp);

    /* 2. Capture AFTER file state */
    get_file_summary(DOCTORS_FILE, after_state, sizeof(after_state));

    /* 3. Display BEFORE & AFTER state on console */
    printf("\n================ FILE STATE AUDIT ================\n");
    printf("[BEFORE RECORD ENTRY FILE CONTENT]:\n%s\n", before_state);
    printf("\n[AFTER RECORD ENTRY FILE CONTENT]:\n%s\n", after_state);
    printf("==================================================\n");

    /* 4. Log to audit_log.txt */
    log_file_update("Admin Add Doctor", before_state, after_state);

    printf("SUCCESS: Doctor record added successfully!\n");
}

void admin_viewDoctors(void) {
    FILE *fp = fopen(DOCTORS_FILE, "r");
    Doctor d;
    if (fp == NULL) {
        printf("No doctor records found in %s!\n", DOCTORS_FILE);
        return;
    }
    printf("\n----------------------------------- DOCTOR RECORDS -----------------------------------\n");
    printf("%-6s %-18s %-12s %-10s %-16s %-15s %-20s\n", "ID", "Name", "Username", "Pass", "Specialty", "Working Days", "Working Hours");
    printf("--------------------------------------------------------------------------------------\n");
    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%59[^|]|%39[^\n]\n", 
                  &d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours) == 7) {
        printf("%-6d %-18s %-12s %-10s %-16s %-15s %-20s\n", d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours);
    }
    fclose(fp);
}

void admin_updateDoctor(void) {
    FILE *fp, *tempFp;
    Doctor d;
    int targetId, found = 0;
    char before_info[300] = "";
    char after_info[300] = "";
    char before_file[1024];
    char after_file[1024];

    printf("\n--- Admin: Update Doctor Information ---\n");
    printf("Enter Doctor ID to update: ");
    scanf("%d", &targetId);

    get_file_summary(DOCTORS_FILE, before_file, sizeof(before_file));

    fp = fopen(DOCTORS_FILE, "r");
    if (fp == NULL) {
        printf("Doctor file not found!\n");
        return;
    }

    tempFp = fopen("temp_doctors.txt", "w");
    if (tempFp == NULL) {
        fclose(fp);
        printf("Error creating temporary file!\n");
        return;
    }

    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%59[^|]|%39[^\n]\n", 
                  &d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours) == 7) {
        if (d.id == targetId) {
            found = 1;
            /* Mark BEFORE UPDATION state */
            snprintf(before_info, sizeof(before_info), 
                     "ID:%d | Name:%s | User:%s | Pass:%s | Spec:%s | Days:%s | Hours:%s", 
                     d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours);

            printf("\n[BEFORE UPDATION TARGET RECORD]: %s\n", before_info);

            printf("Enter New Name: ");
            scanf(" %49[^\n]", d.name);
            printf("Enter New Username: ");
            scanf(" %49s", d.username);
            printf("Enter New Password: ");
            scanf(" %49s", d.password);
            printf("Enter New Specialty: ");
            scanf(" %49[^\n]", d.specialty);
            printf("Enter New Working Days: ");
            scanf(" %59[^\n]", d.working_days);
            printf("Enter New Working Hours: ");
            scanf(" %39[^\n]", d.working_hours);

            /* Mark AFTER UPDATION state */
            snprintf(after_info, sizeof(after_info), 
                     "ID:%d | Name:%s | User:%s | Pass:%s | Spec:%s | Days:%s | Hours:%s", 
                     d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours);
        }
        fprintf(tempFp, "%d|%s|%s|%s|%s|%s|%s\n", d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours);
    }
    fclose(fp);
    fclose(tempFp);

    if (found) {
        remove(DOCTORS_FILE);
        rename("temp_doctors.txt", DOCTORS_FILE);

        get_file_summary(DOCTORS_FILE, after_file, sizeof(after_file));

        printf("\n================ FILE STATE AUDIT ================\n");
        printf("[BEFORE UPDATION RECORD STATE]: %s\n", before_info);
        printf("[AFTER UPDATION RECORD STATE ]: %s\n\n", after_info);
        printf("[BEFORE UPDATION FILE CONTENT]:\n%s\n", before_file);
        printf("\n[AFTER UPDATION FILE CONTENT]:\n%s\n", after_file);
        printf("==================================================\n");

        printf("SUCCESS: Doctor record updated successfully in file!\n");
        log_file_update("Admin Update Doctor", before_file, after_file);
    } else {
        remove("temp_doctors.txt");
        printf("ERROR: Doctor ID %d not found!\n", targetId);
    }
}

void admin_viewAppointments(void) {
    FILE *fp = fopen(APPOINTMENTS_FILE, "r");
    Appointment a;
    if (fp == NULL) {
        printf("No appointment records found in %s!\n", APPOINTMENTS_FILE);
        return;
    }
    printf("\n-------------------------------- ALL APPOINTMENTS --------------------------------\n");
    printf("%-10s %-12s %-10s %-12s %-22s %-12s\n", "Appt ID", "Patient ID", "Doctor ID", "Date", "Time Slot", "Status");
    printf("----------------------------------------------------------------------------------\n");
    while (fscanf(fp, "%d|%d|%d|%14[^|]|%29[^|]|%14[^\n]\n", 
                  &a.appId, &a.patientId, &a.doctorId, a.date, a.time, a.status) == 6) {
        printf("%-10d %-12d %-10d %-12s %-22s %-12s\n", 
               a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);
    }
    fclose(fp);
}

void admin_cancelAppointment(void) {
    FILE *fp, *tempFp;
    Appointment a;
    int targetAppId, found = 0;
    char before_info[300] = "";
    char after_info[300] = "";
    char before_file[1024];
    char after_file[1024];

    printf("\n--- Admin: Cancel Appointment ---\n");
    printf("Enter Appointment ID to cancel: ");
    scanf("%d", &targetAppId);

    get_file_summary(APPOINTMENTS_FILE, before_file, sizeof(before_file));

    fp = fopen(APPOINTMENTS_FILE, "r");
    if (fp == NULL) {
        printf("Appointment file not found!\n");
        return;
    }

    tempFp = fopen("temp_appts.txt", "w");
    if (tempFp == NULL) {
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d|%d|%d|%14[^|]|%29[^|]|%14[^\n]\n", 
                  &a.appId, &a.patientId, &a.doctorId, a.date, a.time, a.status) == 6) {
        if (a.appId == targetAppId) {
            found = 1;
            snprintf(before_info, sizeof(before_info), 
                     "ApptID:%d | PatientID:%d | DoctorID:%d | Date:%s | Time:%s | Status:%s", 
                     a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);

            if (strcmp(a.status, "Booked") == 0) {
                strcpy(a.status, "Cancelled");
                snprintf(after_info, sizeof(after_info), 
                         "ApptID:%d | PatientID:%d | DoctorID:%d | Date:%s | Time:%s | Status:%s", 
                         a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);
            } else {
                printf("Appointment is already %s.\n", a.status);
                snprintf(after_info, sizeof(after_info), "%s (Unchanged)", before_info);
            }
        }
        fprintf(tempFp, "%d|%d|%d|%s|%s|%s\n", a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);
    }
    fclose(fp);
    fclose(tempFp);

    if (found) {
        remove(APPOINTMENTS_FILE);
        rename("temp_appts.txt", APPOINTMENTS_FILE);

        get_file_summary(APPOINTMENTS_FILE, after_file, sizeof(after_file));

        printf("\n================ FILE STATE AUDIT ================\n");
        printf("[BEFORE CANCELLATION RECORD STATE]: %s\n", before_info);
        printf("[AFTER CANCELLATION RECORD STATE ]: %s\n\n", after_info);
        printf("[BEFORE UPDATION FILE CONTENT]:\n%s\n", before_file);
        printf("\n[AFTER UPDATION FILE CONTENT]:\n%s\n", after_file);
        printf("==================================================\n");

        printf("SUCCESS: Appointment status updated to Cancelled!\n");
        log_file_update("Admin Cancel Appointment", before_file, after_file);
    } else {
        remove("temp_appts.txt");
        printf("ERROR: Appointment ID %d not found!\n", targetAppId);
    }
}

void admin_menu(void) {
    int choice;
    char username[50], password[50];

    printf("\n=== ADMIN LOGIN ===\n");
    printf("Username: ");
    scanf(" %49s", username);
    printf("Password: ");
    scanf(" %49s", password);

    /* Hardcoded Admin Credentials specified in SRS */
    if (strcmp(username, "Toufik") != 0 || strcmp(password, "4567") != 0) {
        printf("ERROR: Authentication failed! Invalid Admin credentials.\n");
        return;
    }

    printf("\nLogin Successful! Welcome Admin %s.\n", username);

    while (1) {
        printf("\n=================== ADMIN MENU ===================\n");
        printf("1. Add Patient (Set Username/Password)\n");
        printf("2. View All Patients\n");
        printf("3. Add Doctor (Set Username/Password)\n");
        printf("4. View All Doctors\n");
        printf("5. Update Doctor Details (Pre/Post-Update Marked)\n");
        printf("6. View All Appointments\n");
        printf("7. Cancel Appointment (Pre/Post-Update Marked)\n");
        printf("0. Logout to Main Menu\n");
        printf("==================================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: admin_addPatient(); break;
            case 2: admin_viewPatients(); break;
            case 3: admin_addDoctor(); break;
            case 4: admin_viewDoctors(); break;
            case 5: admin_updateDoctor(); break;
            case 6: admin_viewAppointments(); break;
            case 7: admin_cancelAppointment(); break;
            case 0: printf("Logging out of Admin Role...\n"); return;
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}

/* =====================================================================================
 *                                DOCTOR ROLE MODULE
 * ===================================================================================== */

void doctor_viewSchedule(int doctorId) {
    FILE *fpDocs = fopen(DOCTORS_FILE, "r");
    FILE *fpAppts = fopen(APPOINTMENTS_FILE, "r");
    Doctor d;
    Appointment a;
    int foundDoc = 0, apptCount = 0;

    if (fpDocs != NULL) {
        while (fscanf(fpDocs, "%d|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%59[^|]|%39[^\n]\n", 
                      &d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours) == 7) {
            if (d.id == doctorId) {
                foundDoc = 1;
                printf("\n--- DOCTOR SCHEDULE ---\n");
                printf("Doctor ID: %d | Name: %s\nSpecialty: %s\nWorking Days: %s\nWorking Hours: %s\n",
                       d.id, d.name, d.specialty, d.working_days, d.working_hours);
                break;
            }
        }
        fclose(fpDocs);
    }

    if (!foundDoc) {
        printf("Doctor ID %d details not found in system.\n", doctorId);
        if (fpAppts) fclose(fpAppts);
        return;
    }

    printf("\n--- ASSIGNED APPOINTMENTS ---\n");
    printf("%-10s %-12s %-12s %-22s %-12s\n", "Appt ID", "Patient ID", "Date", "Time Slot", "Status");
    printf("---------------------------------------------------------------\n");
    if (fpAppts != NULL) {
        while (fscanf(fpAppts, "%d|%d|%d|%14[^|]|%29[^|]|%14[^\n]\n", 
                      &a.appId, &a.patientId, &a.doctorId, a.date, a.time, a.status) == 6) {
            if (a.doctorId == doctorId) {
                printf("%-10d %-12d %-12s %-22s %-12s\n", 
                       a.appId, a.patientId, a.date, a.time, a.status);
                apptCount++;
            }
        }
        fclose(fpAppts);
    }
    if (apptCount == 0) {
        printf("No appointments assigned yet.\n");
    }
}

void doctor_viewAppointmentsByDate(int doctorId) {
    FILE *fp = fopen(APPOINTMENTS_FILE, "r");
    Appointment a;
    char targetDate[15];
    int found = 0;

    printf("\nEnter Date (YYYY-MM-DD or DD-MM-YYYY): ");
    scanf("%14s", targetDate);

    if (fp == NULL) {
        printf("No appointments recorded.\n");
        return;
    }

    printf("\n--- APPOINTMENTS FOR DOCTOR ID %d ON %s ---\n", doctorId, targetDate);
    printf("%-10s %-12s %-22s %-12s\n", "Appt ID", "Patient ID", "Time Slot", "Status");
    printf("--------------------------------------------------------\n");
    while (fscanf(fp, "%d|%d|%d|%14[^|]|%29[^|]|%14[^\n]\n", 
                  &a.appId, &a.patientId, &a.doctorId, a.date, a.time, a.status) == 6) {
        if (a.doctorId == doctorId && strcmp(a.date, targetDate) == 0) {
            printf("%-10d %-12d %-22s %-12s\n", a.appId, a.patientId, a.time, a.status);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) printf("No appointments found on this date.\n");
}

void doctor_viewCancelledAppointmentsByDate(int doctorId) {
    FILE *fp = fopen(APPOINTMENTS_FILE, "r");
    Appointment a;
    char targetDate[15];
    int found = 0;

    printf("\nEnter Date (YYYY-MM-DD or DD-MM-YYYY): ");
    scanf("%14s", targetDate);

    if (fp == NULL) {
        printf("No appointments recorded.\n");
        return;
    }

    printf("\n--- CANCELLED APPOINTMENTS FOR DOCTOR ID %d ON %s ---\n", doctorId, targetDate);
    printf("%-10s %-12s %-22s %-12s\n", "Appt ID", "Patient ID", "Time Slot", "Status");
    printf("--------------------------------------------------------\n");
    while (fscanf(fp, "%d|%d|%d|%14[^|]|%29[^|]|%14[^\n]\n", 
                  &a.appId, &a.patientId, &a.doctorId, a.date, a.time, a.status) == 6) {
        if (a.doctorId == doctorId && strcmp(a.date, targetDate) == 0 && strcmp(a.status, "Cancelled") == 0) {
            printf("%-10d %-12d %-22s %-12s\n", a.appId, a.patientId, a.time, a.status);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) printf("No cancelled appointments found on this date.\n");
}

void doctor_viewAvailableSlots(int doctorId) {
    FILE *fpAppts = fopen(APPOINTMENTS_FILE, "r");
    Appointment a;
    char targetDate[15];
    char slots[5][30] = {
        "09:00 AM - 09:30 AM",
        "09:30 AM - 10:00 AM",
        "10:00 AM - 10:30 AM",
        "02:00 PM - 02:30 PM",
        "04:00 PM - 04:30 PM"
    };
    int isBooked[5] = {0, 0, 0, 0, 0};

    printf("\nEnter Date (YYYY-MM-DD or DD-MM-YYYY): ");
    scanf("%14s", targetDate);

    if (fpAppts != NULL) {
        while (fscanf(fpAppts, "%d|%d|%d|%14[^|]|%29[^|]|%14[^\n]\n", 
                      &a.appId, &a.patientId, &a.doctorId, a.date, a.time, a.status) == 6) {
            if (a.doctorId == doctorId && strcmp(a.date, targetDate) == 0 && strcmp(a.status, "Booked") == 0) {
                for (int i = 0; i < 5; i++) {
                    if (strcmp(a.time, slots[i]) == 0) {
                        isBooked[i] = 1;
                    }
                }
            }
        }
        fclose(fpAppts);
    }

    printf("\n--- TIME SLOT AVAILABILITY FOR DOCTOR ID %d ON %s ---\n", doctorId, targetDate);
    for (int i = 0; i < 5; i++) {
        printf("Slot %d: %-25s -> %s\n", i+1, slots[i], isBooked[i] ? "[BOOKED]" : "[AVAILABLE]");
    }
}

void doctor_cancelAppointment(int doctorId) {
    FILE *fp, *tempFp;
    Appointment a;
    int targetAppId, found = 0;
    char before_info[300] = "";
    char after_info[300] = "";
    char before_file[1024];
    char after_file[1024];

    printf("\nEnter Appointment ID to cancel: ");
    scanf("%d", &targetAppId);

    get_file_summary(APPOINTMENTS_FILE, before_file, sizeof(before_file));

    fp = fopen(APPOINTMENTS_FILE, "r");
    if (fp == NULL) {
        printf("No appointments found.\n");
        return;
    }

    tempFp = fopen("temp_appts.txt", "w");
    if (tempFp == NULL) {
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d|%d|%d|%14[^|]|%29[^|]|%14[^\n]\n", 
                  &a.appId, &a.patientId, &a.doctorId, a.date, a.time, a.status) == 6) {
        if (a.appId == targetAppId && a.doctorId == doctorId) {
            found = 1;
            snprintf(before_info, sizeof(before_info), 
                     "ApptID:%d | PatientID:%d | DoctorID:%d | Date:%s | Time:%s | Status:%s", 
                     a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);

            if (strcmp(a.status, "Booked") == 0) {
                strcpy(a.status, "Cancelled");
                snprintf(after_info, sizeof(after_info), 
                         "ApptID:%d | PatientID:%d | DoctorID:%d | Date:%s | Time:%s | Status:%s", 
                         a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);
            } else {
                printf("Appointment is already %s.\n", a.status);
                snprintf(after_info, sizeof(after_info), "%s (Unchanged)", before_info);
            }
        }
        fprintf(tempFp, "%d|%d|%d|%s|%s|%s\n", a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);
    }
    fclose(fp);
    fclose(tempFp);

    if (found) {
        remove(APPOINTMENTS_FILE);
        rename("temp_appts.txt", APPOINTMENTS_FILE);

        get_file_summary(APPOINTMENTS_FILE, after_file, sizeof(after_file));

        printf("\n================ FILE STATE AUDIT ================\n");
        printf("[BEFORE CANCELLATION RECORD STATE]: %s\n", before_info);
        printf("[AFTER CANCELLATION RECORD STATE ]: %s\n\n", after_info);
        printf("[BEFORE UPDATION FILE CONTENT]:\n%s\n", before_file);
        printf("\n[AFTER UPDATION FILE CONTENT]:\n%s\n", after_file);
        printf("==================================================\n");

        printf("SUCCESS: Appointment %d cancelled and slot released!\n", targetAppId);
        log_file_update("Doctor Cancel Appointment", before_file, after_file);
    } else {
        remove("temp_appts.txt");
        printf("ERROR: Appointment ID %d not found or not assigned to Doctor ID %d!\n", targetAppId, doctorId);
    }
}

void doctor_updateSchedule(int doctorId) {
    FILE *fp, *tempFp;
    Doctor d;
    int found = 0;
    char before_info[300] = "";
    char after_info[300] = "";
    char before_file[1024];
    char after_file[1024];

    get_file_summary(DOCTORS_FILE, before_file, sizeof(before_file));

    fp = fopen(DOCTORS_FILE, "r");
    if (fp == NULL) {
        printf("Doctor file not found!\n");
        return;
    }

    tempFp = fopen("temp_doctors.txt", "w");
    if (tempFp == NULL) {
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%59[^|]|%39[^\n]\n", 
                  &d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours) == 7) {
        if (d.id == doctorId) {
            found = 1;
            snprintf(before_info, sizeof(before_info), 
                     "ID:%d | Name:%s | Days:%s | Hours:%s", 
                     d.id, d.name, d.working_days, d.working_hours);

            printf("\n[BEFORE UPDATION TARGET RECORD]: %s\n", before_info);
            printf("Enter New Working Days (e.g., Sun-Tue-Thu): ");
            scanf(" %59[^\n]", d.working_days);
            printf("Enter New Working Hours (e.g., 09:00 AM - 01:00 PM): ");
            scanf(" %39[^\n]", d.working_hours);

            snprintf(after_info, sizeof(after_info), 
                     "ID:%d | Name:%s | Days:%s | Hours:%s", 
                     d.id, d.name, d.working_days, d.working_hours);
        }
        fprintf(tempFp, "%d|%s|%s|%s|%s|%s|%s\n", d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours);
    }
    fclose(fp);
    fclose(tempFp);

    if (found) {
        remove(DOCTORS_FILE);
        rename("temp_doctors.txt", DOCTORS_FILE);

        get_file_summary(DOCTORS_FILE, after_file, sizeof(after_file));

        printf("\n================ FILE STATE AUDIT ================\n");
        printf("[BEFORE UPDATION RECORD STATE]: %s\n", before_info);
        printf("[AFTER UPDATION RECORD STATE ]: %s\n\n", after_info);
        printf("[BEFORE UPDATION FILE CONTENT]:\n%s\n", before_file);
        printf("\n[AFTER UPDATION FILE CONTENT]:\n%s\n", after_file);
        printf("==================================================\n");

        printf("SUCCESS: Schedule updated successfully!\n");
        log_file_update("Doctor Update Schedule", before_file, after_file);
    } else {
        remove("temp_doctors.txt");
        printf("ERROR: Doctor ID %d not found!\n", doctorId);
    }
}

void doctor_menu(void) {
    char input_username[50], input_password[50];
    int choice;
    FILE *fp = fopen(DOCTORS_FILE, "r");
    Doctor d;
    int authenticated = 0;
    int activeDoctorId = 0;

    printf("\n=== DOCTOR LOGIN ===\n");
    printf("Username: ");
    scanf(" %49s", input_username);
    printf("Password: ");
    scanf(" %49s", input_password);

    if (fp != NULL) {
        while (fscanf(fp, "%d|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%59[^|]|%39[^\n]\n", 
                      &d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours) == 7) {
            if (strcmp(d.username, input_username) == 0 && strcmp(d.password, input_password) == 0) {
                authenticated = 1;
                activeDoctorId = d.id;
                printf("\nAuthentication Successful! Welcome Dr. %s (%s)!\n", d.name, d.specialty);
                break;
            }
        }
        fclose(fp);
    }

    if (!authenticated) {
        printf("ERROR: Authentication failed! Invalid Doctor username or password.\n");
        return;
    }

    while (1) {
        printf("\n=================== DOCTOR MENU ===================\n");
        printf("1. View Schedule & All Assigned Appointments\n");
        printf("2. View Appointments by Date\n");
        printf("3. View Cancelled Appointments by Date\n");
        printf("4. View Available Time Slots by Date\n");
        printf("5. Cancel an Assigned Appointment (Pre/Post-Update Marked)\n");
        printf("6. Update Working Days & Hours (Pre/Post-Update Marked)\n");
        printf("0. Logout to Main Menu\n");
        printf("===================================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: doctor_viewSchedule(activeDoctorId); break;
            case 2: doctor_viewAppointmentsByDate(activeDoctorId); break;
            case 3: doctor_viewCancelledAppointmentsByDate(activeDoctorId); break;
            case 4: doctor_viewAvailableSlots(activeDoctorId); break;
            case 5: doctor_cancelAppointment(activeDoctorId); break;
            case 6: doctor_updateSchedule(activeDoctorId); break;
            case 0: printf("Logging out of Doctor Role...\n"); return;
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}

/* =====================================================================================
 *                                PATIENT ROLE MODULE
 * ===================================================================================== */

void patient_register(void) {
    Patient p;
    char before_state[1024];
    char after_state[1024];

    /* 1. Capture BEFORE file state */
    get_file_summary(PATIENTS_FILE, before_state, sizeof(before_state));

    printf("\n--- Patient Registration ---\n");
    printf("Patient ID: ");
    scanf("%d", &p.id);
    printf("Full Name: ");
    scanf(" %49[^\n]", p.name);
    printf("Desired Username: ");
    scanf(" %49s", p.username);
    printf("Desired Password: ");
    scanf(" %49s", p.password);
    printf("Phone Number: ");
    scanf(" %19s", p.phone);

    FILE *fp = fopen(PATIENTS_FILE, "a");
    if (fp == NULL) {
        printf("Error opening patient file!\n");
        return;
    }

    fprintf(fp, "%d|%s|%s|%s|%s\n", p.id, p.name, p.username, p.password, p.phone);
    fclose(fp);

    /* 2. Capture AFTER file state */
    get_file_summary(PATIENTS_FILE, after_state, sizeof(after_state));

    /* 3. Display BEFORE & AFTER state on console */
    printf("\n================ FILE STATE AUDIT ================\n");
    printf("[BEFORE REGISTRATION FILE CONTENT]:\n%s\n", before_state);
    printf("\n[AFTER REGISTRATION FILE CONTENT]:\n%s\n", after_state);
    printf("==================================================\n");

    /* 4. Log to audit_log.txt */
    log_file_update("Patient Register", before_state, after_state);

    printf("SUCCESS: Registration complete! You can now log in with Username: %s\n", p.username);
}

void patient_viewDoctors(void) {
    FILE *fp = fopen(DOCTORS_FILE, "r");
    Doctor d;
    if (fp == NULL) {
        printf("No doctor records found in system.\n");
        return;
    }
    printf("\n---------------- AVAILABLE DOCTORS ----------------\n");
    printf("%-8s %-20s %-20s %-18s\n", "ID", "Doctor Name", "Specialty", "Working Days");
    printf("---------------------------------------------------\n");
    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%59[^|]|%39[^\n]\n", 
                  &d.id, d.name, d.username, d.password, d.specialty, d.working_days, d.working_hours) == 7) {
        printf("%-8d %-20s %-20s %-18s\n", d.id, d.name, d.specialty, d.working_days);
    }
    fclose(fp);
}

void patient_bookAppointment(int patientId) {
    FILE *fp;
    Appointment a, old;
    int isConflict = 0;
    char before_state[1024];
    char after_state[1024];

    patient_viewDoctors();

    /* Capture BEFORE file state */
    get_file_summary(APPOINTMENTS_FILE, before_state, sizeof(before_state));

    a.patientId = patientId;
    printf("\n--- Book Appointment ---\n");
    printf("Appointment ID: ");
    scanf("%d", &a.appId);
    printf("Doctor ID: ");
    scanf("%d", &a.doctorId);
    printf("Date (YYYY-MM-DD or DD-MM-YYYY): ");
    scanf("%14s", a.date);
    printf("Time Slot (e.g., 09:00 AM - 09:30 AM): ");
    scanf(" %29[^\n]", a.time);
    strcpy(a.status, "Booked");

    /* Check for conflicting booking on same Doctor, Date, Time Slot */
    fp = fopen(APPOINTMENTS_FILE, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d|%d|%d|%14[^|]|%29[^|]|%14[^\n]\n", 
                      &old.appId, &old.patientId, &old.doctorId, old.date, old.time, old.status) == 6) {
            if (old.doctorId == a.doctorId && 
                strcmp(old.date, a.date) == 0 && 
                strcmp(old.time, a.time) == 0 && 
                strcmp(old.status, "Booked") == 0) {
                isConflict = 1;
                break;
            }
        }
        fclose(fp);
    }

    if (isConflict) {
        printf("ERROR: Time slot '%s' for Doctor ID %d on %s is ALREADY BOOKED!\n", a.time, a.doctorId, a.date);
        printf("Please choose another slot or date.\n");
        return;
    }

    fp = fopen(APPOINTMENTS_FILE, "a");
    if (fp == NULL) {
        printf("Error opening appointments file!\n");
        return;
    }
    fprintf(fp, "%d|%d|%d|%s|%s|%s\n", a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);
    fclose(fp);

    /* Capture AFTER file state */
    get_file_summary(APPOINTMENTS_FILE, after_state, sizeof(after_state));

    /* Display BEFORE & AFTER state on console */
    printf("\n================ FILE STATE AUDIT ================\n");
    printf("[BEFORE BOOKING FILE CONTENT]:\n%s\n", before_state);
    printf("\n[AFTER BOOKING FILE CONTENT]:\n%s\n", after_state);
    printf("==================================================\n");

    /* Log to audit_log.txt */
    log_file_update("Patient Book Appointment", before_state, after_state);

    printf("SUCCESS: Appointment booked successfully! Appt ID: %d\n", a.appId);
}

void patient_viewHistory(int patientId) {
    FILE *fp = fopen(APPOINTMENTS_FILE, "r");
    Appointment a;
    int count = 0;

    if (fp == NULL) {
        printf("No appointment records found.\n");
        return;
    }

    printf("\n----------------- MY APPOINTMENT HISTORY -----------------\n");
    printf("%-10s %-10s %-12s %-22s %-12s\n", "Appt ID", "Doctor ID", "Date", "Time Slot", "Status");
    printf("----------------------------------------------------------\n");
    while (fscanf(fp, "%d|%d|%d|%14[^|]|%29[^|]|%14[^\n]\n", 
                  &a.appId, &a.patientId, &a.doctorId, a.date, a.time, a.status) == 6) {
        if (a.patientId == patientId) {
            printf("%-10d %-10d %-12s %-22s %-12s\n", 
                   a.appId, a.doctorId, a.date, a.time, a.status);
            count++;
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("No appointment history found for Patient ID: %d\n", patientId);
    }
}

void patient_cancelAppointment(int patientId) {
    FILE *fp, *tempFp;
    Appointment a;
    int targetAppId, found = 0;
    char before_info[300] = "";
    char after_info[300] = "";
    char before_file[1024];
    char after_file[1024];

    printf("\nEnter Appointment ID to cancel: ");
    scanf("%d", &targetAppId);

    get_file_summary(APPOINTMENTS_FILE, before_file, sizeof(before_file));

    fp = fopen(APPOINTMENTS_FILE, "r");
    if (fp == NULL) {
        printf("No appointments file found.\n");
        return;
    }

    tempFp = fopen("temp_appts.txt", "w");
    if (tempFp == NULL) {
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d|%d|%d|%14[^|]|%29[^|]|%14[^\n]\n", 
                  &a.appId, &a.patientId, &a.doctorId, a.date, a.time, a.status) == 6) {
        if (a.appId == targetAppId && a.patientId == patientId) {
            found = 1;
            snprintf(before_info, sizeof(before_info), 
                     "ApptID:%d | PatientID:%d | DoctorID:%d | Date:%s | Time:%s | Status:%s", 
                     a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);

            if (strcmp(a.status, "Booked") == 0) {
                strcpy(a.status, "Cancelled");
                snprintf(after_info, sizeof(after_info), 
                         "ApptID:%d | PatientID:%d | DoctorID:%d | Date:%s | Time:%s | Status:%s", 
                         a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);
            } else {
                printf("Appointment is already %s.\n", a.status);
                snprintf(after_info, sizeof(after_info), "%s (Unchanged)", before_info);
            }
        }
        fprintf(tempFp, "%d|%d|%d|%s|%s|%s\n", a.appId, a.patientId, a.doctorId, a.date, a.time, a.status);
    }
    fclose(fp);
    fclose(tempFp);

    if (found) {
        remove(APPOINTMENTS_FILE);
        rename("temp_appts.txt", APPOINTMENTS_FILE);

        get_file_summary(APPOINTMENTS_FILE, after_file, sizeof(after_file));

        printf("\n================ FILE STATE AUDIT ================\n");
        printf("[BEFORE CANCELLATION RECORD STATE]: %s\n", before_info);
        printf("[AFTER CANCELLATION RECORD STATE ]: %s\n\n", after_info);
        printf("[BEFORE UPDATION FILE CONTENT]:\n%s\n", before_file);
        printf("\n[AFTER UPDATION FILE CONTENT]:\n%s\n", after_file);
        printf("==================================================\n");

        printf("SUCCESS: Appointment %d cancelled!\n", targetAppId);
        log_file_update("Patient Cancel Appointment", before_file, after_file);
    } else {
        remove("temp_appts.txt");
        printf("ERROR: Appointment ID %d not found or does not belong to Patient ID %d!\n", targetAppId, patientId);
    }
}

void patient_menu(void) {
    int choice;
    char input_username[50], input_password[50];
    FILE *fp;
    Patient p;
    int authenticated = 0;
    int activePatientId = 0;

    printf("\n=== PATIENT ACCESS ===\n");
    printf("1. Register New Patient Account\n");
    printf("2. Login with Username & Password\n");
    printf("Choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        patient_register();
        return;
    } else if (choice != 2) {
        printf("Invalid choice!\n");
        return;
    }

    printf("Username: ");
    scanf(" %49s", input_username);
    printf("Password: ");
    scanf(" %49s", input_password);

    fp = fopen(PATIENTS_FILE, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d|%49[^|]|%49[^|]|%49[^|]|%19[^\n]\n", 
                      &p.id, p.name, p.username, p.password, p.phone) == 5) {
            if (strcmp(p.username, input_username) == 0 && strcmp(p.password, input_password) == 0) {
                authenticated = 1;
                activePatientId = p.id;
                printf("\nAuthentication Successful! Welcome %s!\n", p.name);
                break;
            }
        }
        fclose(fp);
    }

    if (!authenticated) {
        printf("ERROR: Authentication failed! Invalid Patient username or password. Please register first if you do not have an account.\n");
        return;
    }

    while (1) {
        printf("\n=================== PATIENT MENU ===================\n");
        printf("1. View Available Doctors\n");
        printf("2. Book Appointment\n");
        printf("3. View Appointment History\n");
        printf("4. Cancel Appointment (Pre/Post-Update Marked)\n");
        printf("0. Logout to Main Menu\n");
        printf("====================================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: patient_viewDoctors(); break;
            case 2: patient_bookAppointment(activePatientId); break;
            case 3: patient_viewHistory(activePatientId); break;
            case 4: patient_cancelAppointment(activePatientId); break;
            case 0: printf("Logging out of Patient Role...\n"); return;
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}

/* =====================================================================================
 *                            MAIN SYSTEM & ROLE SELECTION
 * ===================================================================================== */

int main(void) 
{
    int roleChoice;

    /* Initialize seed data files for default logins (Muntasir / Sami) */
    init_seed_files();

    while (1) {
        printf("\n=====================================================================\n");
        printf("         CLINIC APPOINTMENT SCHEDULING SYSTEM (FALL 2026)            \n");
        printf("=====================================================================\n");
        printf("Please select your role to log in:\n");
        printf("1. Admin Login (Username: Toufik | Pass: 4567)\n");
        printf("2. Doctor Login (Username: Muntasir | Pass: 1234)\n");
        printf("3. Patient Portal (Username: Sami | Pass: 78910 or Register)\n");
        printf("0. Exit Application\n");
        printf("=====================================================================\n");
        printf("Enter your choice (0-3): ");
        if (scanf("%d", &roleChoice) != 1) {
            break;
        }

        switch (roleChoice) {
            case 1:
                admin_menu();
                break;
            case 2:
                doctor_menu();
                break;
            case 3:
                patient_menu();
                break;
            case 0:
                printf("\nThank you for using the Clinic Appointment Scheduling System. Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid role selected! Please select a valid option (0-3).\n");
        }
    }

    return 0;

}
