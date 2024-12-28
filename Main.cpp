// Updated C++ Program for Attendance Management System with CSV for Admins and Attendance
// Uses linked list for student data management and CSV files for storage
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

using namespace std;

// Node structure for linked list
struct Student {
    string usn;
    string name;
    string password;
    int presentCount;
    int lateCount;
    int absentCount;
    Student* next;
};

// Function prototypes
void mainMenu();
void adminLogin();
void studentLogin();
void adminMenu();
void addStudent();
void removeStudent();
void markAttendance();
void viewAttendance();
void viewStudentAttendance(const string& usn);
Student* findStudent(const string& usn);
void loadStudentData();
void saveStudentData();
void loadAdminData();
bool validateAdmin(const string& username, const string& password);
void saveAttendance(const string& usn, const string& name, const string& status);

// Global linked list head
Student* studentList = nullptr;

// Admin credentials
struct Admin {
    string username;
    string password;
};
Admin adminCredentials[10]; // Supports up to 10 admins
int adminCount = 0;



// Function to allow students to mark their own attendance
void studentMarkAttendance(Student* student) {
    int status;

    cout << "\n--- Mark Your Attendance ---\n";
    cout << "1. Present\n";
    cout << "2. Late\n";
    cout << "3. Absent\n";
    cout << "Enter your status (1/2/3): ";
    cin >> status;

    switch (status) {
        case 1:
            student->presentCount++;
            saveAttendance(student->usn, student->name, "Present");
            cout << "Attendance marked as Present.\n";
            break;
        case 2:
            student->lateCount++;
            saveAttendance(student->usn, student->name, "Late");
            cout << "Attendance marked as Late.\n";
            break;
        case 3:
            student->absentCount++;
            saveAttendance(student->usn, student->name, "Absent");
            cout << "Attendance marked as Absent.\n";
            break;
        default:
            cout << "Invalid input. Attendance not marked.\n";
    }
}
void studentMenu(Student* student) {
    int choice;

    do {
        cout << "\n--- Student Menu ---\n";
        cout << "1. View Attendance\n";
        cout << "2. Mark Attendance\n";
        cout << "3. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewStudentAttendance(student->usn);
                break;
            case 2:
                studentMarkAttendance(student);
                break;
            case 3:
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}
// Utility function to load student data from CSV
void loadStudentData() {
    ifstream file("students.csv");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string usn, name, password, present, late, absent;

        getline(ss, usn, ',');
        getline(ss, name, ',');
        getline(ss, password, ',');
        getline(ss, present, ',');
        getline(ss, late, ',');
        getline(ss, absent, ',');

        Student* newStudent = new Student{usn, name, password, stoi(present), stoi(late), stoi(absent), studentList};
        studentList = newStudent;
    }

    file.close();
}

// Utility function to save student data to CSV
void saveStudentData() {
    ofstream file("students.csv");

    Student* current = studentList;
    while (current) {
        file << current->usn << "," << current->name << "," << current->password << "," << current->presentCount << "," << current->lateCount << "," << current->absentCount << "\n";
        current = current->next;
    }

    file.close();
}

// Utility function to load admin data from CSV
void loadAdminData() {
    ifstream file("admin.csv");
    string line;
    adminCount = 0;

    while (getline(file, line) && adminCount < 10) {
        stringstream ss(line);
        getline(ss, adminCredentials[adminCount].username, ',');
        getline(ss, adminCredentials[adminCount].password, ',');
        adminCount++;
    }

    file.close();
}

// Validate admin credentials
bool validateAdmin(const string& username, const string& password) {
    for (int i = 0; i < adminCount; i++) {
        if (adminCredentials[i].username == username && adminCredentials[i].password == password) {
            return true;
        }
    }
    return false;
}

// Save attendance to CSV
void saveAttendance(const string& usn, const string& name, const string& status) {
    ofstream file("attendance.csv", ios::app);
    time_t now = time(0);
    tm* ltm = localtime(&now);

    file << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday << "," << usn << "," << name << "," << status << "\n";

    file.close();
}

