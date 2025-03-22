#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define AdminUser "admin"
#define AdminPass "admin"
#define defaultDoctorPass "doctor@123"
#define defaultPatientPass "user@123"

// global variable
sqlite3 *db;
const char *table[5] = {"patientdetail", "doctordetail", "users", "appointmentdetail", "medicine"};
const char *services[10] = {"anesthesiologists", "cardiologists", "endocrinologists", "gastroenterologists", "general surgeons", "nephrologists", "neurologists", "ophthalmologists", "psychiatrists"};
const char *chars = " !\"#$%&()*+,-./:;<=>?@[]^_`{|}~0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *key = "5Gc7srO6Ll}tp1dY@B0kJAX8zVif<uFIK.P v%3|=o\"wSNh;:n*qa?/E+{)_Tg&QUxZD4b9#!~-RjeC^2`[my,HM>(W]$";

// function to encrypt password
void encrypt(char *pass);

// function to decrypt password
void decrypt(char *encoded_pass);

// function to getchoice
int getChoice(int num);

// function to check authorize or not for patient and doctor
bool IsAuthorize(const char *username, const char *password, const char *identify); // done

// function to add new entry
void Add(const char *identify); // done

// function to update info
void Update(const char *identify); // done

// function to delete
void Delete(const char *identify); // done

// function to search
void Search(const char *identify); // done

// function to fetch all patients and doctors detials
void GetAll(const char *identify); // done

// Function to check patient login
void PatientDoctorLogin(const char *identify); // done

// Function to check Admin login
bool AdminLogin(); // done

// function to manage patient details
void PatientDoctorManage(const char *identify); // done

// function to execute sql
void ExecuteSql(const char *sql); // done

// function to call back
int callback(void *data, int no_fields, char **field_values, char **field_name); // done

// function to clear creen
void clear(); // done

// function to view appointment (this is for doctor)
void ViewAppointment(const char *doctor_username); // done

// function to mark ststus
void MarkAppointmentStatus(const char *username); // done

// function to book appointment
void BookAppointment(const char *doctor_username, const char *patient_username); // done

// function to reschedule appointment
void RescheduleAppointment(const char *patient_username); // done

// fucntion to cancel appointment
void CancelAppointment(const char *patient_username); // done

// function for doctor menu
void DoctorMenu(const char *username); // done

// function to get all servecies
void GetAllServices(const char *patient_usr); // done

// function to add medicine
void AddMedicine(); // done

// function to view medicine
void ViewMedicines(); // done

// function to remove medicine
void DeleteMedicine(); // done

// function to update medicine stock
void UpdateStock(); // done

// function to manage pharmacy
void PharmacyManage(); // done

// function for patient menu
void PatientMenu(const char *username); // done

// function to create table
void CreateTable(); // done

// function for admin menu
void AdminMenu(); // done

// function to convert lower to upper case
void toLowerCase(char *str);

// function to view patientappointment
void PatientAppointment(const char *username);

