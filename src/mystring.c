#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mystring.h"
#include "collection.h"

//создать строку
String* StringCreate(const char* cstr) {    
	if (!cstr) return NULL;

	String* str = CollectionCreate(GetCharFieldInfo());

	if (!str) return NULL;
	
	const char* ptr = cstr;
	while (*ptr) {
		if (!CollectionAppend(str, ptr)) {
			CollectionDestroy(str);
			return NULL;
		}
		ptr++;
	}

	return str;
}

//удалить строку
void StringDestroy(String* str) {  
	CollectionDestroy(str);
}

//найти длину строки
size_t StringLength(const String* str) {   
	return CollectionSize(str);
}

//возвращение определенного символа в строке
char StringCharAt(const String* str, size_t index) { 
	if (!str) return '\0'; 
	char *c = CollectionGet((Collection*)str, index);

	return c ? *c : '\0';
}

//выделить подстроку
String* StringSubstring(const String* str, size_t start, size_t end) { 
	if (!str) return NULL;
	return (String*)CollectionSlice(str, start, end);
}

//конкатенация двух строк
String* StringConcat(const String* str_a, const String* str_b) {   
	if (!str_a || !str_b) return NULL;
	return (String*)CollectionConcat(str_a, str_b); 
}

//разделить строку на отдельные части
Collection* StringSplit(const String* str, const char* delimiters) {  
	if (!str || !delimiters) return NULL;

	Collection* words = CollectionCreate(GetStringFieldInfo());
	if (!words) return NULL;

	size_t len_str = StringLength(str);
	if (len_str == 0) return words;

	char* temp = malloc(len_str + 1);
	if (!temp) { 
		CollectionDestroy(words);
		return NULL;
	}

	size_t len_word = 0;

	for (size_t i = 0; i < len_str; i++) {
		char c = StringCharAt(str, i);
		if (strchr(delimiters, c)) {
			if (len_word > 0) {
				temp[len_word] = '\0';
				String* word = StringCreate(temp);
				if (word) {
					if (!CollectionAppend(words, &word)) {
                        StringDestroy(word);  
                        free(temp);
                        CollectionDestroy(words);
                        return NULL;
					}
				}
				len_word = 0;
			}
		}
		else {
			temp[len_word++] = c;
		}
	}
	if (len_word > 0) {
		temp[len_word] = '\0';
		String* word = StringCreate(temp);
		if (word) {
			if (!CollectionAppend(words, &word)) {
                StringDestroy(word);  
                free(temp);
                CollectionDestroy(words);
                return NULL;
			}
		}
	}
	return words;
}

//перевод строки в символы
char* StringToCstr(const String* str) { 
	if (!str) return NULL;  

	size_t len_str = StringLength(str);
	char* cstr = malloc(len_str + 1);
	if (!cstr) return NULL;

	for (size_t i = 0; i < len_str; i++) {
		cstr[i] = StringCharAt(str, i);
	}

	cstr[len_str] = '\0';

	return cstr;
}

//вывод строки
void StringPrint(const String* str) {  
	for (size_t i = 0; i < StringLength(str); i++) {
		printf("%c", StringCharAt(str, i));
	}
}
