#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Employee* EmpType;
typedef struct newEmployee* NewEmpType;

struct Employee {

    int id;

    char name[50];

    float salary;

    EmpType next;

};

// I've cahnged your definition a bit.. (pointer to incomplete type etc.)
typedef struct newEmployee {

    int id;

    char name[50];

    float salary;

    int age;

} newEmployee;

EmpType get_employee() {
    EmpType emp = (EmpType)malloc(sizeof(struct Employee));
    if (emp == NULL) {
        perror("Alocation Failed\n");
        exit(1); // memory leak...
    }
    emp->next = NULL;

    printf("Enter id:\n");
    scanf_s("%d", &emp->id);
    getchar(); // enter
    printf("Enter name:\n");
    fgets(emp->name, 50, stdin);

    if (strcmp(emp->name, "q\n") == 0) {
        free(emp);
        return NULL;
    }

    emp->name[strlen(emp->name) - 1] = '\0'; // '\n' 
    printf("Enter salary:\n");
    scanf_s("%f", &emp->salary);
    getchar(); // enter

    return emp;
}

void update_file(EmpType head) {
    FILE* fd;
    fopen_s(&fd, "employees.bin", "wb");
    if (fd == NULL) {
        perror("Cannot open file");
        exit(1); // memory leak...
    }

    while (head) {
        if (fwrite(head, sizeof(struct Employee), 1, fd) != 1) {
            perror("Error writing to file");
            break;
        }        
        head = head->next;
    }
    fclose(fd);
}


void add_emp(struct Employee** head, EmpType emp) {
    struct Employee* last = *head;

    // Empty list
    if (last == NULL) {
        *head = emp;
    }
    else {
        while (last && last->next) {
            last = last->next;
        }
        last->next = emp;
    } 
}

void create_list(struct Employee** head) {
    while (1) {
        EmpType emp = get_employee();

        if (emp == NULL)
            break;

        add_emp(head, emp);
        update_file(*head);
    }
}

void free_list(EmpType head) {
    if (head == NULL)
        return;

    EmpType next;
    
    while (head) {
        next = head->next;
        free(head);
        head = next;
    }
}

void load_list_from_file(struct Employee** head) {
    FILE* fd;
    fopen_s(&fd, "employees.bin", "rb");
    if (fd == NULL) {
        perror("Cannot open file");
        exit(1); // memory leak...
    }

    EmpType emp;
    while (1) {
        emp = (EmpType)malloc(sizeof(struct Employee));
        if (emp == NULL) {
            perror("Allocation Failed\n");
            exit(1);
        }

        if (fread(emp, sizeof(struct Employee), 1, fd) != 1) {
            free(emp); 
            break;
        }

        emp->next = NULL;
        add_emp(head, emp);
    }

    fclose(fd);
}

EmpType load_emp(int index) {
    FILE* fd;
    fopen_s(&fd, "employees.bin", "rb");
    if (fd == NULL) {
        perror("Cannot open file");
        exit(1); // memory leak...
    }

    if (fseek(fd, sizeof(struct Employee) * index, SEEK_SET) != 0) {
        perror("Error seeking to second struct");
        fclose(fd);
        exit(1);
    }

    EmpType emp = (EmpType)malloc(sizeof(struct Employee));

    if (fread(emp, sizeof(struct Employee), 1, fd) != 1) {
        perror("Error seeking to second struct");
        fclose(fd);
        free(emp);
        exit(1);
    }

    fclose(fd);
    return emp;
}

void update_file_new(NewEmpType emp) {
    FILE* fd;
    fopen_s(&fd, "employees_new.bin", "ab");
    if (fd == NULL) {
        perror("Cannot open file");
        exit(1); // memory leak...
    }

    if (fwrite(emp, sizeof(newEmployee), 1, fd) != 1)
        perror("Error writing to file");
    fclose(fd);
}

void load_newEmployee_from_file() {
    FILE* fd;
    fopen_s(&fd, "employees.bin", "rb");
    if (fd == NULL) {
        perror("Cannot open file");
        exit(1); // memory leak...
    }

    EmpType emp;
    NewEmpType newEmp;
    while (1) {
        emp = (EmpType)malloc(sizeof(struct Employee));
        newEmp = (NewEmpType)malloc(sizeof(newEmployee));

        if (emp == NULL || newEmp == NULL) {
            perror("Allocation Failed\n");
            exit(1);
        }

        if (fread(emp, sizeof(struct Employee), 1, fd) != 1) {
            free(emp);
            free(newEmp);
            break;
        }

        newEmp->id = emp->id;
        strcpy_s(newEmp->name, 50, emp->name);
        newEmp->salary = emp->salary;
        printf("Enter employee age:\n");
        scanf_s("%d", &newEmp->age);
        
        update_file_new(newEmp);
    }

    fclose(fd);
}

int main()
{
    // ******** !!! NOTE !!! *********
    // In this Exercise each qeustion is relyng on code that
    // belongs to previues questions. so all the code has 
    // to be uncommented AT THE SAME TIME
    

    // ******** Q1 *********
    EmpType head = NULL;

    create_list(&head);
    free_list(head);


    // ******** Q2 *********
    head = NULL;
    load_list_from_file(&head);
    free_list(head);


    // ******** Q3 *********
    int index = 0;

    printf("Enter employee index (starting at 0):\n");
    scanf_s("%d", &index);

    // ! We assume that the index exist, otherwise the program will exit
    EmpType emp = load_emp(index);
    if (emp)
        printf("ID: %d, Name: %s, Salary: %.2f\n", emp->id, emp->name, emp->salary);


    // ******** Q4 *********
    // I wanted to create a list the same way we did with the 
    // the old Employee struct but i was in a rush...
    load_newEmployee_from_file();

    // ******** Q4 *********
    // 1. There isnt a test to see if fopen succeeded
    // 2. There is no limitation on name in fscanf, it should be fscanf(... %19s ...)
    // 3. ?



    return 0;
}