int main()
{

    // char zErrMsg = 0;
    // rc = return code
    int rc, choice;

    rc = sqlite3_open("HMS.db", &db);
    if (choice < 4)
    {
        if (rc != SQLITE_OK)
        {
            printf("Failed to open databese err: %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else
        {
            CreateTable();
            clear();
        }
    }
    printf("-----------------Who are you?-----------------\n");
    printf("1.Patient\n2.Doctor\n3.Admin\n4.Exit\n");
    choice = getChoice(4);
    switch (choice)
    {
    case 1:
        clear();
        PatientDoctorLogin("patient");
        break;

    case 2:
        clear();
        PatientDoctorLogin("doctor");
        break;

    case 3:
        clear();
        if (AdminLogin())
        {
            AdminMenu();
        }
        else
        {
            clear();
            printf("Exiting.......\n");
            sqlite3_close(db);
            exit(0);
        }
        break;

    case 4:
        clear();
        printf("Exiting.......\n");
        sqlite3_close(db);
        exit(0);
        break;
    }

    return 0;
}

// database
void CreateTable()
{
    const char *patient_table, *doctor_table, *appointment_table, *user_pass, *medicine;

    patient_table = "CREATE TABLE IF NOT EXISTS patientdetail("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name TEXT NOT NULL,"
                    "age INTEGER NOT NULL,"
                    "bloodgrp TEXT NOT NULL,"
                    "gender TEXT NOT NULL,"
                    "phone TEXT NOT NULL,"
                    "email TEXT,"
                    "address TEXT);";

    doctor_table = "CREATE TABLE IF NOT EXISTS doctordetail("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "name TEXT NOT NULL,"
                   "age INTEGER NOT NULL,"
                   "phone TEXT NOT NULL,"
                   "email TEXT,"
                   "gender TEXT NOT NULL,"
                   "experience INTEGER,"
                   "specialization TEXT);";

    appointment_table = "CREATE TABLE IF NOT EXISTS appointmentdetail("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "patient_username TEXT NOT NULL,"
                        "doctor_username TEXT NOT NULL,"
                        "appointment_date TEXT NOT NULL,"
                        "appointment_time TEXT NOT NULL,"
                        "status TEXT DEFAULT 'scheduled',"
                        "notes TEXT DEFAULT '');";

    user_pass = "CREATE TABLE IF NOT EXISTS users("
                "username1 TEXT NOT NULL,"
                "username2 TEXT NOT NULL,"
                "password TEXT NOT NULL);";

    medicine = "CREATE TABLE IF NOT EXISTS medicine("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name TEXT NOT NULL,"
               "quantity INTEGER NOT NULL,"
               "price REAL NOT NULL);";

    ExecuteSql(patient_table);
    ExecuteSql(doctor_table);
    ExecuteSql(user_pass);
    ExecuteSql(appointment_table);
    ExecuteSql(medicine);
}

void ExecuteSql(const char *sql)
{
    char *ErrMsg = NULL;
    int rc;
    // sqlite3_exec(sqlite3 *, const char *sql, int (*callback), void *, char **errmsg);
    rc = sqlite3_exec(db, sql, callback, 0, &ErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("SQL err: %s\n", ErrMsg);
        sqlite3_free(ErrMsg);
    }
}

int callback(void *data, int no_fields, char **field_values, char **field_name)
{
    int i;
    for (i = 0; i < no_fields; i++)
    {
        printf("%s: %s\n", field_name[i], field_values[i] ? field_values[i] : NULL);
    }
    printf("\n");
    return 0;
}

// login
void PatientDoctorLogin(const char *identify)
{
    char username[50], password[50];
    int attempt = 0;
    while (true)
    {
        if (attempt == 5)
        {
            printf("Try agian leter! or contact to reception.\n");
            printf("Exiting.......");
            sqlite3_close(db);
            exit(0);
        }

        printf("username: ");
        scanf("%49s", username);
        username[strcspn(username, "\n")] = 0;

        printf("password: ");
        scanf("%49s", password);
        password[strcspn(password, "\n")] = 0;
        encrypt(password);

        if (IsAuthorize(username, password, identify))
        {
            if (strcmp(identify, "patient") == 0)
            {
                clear();
                PatientMenu(username);
                break;
            }
            else if (strcmp(identify, "doctor") == 0)
            {
                clear();
                DoctorMenu(username);
                break;
            }
        }
        else
        {
            clear();
            printf("Invalid username and password!\n");
        }
        attempt++;
    }
}

bool AdminLogin()
{
    char username[50], password[50];
    int attempt = 0;
    do
    {
        if (attempt == 5)
        {
            printf("Sorry, you are not authorized to access!");
            return false;
        }
        printf("username: ");
        scanf("%s", username);
        printf("password: ");
        scanf("%s", password);
        clear();
        if (strcmp(username, AdminUser) != 0)
        {
            printf("\bWrong username and password!\n");
        }
        else
        {
            if (strcmp(password, AdminPass) == 0)
            {
                return true;
            }
            printf("\bWrong password!\n");
        }

        attempt++;

    } while (strcmp(username, AdminUser) != 0 || strcmp(password, AdminPass) != 0);
    return true;
}

// menu
void AdminMenu()
{
    int choice;
    getchar();
    printf("---------------Welcome Cheif---------------\n");
    printf("What do you want to do?\n");
    printf("1.Manage Patient\n2.Manage Doctor\n3.Manage Pharmacy\n4.exit\n");
    choice = getChoice(4);
    switch (choice)
    {
    case 1:
        clear();
        PatientDoctorManage("patient");
        break;

    case 2:
        clear();
        PatientDoctorManage("doctor");
        break;

    case 3:
        clear();
        PharmacyManage();
        break;

    case 4:
        clear();
        printf("Exiting.......\n");
        sqlite3_close(db);
        exit(0);
        break;
    }
}

void PatientMenu(const char *username)
{
    int choice, i = 0;
    char sql[256], specialization[50], doctor_username[20];

    printf("------------------------------------------------------------\n");
    printf("Please entry your choice:\n");
    printf("1.Get self detail\n2.Get all services with doctor\n3.Doctor Detail\n4.Book Appoinment\n5.View my Appointment\n6.Reschedule Appointment\n7.Cancel Appointment\n8.Exit\n");
    choice = getChoice(8);

    switch (choice)
    {
    case 1:
        clear();
        sprintf(sql, "SELECT * FROM %s WHERE phone = '%s' OR email = '%s';", table[0], username, username);
        ExecuteSql(sql);
        printf("\nHit enter!\n");
        getchar();
        clear();
        PatientMenu(username);
        break;

    case 2:
        clear();
        GetAllServices(username);
        break;

    case 3:
        clear();
        printf("Enter doctor specialization or username of doctor: ");
        getchar();
        fgets(specialization, sizeof(specialization), stdin);
        specialization[strcspn(specialization, "\n")] = '\0';
        toLowerCase(specialization);
        clear();
        sprintf(sql, "SELECT * FROM %s WHERE phone = '%s' OR email = '%s' OR specialization = '%s';", table[1], specialization, specialization, specialization);
        ExecuteSql(sql);
        printf("\nHit Enter!");
        getchar();
        clear();
        PatientMenu(username);
        break;

    case 4:
        clear();
        printf("Enter doctor username(phone): ");
        scanf("%20s", &doctor_username);
        BookAppointment(doctor_username, username);
        break;

    case 5:
        clear();
        PatientAppointment(username);
        break;

    case 6:
        clear();
        RescheduleAppointment(username);
        break;

    case 7:
        clear();
        CancelAppointment(username);
        break;

    case 8:
        clear();
        printf("Exiting.......");
        sqlite3_close(db);
        exit(0);
    }
}

void DoctorMenu(const char *username)
{
    int choice;

    printf("------------------------------------------------------------\n");
    printf("1.View Assigned Appointment.\n2.Mark As Completed\n3.Exit\n");
    choice = getChoice(3);
    switch (choice)
    {
    case 1:
        clear();
        ViewAppointment(username);
        break;
    case 2:
        clear();
        MarkAppointmentStatus(username);
    case 3:
        clear();
        printf("Exiting.......\n");
        sqlite3_close(db);
        exit(0);
    }
}

// manage
void PatientDoctorManage(const char *identify)
{
    int choice;
    printf("Choose what do you want to do?\n");
    printf("1.Add new\n2.Update info\n3.Delete record\n4.Get specific record\n5.Get detail of all\n6.Return Previoud Page\n7.exit\n");
    choice = getChoice(7);
    clear();
    switch (choice)
    {
    case 1:
        clear();
        Add(identify);
        break;

    case 2:
        clear();
        Update(identify);
        break;

    case 3:
        clear();
        Delete(identify);
        break;

    case 4:
        clear();
        Search(identify);
        break;

    case 5:
        clear();
        GetAll(identify);
        break;

    case 6:
        clear();
        AdminMenu();
        break;

    case 7:
        clear();
        printf("Exiting.......");
        sqlite3_close(db);
        exit(0);
        break;
    }
}

void PharmacyManage()
{
    int choice;
    printf("\nPharmacy Management System\n");
    printf("1. Add Medicine\n");
    printf("2. View Medicines\n");
    printf("3. Update Stock\n");
    printf("4. Delete Medicine\n");
    printf("5. Goto previous menu\n");
    printf("6. Exit\n");
    printf("Choose an option: ");
    choice = getChoice(6);

    switch (choice)
    {
    case 1:
        clear();
        AddMedicine();
        break;
    case 2:
        clear();
        ViewMedicines();
        break;
    case 3:
        clear();
        UpdateStock();
        break;
    case 4:
        clear();
        DeleteMedicine();
        break;
    case 5:
        clear();
        AdminMenu();
        break;
    case 6:
        clear();
        printf("Exiting...\n");
        sqlite3_close(db);
        exit(0);
        break;
    }
}

void Search(const char *identify)
{
    char sql[500], username[50], selected_table[50];

    getchar();
    printf("username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    if (strlen(username) == 0)
    {
        printf("Invalid!\n");
        Search(identify);
    }
    if (strcmp(identify, "patient") == 0)
    {
        strcpy(selected_table, table[0]);
    }
    else if (strcmp(identify, "doctor") == 0)
    {
        strcpy(selected_table, table[1]);
    }
    sprintf(sql, "SELECT * FROM %s WHERE phone = '%s' OR email = '%s';", selected_table, username, username);
    ExecuteSql(sql);
    printf("\nHit enter!\n");
    getchar();
    clear();
    PatientDoctorManage(identify);
}

void GetAll(const char *identify)
{
    char sql[100], selected_talble[50];
    if (strcmp(identify, "patient") == 0)
    {
        strcpy(selected_talble, table[0]);
    }
    else
    {
        strcpy(selected_talble, table[1]);
    }
    sprintf(sql, "SELECT * FROM %s;", selected_talble);
    ExecuteSql(sql);
    printf("\nHit enter!\n");
    getchar();
    getchar();
    clear();
    PatientDoctorManage(identify);
}

void Update(const char *identify)
{
    int more, age, experience, count = 0;
    char username[50], name[50], phone[20], email[30], address[50], bloodgrp[10], gender[10], specialization[50], sql[500], sql_[100];
    while (true)
    {
        if (count > 0)
        {
            printf("\nHit enter to exit\n");
        }
        printf("Enter username(phone or email): \n");
        getchar();
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;
        if (strlen(username) == 0)
        {
            if (count > 0)
            {
                clear();
                PatientDoctorManage(identify);
            }
            printf("Invalid!");
            Update(identify);
        }

        printf("\n---------------Enter new deatil---------------\n");
        printf("name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        printf("age: ");
        scanf("%d", &age);
        getchar();

        printf("phone: ");
        fgets(phone, sizeof(phone), stdin);
        phone[strcspn(phone, "\n")] = '\0';

        printf("email: ");
        fgets(email, sizeof(email), stdin);
        email[strcspn(email, "\n")] = '\0';

        printf("gender: ");
        fgets(gender, sizeof(gender), stdin);
        gender[strcspn(gender, "\n")] = '\0';
        if (strcmp(identify, "patient") == 0)
        {
            printf("address: ");
            fgets(address, sizeof(address), stdin);
            address[strcspn(address, "\n")] = '\0';

            printf("bloodgrp: ");
            fgets(bloodgrp, sizeof(bloodgrp), stdin);
            bloodgrp[strcspn(bloodgrp, "\n")] = '\0';

            sprintf(sql, "UPDATE %s SET name = '%s', age = %d, bloodgrp = '%s', gender = '%s', phone = '%s', email = '%s', address = '%s' WHERE  phone = '%s' OR email = '%s';", table[0], name, age, bloodgrp, gender, phone, email, address, username, username);
            ExecuteSql(sql);
        }
        else
        {
            printf("address: ");
            scanf("%d", experience);

            printf("specialization: ");
            fgets(specialization, sizeof(specialization), stdin);
            specialization[strcspn(specialization, "\n")] = '\0';
            sprintf(sql, "UPDATE %s SET name = '%s', age = %d, phone = '%s', email = '%s', gender = '%s', experience = '%s', specialization = '%s' WHERE phone = '%s' OR email = '%s';", table[1], name, age, phone, email, gender, experience, specialization, username, username);
            ExecuteSql(sql);
        }
        sprintf(sql_, "UPDATE %s SET username1 = '%s', username2 = '%s' WHERE username1 = '%s' OR username2 = '%s';", table[2], phone, email, username, username);
        ExecuteSql(sql_);
        count++;
    }
}

void Add(const char *identify)
{
    int more, age, experience, count = 0;
    char name[50], phone[20], email[30], address[50], bloodgrp[10], gender[10], specialization[50], sql[500], sqla[100], pass[20] = "";
    getchar();
    do
    {
        if (count > 0)
        {
            printf("\nHit enter to exit!\n");
        }
        printf("----------New Detail----------\n");
        printf("name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        if (strlen(name) == 0)
        {
            if (count > 0)
            {
                clear();
                PatientDoctorManage(identify);
            }
            printf("Invalid!\n");
            Add(identify);
        }
        printf("age: ");
        scanf("%d", &age);
        getchar();
        printf("phone: ");
        fgets(phone, sizeof(phone), stdin);
        phone[strcspn(phone, "\n")] = '\0';

        printf("email: ");
        fgets(email, sizeof(email), stdin);
        email[strcspn(email, "\n")] = '\0';

        printf("gender: ");
        fgets(gender, sizeof(gender), stdin);
        gender[strcspn(gender, "\n")] = '\0';
        if (strcmp(identify, "patient") == 0)
        {
            printf("address: ");
            fgets(address, sizeof(address), stdin);
            address[strcspn(address, "\n")] = '\0';

            printf("bloodgrp: ");
            fgets(bloodgrp, sizeof(bloodgrp), stdin);
            bloodgrp[strcspn(bloodgrp, "\n")] = '\0';

            strcpy(pass, defaultPatientPass);
            encrypt(pass);

            sprintf(sql, "INSERT INTO %s (name, age, bloodgrp, gender, phone, email, address) VALUES ('%s', %d, '%s', '%s', '%s', '%s', '%s');", table[0], name, age, bloodgrp, gender, phone, email, address);
            ExecuteSql(sql);
        }
        else if (strcmp(identify, "doctor") == 0)
        {
            printf("experience: ");
            scanf("%d", &experience);
            getchar();

            strcpy(pass, defaultDoctorPass);
            encrypt(pass);

            printf("specialization: ");
            fgets(specialization, sizeof(specialization), stdin);
            specialization[strcspn(specialization, "\n")] = 0;
            toLowerCase(specialization);

            sprintf(sql, "INSERT INTO %s(name, age, phone, email, gender, experience, specialization) VALUES ('%s', %d, '%s', '%s', '%s', %d, '%s');", table[1], name, age, phone, email, gender, experience, specialization);
            ExecuteSql(sql);
            ;
        }
        sprintf(sqla, "INSERT INTO %s (username1, username2, password) VALUES ('%s', '%s', '%s');", table[2], phone, email, pass);
        ExecuteSql(sqla);
        count++;
        clear();
        printf("Added successfully!\n");
    } while (true);
}

void Delete(const char *identify)
{
    int count = 0;
    char username[50], sql[300], sql_[200], selected_table[20];
    while (true)
    {
        if (count > 0)
        {
            printf("\nHit enter to exit!\n");
        }
        else
        {
            getchar();
        }
        printf("Enter username(phone or email): ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = '\0';

        if (strlen(username) == 0)
        {
            if (count > 0)
            {
                clear();
                PatientDoctorManage(identify);
            }
            printf("Invalid!\n");
            Delete(identify);
        }
        if (strcmp(identify, "patient") == 0)
        {
            strcpy(selected_table, table[0]);
        }
        else if (strcmp(identify, "doctor") == 0)
        {
            strcpy(selected_table, table[1]);
        }
        sprintf(sql, "DELETE FROM %s WHERE phone = '%s' OR email = '%s';", selected_table, username, username);
        ExecuteSql(sql);
        sprintf(sql_, "DELETE FROM %s WHERE username1 = '%s' OR username2 = '%s';", table[2], username, username);
        ExecuteSql(sql_);
        printf("Deleted successfully!\n");
        count++;
    }
}

bool IsAuthorize(const char *username, const char *password, const char *identify)
{
    sqlite3_stmt *stmt;
    char sql[256], sql_[256], update_pass[256], confirm_pass[256], defaultPass[12] = "";
    int rc;

    sprintf(sql, "SELECT * FROM %s;", table[2]);

    // rc --> return code
    // -1 --> calculate the length of the string automatic
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statemetn: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(0);
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const char *stored_user1 = (const char *)sqlite3_column_text(stmt, 0);
        const char *stored_user2 = (const char *)sqlite3_column_text(stmt, 1);
        const char *stored_pass = (const char *)sqlite3_column_text(stmt, 2);

        if (strcmp(username, stored_user1) == 0 || strcmp(username, stored_user2) == 0)
        {
            if (strcmp(password, stored_pass) == 0)
            {
                if (strcmp(identify, "patient") == 0)
                {
                    strcpy(defaultPass, defaultPatientPass);
                }
                else
                {
                    strcpy(defaultPass, defaultDoctorPass);
                }
                encrypt(defaultPass);
                if (strcmp(stored_pass, defaultPass) == 0)
                {
                    clear();
                    printf("At first you've to change password!\n");
                    char update_pass[50], confirm_pass[50];
                    do
                    {

                        printf("Type new password: ");
                        scanf("%s", update_pass);
                        printf("Retype new password: ");
                        scanf("%s", confirm_pass);
                        encrypt(update_pass);
                        encrypt(confirm_pass);
                        if (strcmp(update_pass, confirm_pass) == 0)
                        {
                            if (strcmp(update_pass, defaultPass) == 0)
                            {
                                clear();
                                printf("New password can't be old password.\n");
                            }
                        }
                        else
                        {
                            clear();
                            printf("Password is not match.\n");
                        }
                    } while (!(strcmp(update_pass, confirm_pass) == 0 && strcmp(update_pass, defaultPass) != 0));
                    sprintf(sql_, "UPDATE %s SET password = '%s' WHERE username1 = '%s' OR username2 = '%s';", table[2], confirm_pass, username, username);
                    ExecuteSql(sql_);
                    clear();

                    printf("Password update successfully!\n");
                    printf("Hit enter to continue!\n");
                    getchar();
                    getchar();
                    return true;
                }
                return true;
            }
            return false;
        }
    }
    return false;
}

void GetAllServices(const char *patient_usr)
{
    int i, choice, count = 0;
    char sql[256], want, doctor_username[50];

    printf("We have these services, Please choose any services to get doctor of that particular services\n");
    for (i = 0; i < 10; i++)
    {
        printf("%d.%s\n", i + 1, services[i]);
        count++;
    }
    choice = getChoice(count);
    clear();

    sprintf(sql, "SELECT * FROM %s WHERE specialization = '%s';", table[1], services[choice - 1]);
    ExecuteSql(sql);

    printf("Want to book Appointment (y/n)?\n");
    getchar();
    scanf("%c", &want);
    if (want == 'y' || want == 'Y')
    {
        clear();
        printf("Enter Doctor username(phone): ");
        scanf("%49s", &doctor_username);
        doctor_username[strcspn(doctor_username, "\n")] = 0;
        clear();
        BookAppointment(doctor_username, patient_usr);
    }
    clear();
    PatientMenu(patient_usr);
}

void BookAppointment(const char *doctor_username, const char *patient_username)
{
    char date[20], time[20], status[20], notes[100], sql[300], *errMsg;

    printf("Enter Date(YYYY-MM-DD): ");
    scanf("%20s", &date);
    date[strcspn(date, "\n")] = 0;

    printf("Enter Time(HH-MM): ");
    scanf("%20s", &time);
    time[strcspn(time, "\n")] = 0;

    sprintf(sql, "INSERT INTO %s (patient_username, doctor_username, appointment_date, appointment_time) VALUES ('%s', '%s', '%s', '%s');", table[3], patient_username, doctor_username, date, time);
    ExecuteSql(sql);

    printf("\nAppointment booked successfully!\n");
    printf("\nHit enter!\n");
    getchar();
    getchar();
    clear();
    PatientMenu(patient_username);
}

void MarkAppointmentStatus(const char *username)
{
    char sql[300], note[200], patient_username[20];

    printf("Enter patient username(phone): ");
    getchar();
    fgets(patient_username, sizeof(patient_username), stdin);
    patient_username[strcspn(patient_username, "\n")] = 0;

    printf("Suggestion: ");
    fgets(note, sizeof(note), stdin);
    note[strcspn(note, "\n")] = '\0';

    sprintf(sql, "UPDATE %s SET notes = '%s', status = 'completed' WHERE patient_username = '%s' AND doctor_username = '%s';", table[3], note, patient_username, username);
    ExecuteSql(sql);
    printf("\nMarked complete successfully\n");
    printf("\nHit enter!");
    getchar();
    clear();
    DoctorMenu(username);
}

void ViewAppointment(const char *doctor_username)
{
    char sql[300];
    sprintf(sql, "SELECT * FROM %s WHERE doctor_username = '%s' AND (status = 'scheduled' OR status = 'rescheduled');", table[3], doctor_username);
    ExecuteSql(sql);
    printf("\nHit enter!");
    getchar();
    getchar();
    clear();
    DoctorMenu(doctor_username);
}

void PatientAppointment(const char *username)
{
    char sql[300];
    sprintf(sql, "SELECT * FROM %s WHERE patient_username = '%s' AND (status != 'canlced' OR status != 'completed');", table[3], username);
    ExecuteSql(sql);
    printf("\nHit enter!");
    getchar();
    getchar();
    clear();
    PatientMenu(username);
}

void RescheduleAppointment(const char *patient_username)
{
    char sql[256], date[20], time[20];

    printf("Enter new date(YYYY-MM-DD): ");
    scanf("%20s", &date);
    date[strcspn(date, "\n")] = 0;

    printf("Enter new time(HH:MM): ");
    scanf("%20s", &time);
    time[strcspn(time, "\n")] = 0;

    sprintf(sql, "UPDATE %s SET appointment_date = '%s', appointment_time = '%s', status = 'rescheduled'WHERE patient_username = '%s';", table[3], date, time, patient_username);
    ExecuteSql(sql);

    printf("\nAppointment rescheduled successully.\n");
    printf("\nHit enter!\n");
    getchar();
    getchar();
    clear();
    PatientMenu(patient_username);
}

void CancelAppointment(const char *patient_username)
{
    char sql[256];

    sprintf(sql, "DELETE FROM %s WHERE patient_username = '%s';", table[3], patient_username);
    ExecuteSql(sql);
    printf("\nAppointment calcel successfully\n");
    printf("\nHit enter!\n");
    getchar();
    getchar();
    clear();
    PatientMenu(patient_username);
}

void AddMedicine()
{
    char name[50], sql[256];
    int quantity, count = 0;
    float price;
    clear();
    while (true)
    {
        if (count != 0)
        {
            printf("\nHit enter to exit!\n");
        }
        getchar();
        printf("Enter medicine name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        if (strlen(name) == 0)
        {
            if (count > 0)
            {
                clear();
                PharmacyManage();
            }
            printf("Invalid!");
            AddMedicine();
        }

        printf("Enter quantity: ");
        scanf("%d", &quantity);
        printf("Enter price: ");
        scanf("%f", &price);
        snprintf(sql, sizeof(sql),
                 "INSERT INTO medicine (name, quantity, price) VALUES ('%s', %d, %.2f);",
                 name, quantity, price);

        ExecuteSql(sql);
        printf("Medicine added successfully!\n");
        count++;
    }
}

void ViewMedicines()
{
    sqlite3_stmt *stmt;
    char *sql = "SELECT * FROM medicine;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        printf("Failed to fetch medicines.\n");
        return;
    }

    printf("\nID | Name          | Quantity      | Price\n");
    printf("------------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        printf("%d | %-10s | %d | %.2f\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_int(stmt, 2),
               sqlite3_column_double(stmt, 3));
    }

    sqlite3_finalize(stmt);
    printf("\n\nHit enter!\n");
    getchar();
    getchar();
    clear();
    PharmacyManage();
}

void UpdateStock()
{
    char name[50];
    int quantity, count = 0;
    char sql[256];
    clear();
    while (true)
    {
        if (count != 0)
        {
            printf("\nHit enter to exit!\n");
        }
        getchar();
        printf("Enter medicine name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        if (strlen(name) == 0)
        {
            if (count > 0)
            {
                PharmacyManage();
            }
            clear();
            printf("Invalid!");
            UpdateStock();
        }
        printf("Enter new quantity: ");
        scanf("%d", &quantity);

        snprintf(sql, sizeof(sql),
                 "UPDATE %s SET quantity = %d WHERE name = '%s';",
                 table[4], quantity, name);

        ExecuteSql(sql);
        printf("Stock updated successfully!\n");
        count++;
    }
}

void DeleteMedicine()
{
    char name[50], sql[256];
    int count = 0;

    while (true)
    {
        if (count != 0)
        {
            printf("\nHit enter to exit!\n");
        }
        else
        {
            getchar();
        }
        printf("Enter medicine name to delete: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        if (strlen(name) == 0)
        {
            if (count > 0)
            {
                clear();
                PharmacyManage();
            }
            clear();
            printf("Invalid!");
            DeleteMedicine();
        }
        snprintf(sql, sizeof(sql),
                 "DELETE FROM %s WHERE name = '%s';",
                 table[4], name);

        ExecuteSql(sql);
        printf("Medicine deleted successfully!\n");
        count++;
    }
}

void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] += 32; // Convert uppercase to lowercase
        }
    }
}

void encrypt(char *pass)
{

    int i, j;
    char encoded_pass[100];
    for (i = 0; i < strlen(pass); i++)
    {
        for (j = 0; j < strlen(chars); j++)
        {
            if (pass[i] == chars[j])
            {
                encoded_pass[i] = key[j];
            }
        }
    }
    encoded_pass[strlen(pass)] = '\0';
    strcpy(pass, encoded_pass);
}

void decrypt(char *encoded_pass)
{
    int i, j;
    char decoded[100];

    for (i = 0; i < strlen(encoded_pass); i++)
    {
        for (j = 0; j < strlen(chars); j++)
        {
            if (encoded_pass[i] == key[j])
            {
                decoded[i] = chars[j];
            }
        }
    }
    decoded[strlen(encoded_pass)] = '\0';
    strcpy(encoded_pass, decoded);
}

int getChoice(int num)
{
    char c[40];
    int count = 0;

    while (true)
    {
        printf("Choice: ");
        do
        {
            fgets(c, sizeof(c), stdin);
        } while (isspace(c[0]));

        while(c[count] != '\n')
        {
            count++;
        }
        if(count > 1)
        {
            count = 0;
            c[0] = '\0';
            printf("Invalid!\n");
            continue;
        }
        c[1] = '\0';
        if (isdigit(c[0]))
        {
            if (((c[0] - '0') > 0) &&(num >= (c[0] - '0')))
            {
                return (c[0] - '0');
            }
            else
            {
                printf("Invalid!\n");
                continue;
            }
        }
        while (getchar() != '\n');
		printf("Invalid!\n");
    }
}

void clear()
{
    system("clear");
}