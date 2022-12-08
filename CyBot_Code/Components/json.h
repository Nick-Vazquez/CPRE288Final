#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STARTING_ELEMENT_COUNT 100
#define TO_STRING_SIZE 250
#define MAX_NUM_LEN 20
#define TRUE_STR "true"
#define FALSE_STR "false"
#define NUMBER_FORMAT "%.0lf"

typedef enum json_types
{
    STRING, NUMBER, OBJECT, ARRAY, BOOL, EMPTY
} json_types_t;

typedef struct json_array
{
    int num_elements;
    json_types_t type;
    void* array;
    void* array_start;
} json_array_t;

typedef struct json_element
{
    char* name;
    json_types_t type;
    void* data;
} json_element_t;

typedef struct json_object
{
    int num_elements;
    json_element_t** elements;
    json_element_t** array_start;
} json_object_t;

typedef union json_data
{
    char* string;
    double number;
    json_object_t* object;
    json_array_t* array;
    int boolean;
} json_data_t;

json_object_t* create_object();
json_array_t* create_array(json_types_t type, void* array, int size);
void add_element(json_object_t* object, json_types_t type, char* name, void* data);
json_element_t* get_element(json_object_t* object, char* key);
json_data_t get_data(json_element_t* element);
void to_string(json_object_t* object, char** dest, int size);
void free_object(json_object_t* object);

#endif
