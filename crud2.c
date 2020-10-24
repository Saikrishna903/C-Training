//CRUD-create, read, update, delete operations on file.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define LENGTH 20
#define PRINT_STATEMENT "Name '%s' is not present in the file.\n"
#define FILE_NAME "listOfPersonNames.txt"
#define TEMP_FILE_NAME "tempListOfPersonNames.txt"
void show_menu();
char* read_name_from_user(char*);
void check_fp(FILE *);
void save_name_into_file();
void show_names_from_file();
void update_name_in_file();
void delete_name_in_file();
void main()
{
	show_menu();
}

void show_menu()
{
	int user_choice;
	while(1)
	{
		printf("\n********MENU**********\n1. Read name and save to file.\n2. Show saved names in file.\n3. Update a name in the file.\n4. Delete a name in the file.\n5. Exit.\n");
		printf("\nEnter your choice: ");
		fflush(stdin);
		scanf("%d", &user_choice);
		switch(user_choice)
		{
			case 1: save_name_into_file();
					break;
			case 2: show_names_from_file();
					break;
			case 3: update_name_in_file();
					break;
			case 4: delete_name_in_file();
					break;
			case 5:	exit(0);
			default : printf("Invalid choice.\n");
					  break;
		}
	}
}
char* read_name_from_user(char *ptr_name)
{
	printf("\nEnter a name to save into file: ");
	fflush(stdin);
	scanf("%s", ptr_name);
	return ptr_name;
}

void save_name_into_file()
{
	FILE *fp_names;
	int counter;
	char *ptr_name;
	char name[LENGTH];
	fp_names = fopen(FILE_NAME, "a");
	check_fp(fp_names);
	ptr_name = read_name_from_user(name);
	fwrite(ptr_name, LENGTH, 1, fp_names);
	printf("\nName '%s' saved to file successfully.\n", ptr_name);
	fclose(fp_names);
	return;
}
void check_fp(FILE *fp_person_names)
{
	if(fp_person_names == NULL)
	{
		printf("Error in opening the file.\n");
		exit(0);
	}	
}

void show_names_from_file()
{
	FILE *fp_names;
	char copy_of_name[LENGTH];
	int count_of_names = 0;
	fp_names = fopen(FILE_NAME, "r");
	check_fp(fp_names);
	printf("\nList of Names\n-------------\n");
	while(fread(copy_of_name, LENGTH, 1, fp_names))
	{
		count_of_names++;
		printf("%d. %s\n", count_of_names, copy_of_name);
	}
	fclose(fp_names);
}

void update_name_in_file()
{
	int name_found_status = 0, count_of_names = 0;
	char name_to_update[LENGTH], copy_of_name[LENGTH], new_name[LENGTH];
	FILE *fp_names;
	fp_names = fopen(FILE_NAME, "r+");
	printf("Enter the old name that you want to update it in the file: ");
	scanf("%s", name_to_update);
	check_fp(fp_names);
	while(fread(copy_of_name, LENGTH, 1, fp_names))
	{
		if(strcmp(copy_of_name, name_to_update) == 0)
		{
			name_found_status = 1;
			printf("Enter a new name: ");
			scanf("%s", new_name);
			fseek(fp_names, LENGTH * (-1), SEEK_CUR);
		 	fwrite(new_name, LENGTH, 1, fp_names);
		 	printf("\nUpdate successful!\n");
			break;
		 }
	}
	if(name_found_status == 0)
	{
		printf(PRINT_STATEMENT, name_to_update);
	}
	fclose(fp_names);
	return;
}

void delete_name_in_file()
{
	int counter = 0, name_found_status = 1, count_of_names = 0;
	char name_to_be_deleted[LENGTH], copy_of_name[LENGTH];
	FILE *fp_names, *temp_fp_names;
	fp_names = fopen(FILE_NAME, "r");
	printf("Enter the name you want to delete: ");
	scanf("%s", name_to_be_deleted);
	check_fp(fp_names);
   	temp_fp_names = fopen(TEMP_FILE_NAME, "w");
   	while(fread(copy_of_name, LENGTH, 1, fp_names))
   	{
		if(strcmp(copy_of_name, name_to_be_deleted) != 0)
   		{
   			name_found_status = 0;
   			fwrite(copy_of_name, LENGTH, 1, temp_fp_names);
   		}
   	}
   	fclose(fp_names);
   	fclose(temp_fp_names);
	if(name_found_status == 1)
	{
		printf(PRINT_STATEMENT, name_to_be_deleted);
		return;
	}
	else
	{
		printf("\nName is deleted from file successfully.\n");
	}
   	remove(FILE_NAME);
   	rename(TEMP_FILE_NAME, FILE_NAME);
}