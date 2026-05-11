#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fieldinfo.h"
#include "collection.h"
#include "mystring.h"

//очищает ввод от ненужных символов
static void ClearInput(void) { 
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

//читает строки
static char* ReadLine(const char* prompt) {  
	printf("%s", prompt);

	char buffer[1024];
	if (fgets(buffer, sizeof(buffer), stdin)) {
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n') {
			buffer[len - 1] = '\0';
		}

		char* result = malloc(len + 1);
		if (result) {
			strcpy(result, buffer);
		}
		return result;
	}
	return NULL;
}

//выводит коллекцию строк через запятую
static void PrintStringCollection(Collection* col) {   
	if (!col) {
		printf("NULL");
		return;
	}

	printf("[");

	for (size_t i = 0; i < CollectionSize(col); i++) {
		String** str_ptr = CollectionGet(col, i);
		if (str_ptr && *str_ptr) {
			StringPrint(*str_ptr);
		}
		else {
			printf("NULL");
		}
		if (i < CollectionSize(col) - 1) {
			printf(", ");
		}
	}
	printf("]");
}

//удаляет строки
static void CleanUp(String** str1, String** str2) {  
	if (*str1) {
		StringDestroy(*str1);
		*str1 = NULL;
	}
	if (*str2) {
		StringDestroy(*str2);
		*str2 = NULL;
	}
}

int main() {
	String* str1 = NULL;
	String* str2 = NULL;
	int choice;
	do {
		printf("\n");
        printf("+--------------------------------------+\n");
        printf("|     LAB WORK #1 - VARIANT 25        |\n");
        printf("|     Polymorphic Collection - String |\n");
        printf("+--------------------------------------+\n");
        printf("| 1. Set main string                  |\n");
        printf("| 2. Set second string                |\n");
        printf("| 3. Show main string                 |\n");
        printf("| 4. Show second string               |\n");
        printf("| 5. Concatenate strings              |\n");
        printf("| 6. Get substring                    |\n");
        printf("| 7. Split into words                 |\n");
        printf("| 8. Clear all strings                |\n");
        printf("| 0. Exit                             |\n");
        printf("+--------------------------------------+\n");
        printf("Choice: ");

		if (scanf("%d", &choice) != 1) {
			printf("Input error\n");
			ClearInput();
			continue;
		}
		ClearInput();

		switch (choice) {
		case 1:
		{
			char* input = ReadLine("Enter the first string: ");

			if (str1) {
				StringDestroy(str1);
			}
			str1 = StringCreate(input);
			if (str1) {
    			printf("First string saved: ");
    			StringPrint(str1);
    			printf(" (length: %zu)\n", StringLength(str1));
				} else {
    			printf("Error: Failed to create string\n");
				}
			free(input);
			break;
		}

		case 2:
		{
			char* input = ReadLine("Enter the second string: ");

			if (str2) {
				StringDestroy(str2);
			}

			str2 = StringCreate(input);
			if (str2) {
    			printf("Second string saved: ");
    			StringPrint(str2);
    			printf(" (length: %zu)\n", StringLength(str2));
				} else {
    			printf("Error: Failed to create string\n");
				}
			free(input);
			break;
		}

		case 3:
		{
			printf("Main string: ");

			if (str1) {
				StringPrint(str1);
				printf("(length: %zu)", StringLength(str1));
			}
			else {
				printf("String not set");
			}

			break;
		}

		case 4:
		{
			printf("Second string: ");

			if (str2) {
				StringPrint(str2);
				printf("(length: %zu)", StringLength(str2));
			}
			else {
				printf("String not set");
			}

			break;
		}

		case 5:
		{
			if (!str1 || !str2) {
				printf("Error: Both strings must be specified\n");
				break;
			}

			String* result = StringConcat(str1, str2);

			if (result) {
				printf("The result of concatenation: ");
				StringPrint(result);
				printf("\n");
				StringDestroy(result);
			}
			else {
				printf("Error while concatenating\n");
			}

			break;
		}

		case 6:
		{
			if (!str1) {
				printf("Error: string not set\n");
				break;
			}

			size_t start, end;

			printf("Enter starting index: ");
			if (scanf("%zu", &start) != 1) {
				printf("Input error\n");
				ClearInput();
				break;
			}

			printf("Enter ending index (not including): ");
			if (scanf("%zu", &end) != 1) {
				printf("Input error\n");
				ClearInput();
				break;
			}
			ClearInput();

			String* sub = StringSubstring(str1, start, end);
			if (sub) {
				printf("Substring [%zu, %zu]: ", start, end);
				StringPrint(sub);
				printf("\n");
				StringDestroy(sub);
			}
			else {
				printf("Error: Invalid indexes (string length: %zu)\n", StringLength(str1));
			}

			break;
		}

		case 7:
		{
			if (!str1) {
				printf("Error: string not set\n");
				break;
			}
			char* delimiters = ReadLine("Enter delimiters (e.g. ' ,.!?;:'): ");
			if (!delimiters) break;

			Collection* words = StringSplit(str1, delimiters);
			if (words) {
				printf("Split result: ");
				PrintStringCollection(words);
				printf("\n");
				CollectionDestroy(words);
			}
			else {
				printf("Error while splitting\n");
			}

			free(delimiters);
			break;
		}

		case 8: 
		{
			CleanUp(&str1, &str2);
			printf("All strings cleared\n");
			break;
		}

		case 0:
		{
			printf("Exit the program\n");
			break;
		}

		}
	}
	while (choice != 0);
	CleanUp(&str1, &str2);

	return 0;
}