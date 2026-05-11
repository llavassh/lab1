#pragma once

#include <stddef.h>
#include "collection.h"

typedef struct Collection String;

String* StringCreate(const char* cstr);
void StringDestroy(String* str);

size_t StringLength(const String* str);
char StringCharAt(const String* str, size_t index);

String* StringSubstring(const String* str, size_t start, size_t end);
String* StringConcat(const String* str_a, const String* str_b);
Collection* StringSplit(const String* str, const char* delimiters);

char* StringToCstr(const String* str);
void StringPrint(const String* str);