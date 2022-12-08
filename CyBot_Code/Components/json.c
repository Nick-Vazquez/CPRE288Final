#include "json.h"

/*
    General Note:
    All elements added using add_element() must have the data parameter's
    address by using the & operator, even if this data is an array.
    This is to stay consistent because we could be adding just a plain
    number or a triple nested array of arrays.

    However, for create_array() the data can be passed simply as a pointer
    because only arrays will be passed in. There is not a mix of primative
    and pointer here, unlike add_element() which has a mix of primative and pointer.
*/


// The double pointer is so that the pointer address will update, no need to return the address
void append_char(char** dest, char c)
{
    *(*dest) = c;

    (*dest)++;
}

void append_string(char** dest, char* str)
{
    while(*str != 0)
    {
        append_char(dest, *str);
        str++;
    }
}

// Utility to add the following to the destination string: "<key name>":
void append_key(char** dest, json_element_t* element)
{
    append_char(dest, '\"');
    append_string(dest, element->name);
    append_char(dest, '\"');
    append_char(dest, ':');
}

json_object_t* create_object()
{
    json_object_t* root = malloc(sizeof(json_object_t));

    root->elements = malloc(sizeof(json_element_t*) * STARTING_ELEMENT_COUNT);

    root->array_start = root->elements;

    root->num_elements = 0;

    return root;
}

void next_index(json_array_t* array)
{
    switch(array->type)
    {
        // Evaluated as double pointer
        case NUMBER:
        {
            // The pointer has to be treated as a number here to do the increment, then it is cast back to a pointer.
            // For some reason, the C compiler will not allow a direct cast to the appropriate pointer and then the ++ operation.
            array->array = (void*) (((int) (array->array)) + sizeof(double));
            break;
        }

        // Evaluated as integer pointer
        case BOOL:
        {
            array->array = (void*) (((int) (array->array)) + sizeof(int));
            break;
        }

        // There is no data to iterate over if null
        case EMPTY:
        {
            break;
        }

        // All others are evaluated as double pointers (**) which all have the same size
        // Therefore, the type does not matter, let's choose char
        default:
        {
                array->array = (void*) (((int) (array->array)) + sizeof(char*));
            break;
        }

    }
}

// Helper function to free arrays, may call itself
void free_array(json_array_t* array)
{

    if(array->type == ARRAY || array->type == OBJECT)
    {
        int i;
        for(i = 0; i < array->num_elements; ++i)
        {
            if(array->type == ARRAY)
            {
                // Need to treat as a two-star pointer here since this is an array of arrays
                free_array((json_array_t*) *((void**) array->array));
            }
            // Must be OBJECT
            else
            {
                // Need to treat as a two-star pointer here since this is an array of objects
                free_object((json_object_t*) *((void**) array->array));
            }

            next_index(array);
        }
    }

    free(array);
}

// This will not free the actual data elements, only the object and element structs
void free_object(json_object_t* object)
{
    // Get all elements, run this function again on any objects, iterate
    // all arrays of objects too

    int i;
    for(i = 0; i < object->num_elements; ++i)
    {
        json_element_t* element = *(object->elements);

        if(element->type == OBJECT)
        {
            free_object(get_data(element).object);
        }
        else if(element->type == ARRAY)
        {
            json_array_t* array = get_data(element).array;

            free_array(array);
        }

        // All nested objects are freed, now free the element
        free(element);

        // Go to the next element
        (object->elements)++;
    }
    
    // Now that all elements are freed, free the element array and object
    free(object->elements);
    free(object);
}

json_array_t* create_array(json_types_t type, void* array, int size)
{
    json_array_t* arr = malloc(sizeof(json_array_t));
    
    arr->type = type;
    arr->array = array;
    arr->array_start = array;
    arr->num_elements = size;

    return arr;
}

// TODO: Add array bound checking here
void add_element(json_object_t* object, json_types_t type, char* name, void* data)
{
    json_element_t* element = malloc(sizeof(json_element_t));

    element->name = name;
    element->type = type;
    element->data = data;

    (object->elements) += object->num_elements;

    *(object->elements) = element;

    object->num_elements++;

    object->elements = object->array_start;
}

json_element_t* get_element(json_object_t* object, char* key)
{
    json_element_t* result;

    int i;
    for(i = 0; i < object->num_elements; ++i)
    {
        if(!strcmp(key, (*(object->elements))->name))
        {
            result = *(object->elements);

            object->elements = object->array_start;

            return result;
        }

        (object->elements)++;
    }
    object->elements = object->array_start;

    return 0;
}

void write_element_data(char** dest, json_element_t* element, int size)
{
        switch(element->type)
        {
            case NUMBER:
            {
                char str_num[MAX_NUM_LEN];

                int j;
                for(j = 0; j < MAX_NUM_LEN; ++j)
                {
                    str_num[j] = 0;
                }

                // Write the double to a string
                sprintf(str_num, NUMBER_FORMAT, get_data(element).number);
                append_string(dest, str_num);

                break;
            }

            case OBJECT:
            {
                char* str = malloc(sizeof(char) * size);
                to_string(get_data(element).object, &str, size);
                append_string(dest, str);
                free(str);
                break;
            }

            case STRING:
            {
                append_char(dest, '\"');
                append_string(dest, get_data(element).string);
                append_char(dest, '\"');
                break;
            }

            case BOOL:
            {
                // String literals are fun :D
                append_string(dest, get_data(element).boolean ? TRUE_STR : FALSE_STR);
                break;
            }

            // Null is a reserved constant, EMPTY is used instead
            case EMPTY:
            {
                append_string(dest, "null");
                break;
            }

            case ARRAY:
            {
                json_array_t* arr = get_data(element).array;
                int j;
                append_char(dest, '[');

                for(j = 0; j < arr->num_elements; ++j)
                {

                    // Package data into a temp element
                    json_element_t temp;
                    temp.name = "temp";
                    temp.type = arr->type;
                    temp.data = arr->array;

                    write_element_data(dest, &temp, size);

                    // Add comma if not the last element in the array
                    if(j < (arr->num_elements) - 1)
                    {
                        append_char(dest, ',');
                    }

                    next_index(arr);

                }

                // Return array index back to start
                arr->array = arr->array_start;

                append_char(dest, ']');
                break;
            }
        }
}

void to_string(json_object_t* object, char** dest, int size)
{
    // Keeps track of starting location for the pointer
    char* dest_start = *dest;

    int i;
    for(i = 0; i < size; ++i)
    {
        append_char(dest, 0);
    }
    *dest = dest_start;

    append_char(dest, '{');

    for(i = 0; i < object->num_elements; ++i)
    {
        append_key(dest, *(object->elements));

        write_element_data(dest, *(object->elements), size);

        // Add comma if not the last element in the element array
        if(i < (object->num_elements) - 1)
        {
            append_char(dest, ',');
        }

        (object->elements)++;
    }

    append_char(dest, '}');

    object->elements = object->array_start;

    *dest = dest_start;
}

json_data_t get_data(json_element_t* element)
{
    json_data_t result;

    switch(element->type)
    {
        case STRING:
        {
            result.string = *((char**) element->data);
            break;
        }
        
        case NUMBER:
        {
            result.number = (double) *((double*) element->data);
            break;
        }

        case OBJECT:
        {
            result.object = *((json_object_t**) element->data);
            break;
        }

        case ARRAY:
        {
            result.array = *((json_array_t**) element->data);
            break;
        }

        case BOOL:
        {
            int data = *((int*) element->data);

            result.boolean = data ? 1 : 0;

            break;
        }
    }

    return result;
}
