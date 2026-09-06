/*
Structures in C
---------------
What this program solves:
Input-based demonstration of C structures: declaring a struct,
creating variables, entering records with scanf, accessing members
with the dot operator, arrays of structures, computing per-record
averages, nested structures, and structure pointers with the arrow
operator.

Real life example:
A college student record system. Each student has a roll number,
name and three subject marks. A single struct groups all this
related data into one unit, and an array of structs stores the
whole class - exactly like a row in a database table.

DIAGRAM (a struct is one labelled box; -> accesses through a pointer):
  struct Student s = { roll 7, marks 88.5 };
  +---------+----------+
  | roll: 7 | marks: 88.5 |
  +---------+----------+
  struct Student *p = &s;   p->roll  is  (*p).roll

How the process works:
1. struct Student defines a blueprint: int roll, char name[30],
   float marks[3].
2. struct Student s1; reserves memory for one record; scanf fills
   each field using the dot (.) operator, e.g. scanf("%d", &s1.roll).
3. An array of structs stores many records: students[i].marks[j].
4. A struct is passed to a function by pointer so the function can
   read or modify the original without copying.
5. struct Student *ptr = &s1; then ptr->roll accesses members
   through the pointer (arrow operator).
6. Nested struct: a Date struct inside an Admission struct gives
   one level of containment, accessed as admission.joiningDate.day.

Pseudo code:
START
DEFINE Student { roll, name, marks[3] }
READ n
FOR i = 0..n-1: READ students[i].roll, name, marks[3]
FOR i = 0..n-1: average = sum(marks)/3, PRINT record
READ admission id and date; PRINT nested record
ptr = &students[0]; PRINT ptr->roll
END

Sample input and dry run:
n = 2
Student 1: roll 1, "Arjun", marks 90 85 88 -> avg 87.67
Student 2: roll 2, "Diya",  marks 75 80 82 -> avg 79.00
Admission 501 joined on 14/06/2026

Main logic to understand:
A struct bundles logically related fields of different types into
one named unit. The dot operator works on struct variables, the
arrow operator works on struct pointers, and arrays of structs give
simple in-memory "tables".
*/

#include <stdio.h>
#include <string.h>

#define NUM_SUBJECTS 3
#define MAX_STUDENTS 10

struct Date {
    int day;
    int month;
    int year;
};

struct Student {
    int roll;
    char name[30];
    float marks[NUM_SUBJECTS];
};

struct Admission {
    int admissionId;
    struct Date joiningDate;
};

/*
 * computeAverage - Returns the mean of the three subject marks.
 */
float computeAverage(const struct Student* s) {
    float total = 0;
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        total = total + s->marks[i];
    }
    return total / NUM_SUBJECTS;
}

/*
 * printStudent - Displays one student record.
 */
void printStudent(const struct Student* s) {
    printf("Roll %-4d Name %-10s Marks: %5.1f %5.1f %5.1f  Avg: %.2f\n",
           s->roll, s->name,
           s->marks[0], s->marks[1], s->marks[2],
           computeAverage(s));
}

/*
 * readStudent - Reads one student record from the user.
 */
void readStudent(struct Student* s) {
    printf("Enter roll number: ");
    scanf("%d", &s->roll);
    printf("Enter name (single word): ");
    scanf("%29s", s->name);
    printf("Enter %d subject marks separated by space: ", NUM_SUBJECTS);
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        scanf("%f", &s->marks[i]);
    }
}

int main() {
    int n;

    printf("Structures in C - Student Records\n\n");

    /* STEP: array of structures filled with user input */
    printf("How many students (1-%d)? ", MAX_STUDENTS);
    scanf("%d", &n);

    if (n < 1 || n > MAX_STUDENTS) {
        printf("Invalid count\n");
        return 1;
    }

    struct Student students[MAX_STUDENTS];

    for (int i = 0; i < n; i++) {
        printf("\nStudent %d:\n", i + 1);
        readStudent(&students[i]);
    }

    printf("\nAll student records:\n");
    for (int i = 0; i < n; i++) {
        printStudent(&students[i]);
    }

    /* STEP: find the topper - comparing struct fields */
    int topperIndex = 0;
    float topperAvg = computeAverage(&students[0]);
    for (int i = 1; i < n; i++) {
        if (computeAverage(&students[i]) > topperAvg) {
            topperAvg = computeAverage(&students[i]);
            topperIndex = i;
        }
    }
    printf("\nTopper: %s (roll %d) with average %.2f\n",
           students[topperIndex].name, students[topperIndex].roll, topperAvg);

    /* STEP: search a record by roll number using the dot operator */
    int searchRoll;
    printf("\nEnter roll number to search: ");
    scanf("%d", &searchRoll);

    int found = -1;
    for (int i = 0; i < n; i++) {
        if (students[i].roll == searchRoll) {
            found = i;
            break;
        }
    }
    if (found != -1) {
        printf("Found:\n");
        printStudent(&students[found]);
    } else {
        printf("No student with roll %d\n", searchRoll);
    }

    /* STEP: struct pointer with the arrow operator */
    struct Student* ptr = &students[0];
    printf("\nStruct pointer (arrow operator) on first record:\n");
    printf("ptr->roll = %d\n", ptr->roll);
    printf("ptr->name = %s\n", ptr->name);

    /* STEP: nested structure - a Date inside Admission, from user input */
    struct Admission adm;
    printf("\nAdmission details:\n");
    printf("Enter admission id: ");
    scanf("%d", &adm.admissionId);
    printf("Enter joining date (dd mm yyyy): ");
    scanf("%d %d %d", &adm.joiningDate.day,
          &adm.joiningDate.month, &adm.joiningDate.year);

    printf("Admission %d joined on %02d/%02d/%04d\n",
           adm.admissionId,
           adm.joiningDate.day, adm.joiningDate.month, adm.joiningDate.year);

    /* STEP: sizeof shows how fields are laid out in memory */
    printf("\nMemory sizes:\n");
    printf("sizeof(struct Student)   = %zu bytes\n", sizeof(struct Student));
    printf("sizeof(struct Admission) = %zu bytes\n", sizeof(struct Admission));
    printf("sizeof(struct Date)      = %zu bytes\n", sizeof(struct Date));

    return 0;
}