// Function to find a student by USN
Student* findStudent(const string& usn) {
    Student* current = studentList;
    while (current) {
        if (current->usn == usn) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Admin menu implementation
void adminMenu() {
    int choice;

    do {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Student\n";
        cout << "2. Remove Student\n";
        cout << "3. Mark Attendance\n";
        cout << "4. View Attendance\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                removeStudent();
                break;
            case 3:
                markAttendance();
                break;
            case 4:
                viewAttendance();
                break;
            case 5:
                saveStudentData();
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

// Function to add a student
void addStudent() {
    string usn, name, password;
    cout << "Enter USN: ";
    cin >> usn;
    cout << "Enter Name: ";
    cin >> name;
    cout << "Set Password: ";
    cin >> password;

    if (findStudent(usn)) {
        cout << "Student with USN " << usn << " already exists.\n";
        return;
    }

    Student* newStudent = new Student{usn, name, password, 0, 0, 0, studentList};
    studentList = newStudent;

    cout << "Student added successfully.\n";
}

// Function to remove a student
void removeStudent() {
    string usn;
    cout << "Enter USN of the student to remove: ";
    cin >> usn;

    Student* current = studentList;
    Student* prev = nullptr;

    while (current && current->usn != usn) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        cout << "Student with USN " << usn << " not found.\n";
        return;
    }

    if (!prev) {
        studentList = current->next;
    } else {
        prev->next = current->next;
    }

    delete current;
    cout << "Student removed successfully.\n";
}

// Function to mark attendance
void markAttendance() {
    string usn;
    int status;

    cout << "\n--- Mark Attendance ---\n";
    cout << "1. Present\n";
    cout << "2. Late\n";
    cout << "3. Absent\n";

    Student* current = studentList;
    while (current) {
        cout << "USN: " << current->usn << ", Name: " << current->name << ", Status (1/2/3): ";
        cin >> status;

        switch (status) {
            case 1:
                current->presentCount++;
                saveAttendance(current->usn, current->name, "Present");
                break;
            case 2:
                current->lateCount++;
                saveAttendance(current->usn, current->name, "Late");
                break;
            case 3:
                current->absentCount++;
                saveAttendance(current->usn, current->name, "Absent");
                break;
            default:
                cout << "Invalid input. Skipping this student.\n";
        }

        current = current->next;
    }

    cout << "Attendance marked successfully.\n";
}

// Function to view all attendance
void viewAttendance() {
    Student* current = studentList;

    cout << "\n--- Attendance List ---\n";
    while (current) {
        cout << "USN: " << current->usn << ", Name: " << current->name
             << ", Present: " << current->presentCount
             << ", Late: " << current->lateCount
             << ", Absent: " << current->absentCount << "\n";
        current = current->next;
    }
}

// Function to view specific student attendance
void viewStudentAttendance(const string& usn) {
    Student* student = findStudent(usn);

    if (!student) {
        cout << "Student with USN " << usn << " not found.\n";
        return;
    }

    cout << "\n--- Attendance Details ---\n";
    cout << "USN: " << student->usn << ", Name: " << student->name
         << ", Present: " << student->presentCount
         << ", Late: " << student->lateCount
         << ", Absent: " << student->absentCount << "\n";
}

// Main menu
void mainMenu() {
    int choice;

    loadStudentData();
    loadAdminData();

    do {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Student Login\n";
        cout << "2. Admin Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string usn, password;
                cout << "Enter your USN: ";
                cin >> usn;
                cout << "Enter your Password: ";
                cin >> password;

                Student* student = findStudent(usn);
                if (student && student->password == password) {
                    studentMenu(student); // Navigate to the student menu
                } else {
                    cout << "Invalid USN or Password.\n";
                }
                break;
            }
            case 2:
                adminLogin();
                break;
            case 3:
                saveStudentData();
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

// Admin login stub
void adminLogin() {
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;

    if (validateAdmin(username, password)) {
        adminMenu();
    } else {
        cout << "Invalid credentials.\n";
    }
}

int main() {
    mainMenu();
    return 0;
}
