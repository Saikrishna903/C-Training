//Program to do CRUD operations using framework concept.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define FIELD_NAME_LENGTH 17
#define FIELD_VALUE_LENGTH 20
#define MENU_LIST_LENGTH 250
#define MENU_CONFIG_FILE_NAME "menu.cfg"
#define FIELDS_CONFIG_FILE_NAME "fields.cfg"
#define DATA_FILE_NAME "data.txt"
#define UPDATE_FIELDS_CONFIG_FILE_NAME "fieldstoupdate.cfg"
#define PRINT_NOT_FOUND printf("\nNo record found.\n");
void show_menu();
void create_record();
void delete_record();
void update_record();
void print_records();
void initialize_global_variables();
int get_count_of_fields();
int get_count_of_updatable_fields();
void load_field_names();
void load_updatable_fields();
void check_file_pointer(FILE*);
char **ptr_fields_name;
FILE *fp_fields;
FILE *fp_updatable_fields;
char DB_field_value[FIELD_VALUE_LENGTH];
int row_number, *ptr_updatable_fields, count_of_fields, count_of_fields_to_be_updated;

int main()
{
	initialize_global_variables();
	show_menu();
	return 0;
}

void initialize_global_variables()
{
	fp_fields = fopen(FIELDS_CONFIG_FILE_NAME, "r");
	fp_updatable_fields = fopen(UPDATE_FIELDS_CONFIG_FILE_NAME, "r");
	check_file_pointer(fp_fields);
	check_file_pointer(fp_updatable_fields);
	count_of_fields = get_count_of_fields();
	count_of_fields_to_be_updated = get_count_of_updatable_fields();
	load_field_names();
	load_updatable_fields();
	fclose(fp_fields);
	fclose(fp_updatable_fields);
	return;
}

int get_count_of_fields()
{
	char field_name[FIELD_NAME_LENGTH];
	int no_of_fields = 0;
	while(fgets(field_name, FIELD_NAME_LENGTH, fp_fields) != NULL)
	{
		no_of_fields++;
	}
	return no_of_fields;
}

void load_field_names()
{
	char field_name[FIELD_NAME_LENGTH];
	rewind(fp_fields);
	row_number = 0;
	ptr_fields_name = malloc(count_of_fields * (sizeof(char*)));
	while(fgets(field_name, FIELD_NAME_LENGTH , fp_fields) != NULL)
	{
		ptr_fields_name[row_number] = malloc(FIELD_NAME_LENGTH);
		field_name[strlen(field_name) - 1] = '\0';
		strcpy(ptr_fields_name[row_number], field_name);
		row_number++;
	}
	return;
}

int get_count_of_updatable_fields()
{
	char field_name[FIELD_NAME_LENGTH];
	int no_of_updatable_fields = 0;
	while(fgetc(fp_updatable_fields) != EOF)
	{
		no_of_updatable_fields++;
	}
	return no_of_updatable_fields;
}

void load_updatable_fields()
{
	int field_number_in_char, field_number;
	rewind(fp_updatable_fields);
	row_number = 0;
	ptr_fields_name = malloc(count_of_fields * (sizeof(char*)));
	while((field_number_in_char = fgetc(fp_updatable_fields)) != EOF)
	{
		field_number = field_number_in_char - 48;
		ptr_updatable_fields[row_number] = field_number;
		row_number++;
	}
	return;
}

void check_file_pointer(FILE *fp)
{
	if(fp == NULL)
	{
		printf("Error in opening the file.\n");
		exit(0);
	}	
}

void show_menu()
{
	char menu_list[MENU_LIST_LENGTH];
	int user_choice;
	FILE *fp_menu = fopen(MENU_CONFIG_FILE_NAME, "r");
	check_file_pointer(fp_menu);
	fread(menu_list, sizeof(menu_list), 1, fp_menu);
	fclose(fp_menu);
	menu_list[strlen(menu_list) - 1] = '\0';
	while(1)
	{
		printf("%s", menu_list);
		printf("Enter your choice: ");
		fflush(stdin);
		scanf("%d", &user_choice);
		switch(user_choice)
		{
			case 1:	create_record();
					break;
			case 2: print_records();
					break;
			case 3: update_record();
					break;
			case 4: delete_record();
					break;
			case 5: exit(0);
			default:printf("INVALID CHOICE.\n");
					break;
		}
	}
	return;
}

void create_record()
{
	char user_given_field_value[FIELD_VALUE_LENGTH], record_status = 'A'; 
	row_number = 0;
	FILE *fp_data = fopen(DATA_FILE_NAME, "a");
	check_file_pointer(fp_data);
	fwrite(&record_status, sizeof(record_status), 1, fp_data);
	while(row_number < count_of_fields)
	{
		printf("Enter %s : ", ptr_fields_name[row_number]);
		fflush(stdin);
		scanf("%s", user_given_field_value);
		fwrite(user_given_field_value, FIELD_VALUE_LENGTH, 1, fp_data);
		row_number++;			
	} 
	printf("\nDetails are saved successfully.\n");
	fclose(fp_data);
	return;
}

