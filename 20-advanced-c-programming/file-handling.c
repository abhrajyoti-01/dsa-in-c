/*
File Handling - Persistence in C (Master Level C)
--------------------------------------------------
What this program solves:
Reading and writing FILES: text mode (fprintf/fscanf/fgets),
binary mode (fwrite/fread), append mode, and CSV-style records -
the last piece that turns lab programs into real applications.

Real life example:
Config files, saved games and CSV exports all use fopen/fscanf/
fprintf patterns exactly like this program.

DIAGRAM (FILE* wraps a buffer between your program and disk):
  program  <--fprintf-->  buffer  <--flush-->  disk file
  modes: "r" read, "w" truncate+write, "a" append, "r+" both
  always check fopen != NULL; fclose flushes the buffer.

BEGINNER EXPLANATION:
Variables die when the program ends. FILES let data survive:
- fopen("name", "w") = open for writing (OVERWRITES).
- fopen("name", "a") = append (adds at the end).
- fopen("name", "r") = read.
- "rb"/"wb" = binary mode for raw structs (fast, exact).
fprintf writes like printf but into a file; fgets reads a LINE;
fwrite/fread copy raw memory. Always check the pointer - fopen
returns NULL when the file cannot be opened.

SUPER SIMPLE EXAMPLE:
FILE* f = fopen("scores.txt", "w");
fprintf(f, "Riya 88\n");
fclose(f);
cat scores.txt -> "Riya 88". That's persistence in 3 lines.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Reading a file that does not exist: fopen returns NULL -> the
  program reports it instead of crashing (ALWAYS check).
- "w" on an existing file DELETES its contents - use "a" to keep
  and extend. The most common file-handling accident.
- fscanf returning EOF at end-of-file: loop with
  while (fscanf(...) != EOF) - forgetting this loops forever.
- Binary files are not human-readable: opening a struct-written
  file in a text editor shows garbage - that is expected.
- fgets keeps the trailing newline - strip it or strcmp fails
  mysteriously (classic bug).

DRY RUN (write then read back 3 records):
write: fprintf "1 Arjun 90" etc. fclose.
read: fscanf loops 3 times printing each record; 4th fscanf hits
EOF -> loop ends. Output identical to input -> persistence proven.

TIPS:
- Text files are portable and debuggable; binary is compact and
  exact for structs (fixed-size records -> random access with
  fseek). Choose per need.
- The pattern here (save on exit, load on start) is how every
  real app remembers state - combine with the generic vector
  (folder 20) for in-memory lists.
- Always fclose: buffered data may not reach the disk otherwise.
*/

#include <stdio.h>
#include <string.h>

typedef struct {
    int roll;
    char name[30];
    int marks;
} StudentRecord;

void writeTextRecords(void) {
    FILE* f = fopen("students.txt", "w");
    if (f == NULL) {
        printf("Cannot open students.txt for writing\n");
        return;
    }
    int n;
    printf("How many records? ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        StudentRecord s;
        printf("roll name marks: ");
        scanf("%d %29s %d", &s.roll, s.name, &s.marks);
        fprintf(f, "%d %s %d\n", s.roll, s.name, s.marks);
    }
    fclose(f);
    printf("Wrote %d records to students.txt\n", n);
}

void readTextRecords(void) {
    FILE* f = fopen("students.txt", "r");
    if (f == NULL) {
        printf("students.txt not found - write first\n");
        return;
    }
    StudentRecord s;
    int count = 0;
    printf("Records:\n");
    while (fscanf(f, "%d %29s %d", &s.roll, s.name, &s.marks) == 3) {
        printf("  roll %d, %s, marks %d\n", s.roll, s.name, s.marks);
        count = count + 1;
    }
    fclose(f);
    printf("Read %d records\n", count);
}

void appendRecord(void) {
    FILE* f = fopen("students.txt", "a");
    if (f == NULL) {
        printf("Cannot open for append\n");
        return;
    }
    StudentRecord s;
    printf("roll name marks: ");
    scanf("%d %29s %d", &s.roll, s.name, &s.marks);
    fprintf(f, "%d %s %d\n", s.roll, s.name, s.marks);
    fclose(f);
    printf("Appended 1 record\n");
}

void binaryRoundTrip(void) {
    FILE* f = fopen("students.bin", "wb");
    if (f == NULL) {
        printf("Cannot open binary file\n");
        return;
    }
    int n;
    printf("How many records (binary)? ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        StudentRecord s;
        printf("roll name marks: ");
        scanf("%d %29s %d", &s.roll, s.name, &s.marks);
        fwrite(&s, sizeof(StudentRecord), 1, f);
    }
    fclose(f);

    f = fopen("students.bin", "rb");
    if (f == NULL) {
        printf("Cannot reopen\n");
        return;
    }
    printf("Read back from binary:\n");
    StudentRecord s;
    while (fread(&s, sizeof(StudentRecord), 1, f) == 1) {
        printf("  roll %d, %s, marks %d\n", s.roll, s.name, s.marks);
    }
    fclose(f);
}

int main() {
    int choice;

    printf("File Handling (Master Level C)\n\n");

    while (1) {
        printf("\n1. Write text records\n");
        printf("2. Read text records\n");
        printf("3. Append one record\n");
        printf("4. Binary write + read back (structs)\n");
        printf("5. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            writeTextRecords();
        } else if (choice == 2) {
            readTextRecords();
        } else if (choice == 3) {
            appendRecord();
        } else if (choice == 4) {
            binaryRoundTrip();
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
