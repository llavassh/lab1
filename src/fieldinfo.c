#include <stdlib.h>
#include <stdio.h>

#include "fieldinfo.h"
#include "mystring.h"

static FieldInfo* CharInfo = NULL;
static FieldInfo* StringInfo = NULL;

//char 
static void CharPrint(const void* data) {
	if (data) {
		printf("%c", *(const char*)data);
	}
}

static void CharCopy(void* dest, const void* src) {
	if (dest && src) {
		*(char*)dest = *(char*)src;
	}
}

static void CharDestroy(void* data) {
	(void)data;
}

//string
static void StringPtrPrint(const void* data) {
    if (data) {
        String* str = *(String**)data;
        if (str) {
            StringPrint(str);
        }
    }
}

static void StringPtrCopy(void* dest, const void* src) {
    if (dest && src) {
        String* original = *(String**)src;
        if (original) {
            char* temp = StringToCstr(original); //копирование дважды, так как StringToCstr возвращает массив char, а не строку
            if (temp) {
                String* copy = StringCreate(temp);
                free(temp);
                *(String**)dest = copy;
            } else {
                *(String**)dest = NULL;
            }
        } else {
            *(String**)dest = NULL;
        }
    }
}

static void StringPtrDestroy(void* data) {
    if (data) {
        String* str = *(String**)data;
        if (str) {
            StringDestroy(str);
        }
        *(String**)data = NULL;
    }
}

FieldInfo* GetCharFieldInfo(void) {
    if (!CharInfo) {
        CharInfo = (FieldInfo*)malloc(sizeof(FieldInfo));
        if (CharInfo) {
            CharInfo->size = sizeof(char);
            CharInfo->print = CharPrint;
            CharInfo->copy = CharCopy;
            CharInfo->destroy = CharDestroy;
        }
    }
    return CharInfo;
}

FieldInfo* GetStringFieldInfo(void) {
    if (!StringInfo) {
        StringInfo = (FieldInfo*)malloc(sizeof(FieldInfo));
        if (StringInfo) {
            StringInfo->size = sizeof(String*);
            StringInfo->print = StringPtrPrint;
            StringInfo->copy = StringPtrCopy;
            StringInfo->destroy = StringPtrDestroy;
        }
    }
    return StringInfo;
}