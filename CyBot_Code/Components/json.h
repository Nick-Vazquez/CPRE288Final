#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// General Note:
// All objects and elements are stored on the heap via a malloc statement.

// Default number of elements to be allocated upon object creation.
#define STARTING_ELEMENT_COUNT 100

// Default char count for to_string() string.
#define TO_STRING_SIZE 250

// Maximum number string length.
#define MAX_NUM_LEN 20

// Boolean string constants.
#define TRUE_STR "true"
#define FALSE_STR "false"

// sprintf format for numbers.
#define NUMBER_FORMAT "%.0lf"

// Possible JSON types.
typedef enum json_types
{
    STRING, NUMBER, OBJECT, ARRAY, BOOL, EMPTY
} json_types_t;

// Contains JSON array information.
typedef struct json_array
{
    int num_elements;
    json_types_t type;
    void* array;
    void* array_start;
} json_array_t;

// Contains JSON element information.
typedef struct json_element
{
    char* name;
    json_types_t type;
    void* data;
} json_element_t;

// Contains JSON object information.
typedef struct json_object
{
    int num_elements;
    json_element_t** elements;
    json_element_t** array_start;
} json_object_t;

// Union used here as the output to get_data(). This allows the user to specify
// how the data is interpreted.
typedef union json_data
{
    char* string;
    double number;
    json_object_t* object;
    json_array_t* array;
    int boolean;
} json_data_t;

// Creates a JSON object.
json_object_t* create_object();

// Creates a JSON array.
// Size is the array size.
json_array_t* create_array(json_types_t type, void* array, int size);

// Creates an element using the name, data, and type parameters and then adds it to
// the specified object.
void add_element(json_object_t* object, json_types_t type, char* name, void* data);

// Gets a JSON element from a specific object by the key parameter.
json_element_t* get_element(json_object_t* object, char* key);

// Returns a union of the data of the element. See the json_data_t
// struct.
json_data_t get_data(json_element_t* element);

// Converts the specified JSON object to a string and stores it into dest.
// The size param is the expected string size. This is used in recursive calls
// to this function.
void to_string(json_object_t* object, char** dest, int size);

// Runs the free() command on all JSON object, element, and array pointers.
void free_object(json_object_t* object);

#endif