void print_records()
{
	int no_of_records = 0;
	char record[count_of_fields * FIELD_VALUE_LENGTH], existing_record_status;
	FILE *fp_data = fopen(DATA_FILE_NAME, "r");
	check_file_pointer(fp_data);
	while(fread(record, sizeof(record), 1, fp_data))
	{
		fseek(fp_data, sizeof(record) * (-1), SEEK_CUR);
		fread(&existing_record_status, sizeof(existing_record_status),1, fp_data);
		if(existing_record_status == 'A')
		{
				no_of_records++;
				row_number = 0;
				printf("\nRecord %d: \n", no_of_records);
				while(row_number < count_of_fields)
				{
					fread(DB_field_value, FIELD_VALUE_LENGTH, 1, fp_data);
					printf("%-20s: %s\n", ptr_fields_name[row_number], DB_field_value);
					row_number++;
				}
		}
		else
		{
			fseek(fp_data, sizeof(record), SEEK_CUR);
		}
	}
	printf("\nNumber of records present: %d\n", no_of_records);
	fclose(fp_data);
	return;
}

void delete_record()
{
	char field_value_to_delete_record[FIELD_VALUE_LENGTH], record_status;
	int delete_status = 0;
	FILE *fp_data = fopen(DATA_FILE_NAME, "r+");
	check_file_pointer(fp_data);
	row_number = 0,
	printf("Enter %s to delete: ", ptr_fields_name[row_number]);
	scanf("%s", field_value_to_delete_record);
	while(fread(&record_status, sizeof(record_status), 1, fp_data))
	{
		if(record_status == 'A')
		{
				fread(DB_field_value, FIELD_VALUE_LENGTH , 1, fp_data);
				if(strcmp(DB_field_value, field_value_to_delete_record) == 0)
				{
					record_status = 'I';
					delete_status = 1;
					fseek(fp_data, (sizeof(DB_field_value) + 1) * (-1), SEEK_CUR);
					fwrite(&record_status, sizeof(record_status), 1, fp_data);
					fseek(fp_data, (-1), SEEK_CUR);
					break;
				}
				fseek(fp_data, sizeof(DB_field_value) * (count_of_fields - 1), SEEK_CUR);
		}
		else
		{
			fseek(fp_data, (count_of_fields * FIELD_VALUE_LENGTH), SEEK_CUR);
		}
	}
	if(delete_status == 0)
	{
		PRINT_NOT_FOUND
	}
	else
	{
		printf("\nDeletion is successful.\n");
	}
	fclose(fp_data);
	return;
}

void update_record()
{
	int update_option, update_status = 0;
	char field_value_to_update[FIELD_VALUE_LENGTH], record_status;
	char new_field_value[FIELD_VALUE_LENGTH];
	FILE *fp_data = fopen(DATA_FILE_NAME, "r+");
	check_file_pointer(fp_data);
	row_number = 0;
	printf("\nDo you want to update: \n");
	while(row_number < count_of_fields_to_be_updated)
	{
		printf("%d. %s\n", row_number + 1, ptr_fields_name[ptr_updatable_fields[row_number] - 1]);
		row_number++;
	}
	fflush(stdin);
	printf("Enter your option: ");
	scanf("%d", &update_option);
	row_number = 0;
	printf("Enter %s to update: ", ptr_fields_name[row_number]);
	scanf("%s", field_value_to_update);

	while(fread(&record_status, sizeof(record_status), 1, fp_data))
	{
		if(record_status == 'A')
		{
				fread(DB_field_value, FIELD_VALUE_LENGTH , 1, fp_data);
				if(strcmp(DB_field_value, field_value_to_update) == 0)
				{
					update_status = 1;
					printf("Enter new %s: ", ptr_fields_name[ptr_updatable_fields[(update_option - 1)] - 1]);
					scanf("%s", new_field_value);
					fseek(fp_data, FIELD_VALUE_LENGTH * ((ptr_updatable_fields[(update_option - 1)]) - 2), SEEK_CUR);
					fwrite(new_field_value, sizeof(new_field_value), 1, fp_data);
					break;
				}
				fseek(fp_data, sizeof(DB_field_value) * (count_of_fields - 1), SEEK_CUR);
		}
		else
		{
			fseek(fp_data, (count_of_fields * FIELD_VALUE_LENGTH), SEEK_CUR);
		}
	}
	if(update_status == 0)
	{
		PRINT_NOT_FOUND
	}
	else
	{
		printf("\nUpdate successful\n");
	}
	fclose(fp_data);
	return;
}
