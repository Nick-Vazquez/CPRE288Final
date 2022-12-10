#include <Components/object_scan.h>

// Converts radial to linear width.
float radial_to_linear_width(int angle, float distance)
{
    return (2 * distance * sin((PI/180) * (angle / 2)));
}

// Gets the prvious object count index. Returns zero
// if no objects are recorded.
int previous_object_count_index(int object_count)
{
    if(object_count > 0)
    {
        return object_count - 1;
    }
    else
    {
        return 0;
    }
}

// Filter for if the provided distance is within the defined range.
int dist_in_range(int dist)
{
    if(dist <= farthest_distance && dist >= SHORTEST_DISTANCE)
    {
        return 1;
    }

    return 0;
}

float compute_sample_average_float(float* samples, int num_samples)
{
    float total = 0;

    int i;
    for(i = 0; i < num_samples; ++i)
    {
        total += *samples;
        samples++;
    }

    return (total/(float)num_samples);
}

int compute_sample_average_int(int* samples, int num_samples)
{
    int total = 0;

    int i;
    for(i = 0; i < num_samples; ++i)
    {
        total += *samples;
        samples++;
    }

    return (total/num_samples);
}

// Determines if sequential samples are within the defined distance bounds.
float samples_within_tolerance(float* samples, int num_samples, float tolerance, float farthest_distance, float shortest_distance)
{
    int i;
    float current_sample;
    float next_sample;

    for(i = 0; i < (num_samples - 1); ++i)
    {
        current_sample = *samples;
        samples++;
        next_sample = *samples;

        if((fabs(current_sample - next_sample) > tolerance) || current_sample > farthest_distance || current_sample < shortest_distance)
        {
            return 0;
        }

    }

    return 1;
}

object_t get_smallest_object(objects_data_t data)
{
    object_t result = data.objects[0];

    int i;
    for(i = 1; i < data.object_count; ++i)
    {
        if(data.objects[i].linear_width < result.linear_width)
        {
            result = data.objects[i];
        }
    }

    return result;
}

// All JSON global declarations.
object_t* objects;
object_t* objects_start;

scan_data_t* scan_data;
scan_data_t* scan_data_start;
json_object_t* root;
double message_type;
char* sensor_select;
double* raw_json_angles;
double* raw_json_distances;

json_array_t* json_angles;
json_array_t* json_distances;

char* str;

void object_scan_alloc()
{
    objects = malloc(sizeof(object_t) * MAX_OBJECTS);
    objects_start = objects;

    scan_data = malloc(sizeof(scan_data_t) * (180/ANGLE_DELTA));
    scan_data_start = scan_data;

    sensor_select = malloc(5);
    raw_json_angles = malloc(sizeof(double) * 180/ANGLE_DELTA);
    raw_json_distances = malloc(sizeof(double) * 180/ANGLE_DELTA);

    json_angles = create_array(NUMBER, raw_json_angles, (180/ANGLE_DELTA));
    json_distances = create_array(NUMBER, raw_json_distances, (180/ANGLE_DELTA));

    int i;
    for(i = 0; i < (180/ANGLE_DELTA); ++i)
    {
        raw_json_angles[i] = 0;
        raw_json_distances[i] = 0;
    }

    message_type = 4;
    strcpy(sensor_select, "IR");

    root = create_object();
    add_element(root, NUMBER, "mes_type", &message_type);
    add_element(root, STRING, "update_type", &sensor_select);
    add_element(root, ARRAY, "angles", &json_angles);
    add_element(root, ARRAY, "distances", &json_distances);

    str = malloc(JSON_STRING_SIZE);

    for(i = 0; i < JSON_STRING_SIZE; ++i)
    {
        str[i] = 0;
    }
}

void object_scan_free()
{
    free(objects);
    free(scan_data);
    free(str);
}

// This will get the average_count largest distances and then return the average of them
float get_largest_average_distance(int start_angle, int end_angle, int average_count)
{
    int size = (start_angle - end_angle) / ANGLE_DELTA;

    float values[size];
    float sum = 0;
    float largest_values[average_count];
    int i = 0;
    int j = 0;

    servo_move(start_angle);
    timer_waitMillis(2000);

    // Fill arrays with zeros
    for(i = 0; i < size; ++i)
    {
        values[i] = 0;
    }

    for(i = 0; i < average_count; ++i)
    {
        largest_values[i] = 0;
    }

    int angle = start_angle;
    int target_time = 0;

    while(i < size)
    {
        if(timer_getMillis() >= target_time)
        {
            angle -= ANGLE_DELTA;

            // Servo angle only needs to be set once
            servo_move(angle);

            values[i] = calculate_ir_distance(ir_read());

            target_time = timer_getMillis() + TIME_DELTA;
            i++;
        }

    }

    // Get the largest
    for(i = 0; i < size; ++i)
    {
        if(values[i] > largest_values[0])
        {
            largest_values[0] = values[i];
        }
    }


    // Get average_count largest distances
    for(i = 1; i < average_count; ++i)
    {
        for(j = 0; j < size; ++j)
        {
            if(values[j] > largest_values[i] && values[j] < largest_values[i - 1])
            {
                largest_values[i] = values[j];
            }
        }
    }

    for(i = 0; i < average_count; ++i)
    {
        sum += largest_values[i];
    }

    return (sum / average_count);
}

// This will get the average_count smallest distances and then return the average of them
float get_smallest_average_distance(int start_angle, int end_angle, int average_count)
{
    int size = (start_angle - end_angle) / ANGLE_DELTA;

    float values[size];
    float sum = 0;
    float smallest_values[average_count];
    int i = 0;
    int j = 0;

    servo_move(start_angle);
    timer_waitMillis(2000);

    // Fill arrays with large numbers
    for(i = 0; i < size; ++i)
    {
        values[i] = 1000000;
    }

    for(i = 0; i < average_count; ++i)
    {
        smallest_values[i] = 1000000;
    }

    int angle = start_angle;
    int target_time = 0;

    while(i < size)
    {
        if(timer_getMillis() >= target_time)
        {
            angle -= ANGLE_DELTA;

            // Servo angle only needs to be set once
            servo_move(angle);

            values[i] = calculate_ir_distance(ir_read());

            target_time = timer_getMillis() + TIME_DELTA;
            i++;
        }

    }

    // Get the smallest
    for(i = 0; i < size; ++i)
    {
        if(values[i] < smallest_values[0])
        {
            smallest_values[0] = values[i];
        }
    }


    // Get average_count smallest distances
    for(i = 1; i < average_count; ++i)
    {
        for(j = 0; j < size; ++j)
        {
            if(values[j] < smallest_values[i] && values[j] > smallest_values[i - 1])
            {
                smallest_values[i] = values[j];
            }
        }
    }

    for(i = 0; i < average_count; ++i)
    {
        sum += smallest_values[i];
    }

    return (sum / average_count);
}

// start_angle > end_angle
objects_data_t scan(int start_angle, int end_angle, int silent)
{

    objects_data_t result;

    // Flag to see if we can detect a new object
    int new_object = 1;

    float samples[DATA_BLOCK_SIZE];

    float ir_readings_scan[NUM_READINGS_IR_SCAN];

    float previous_samples[DATA_BLOCK_SIZE];

    float object_distances[100];

    int i = 0;
    int j = 0;

    // -1 means no angle detected yet
    int object_detection_start_angle = -1;

    int object_count = 0;
    int stored_object_count = 0;

    int room_number = 0;

    int target_time = 0;

    int angle = start_angle;

    // Start back at the start of the array
    objects = objects_start;

    // Clear out objects
    for(i = 0; i < MAX_OBJECTS; ++i)
    {
        objects->distance = 0;
        objects->median_angle = 0;
        objects->radial_width = 0;
        objects->linear_width = 0;
        objects->start_angle = 0;
        objects->room_number = 0;

        objects++;
    }

    // Start back at the start of the array
    scan_data = scan_data_start;

    objects = objects_start;

    for(i = 0; i < 100; ++i)
    {
        object_distances[i] = 0;
    }

    for(i = 0; i < DATA_BLOCK_SIZE; ++i)
    {
        samples[i] = 0;
        previous_samples[i] = 0;
    }

    for(i = 0; i < NUM_READINGS_IR_SCAN; ++i)
    {
        ir_readings_scan[i] = 0;
    }

    if(!silent)
    {
        sprintf(str, "%-15s%+15s\n\r", "Degrees", "Distance (cm)");

        uart_sendStr(str);
    }


    servo_move(start_angle);

    // Wait for servo to move
    timer_waitMillis(750);

    // Record first reading
    scan_data->angle = start_angle;
    scan_data->ir_distance = calculate_ir_distance(ir_read());
    raw_json_angles[0] = (double) start_angle;
    raw_json_distances[0] = (double) scan_data->ir_distance;
    scan_data++;


    // Reset i to start at 0 for the while loop
    i = 0;

    while(i < ((start_angle - end_angle) / ANGLE_DELTA))
    {
        if(timer_getMillis() >= target_time)
        {
            angle -= ANGLE_DELTA;

            // Servo angle only needs to be set once
            servo_move(angle);
            for(j = 0; j < NUM_READINGS_IR_SCAN; ++j)
            {

                ir_readings_scan[j] = calculate_ir_distance(ir_read());
            }

            scan_data->angle = angle;
            scan_data->ir_distance = compute_sample_average_float(ir_readings_scan, NUM_READINGS_IR_SCAN);

            raw_json_angles[i] = (double) scan_data->angle;
            raw_json_distances[i] = (double) scan_data->ir_distance;

            if(!silent)
            {
                sprintf(str, "%-15d%+15.3f\n\r", angle, scan_data->ir_distance);
                uart_sendStr(str);
            }

            scan_data++;

            target_time = timer_getMillis() + TIME_DELTA;
            i++;
        }

    }

    to_string(root, &str, JSON_STRING_SIZE);
    uart_sendStr(str);

    // Start at the beginning of the array
    objects = objects_start;

    // Start at the beginning of the array
    scan_data = scan_data_start;

    // Find object distances and count objects
    // This is a rough count of objects, the code will re-iterate and check the actual values, one-by-one to see if they match
    // the average discovered here. If the individual readings don't match up, the object will not be saved but the "object count" will
    // remain the same. Checks are also done to ensure that a certain width is present. If a width of 0.1 is detected for example, that object
    // should be thrown away.
    for(i = 0; i < ((start_angle - end_angle) / ANGLE_DELTA / DATA_BLOCK_SIZE); ++i)
    {

        j = 0;

        for(j = 0; j < DATA_BLOCK_SIZE; ++j)
        {
            previous_samples[j] = samples[j];
            samples[j] = scan_data->ir_distance;
            scan_data++;
        }

        if(fabs(compute_sample_average_float(previous_samples, DATA_BLOCK_SIZE) - compute_sample_average_float(samples, DATA_BLOCK_SIZE)) > DIFFERENT_OBJECT_TOLERANCE)
        {
            new_object = 1;
        }

        // If within the tolerance and we have not already recorded the object, record the object
        if(samples_within_tolerance(samples, DATA_BLOCK_SIZE, DISTANCE_TOLERANCE, farthest_distance, SHORTEST_DISTANCE) && new_object)
        {
            new_object = 0;
            object_distances[object_count] = compute_sample_average_float(samples, DATA_BLOCK_SIZE);
            object_count++;
        }
    }

    if(!silent)
    {
        sprintf(str, "Possible Object Count: %d\n\r", object_count);

        uart_sendStr(str);

        lcd_printf("Possible Object Count: %d", object_count);

        int z;
        for(z = 0; z < object_count; ++z)
        {
            sprintf(str, "Object %d: Dist: %.3f\n\r", z, object_distances[z]);
            uart_sendStr(str);
        }
    }


    // Go to the start of the array
    scan_data = scan_data_start;

    // Now that the objects are counted, get the radial widths
    for(i = 0; i < ((start_angle - end_angle) / ANGLE_DELTA) && stored_object_count < object_count; ++i)
    {
        // Look for start, if object_detection_start_angle is less than 0, then there is not an angle stored
        if((fabs(scan_data->ir_distance - object_distances[stored_object_count]) <= AVERAGE_TOLERANCE) && object_detection_start_angle < 0)
        {
            object_detection_start_angle = scan_data->angle;
        }
        else if((fabs(scan_data->ir_distance - object_distances[stored_object_count]) >= AVERAGE_TOLERANCE
                || (i + 1) == (start_angle - end_angle)) // If this is the last value
                && object_detection_start_angle > -1)
        {
            if(radial_to_linear_width((object_detection_start_angle - scan_data->angle), object_distances[stored_object_count]) >= MIN_WIDTH &&
                    radial_to_linear_width((object_detection_start_angle - scan_data->angle), object_distances[stored_object_count]) <= MAX_WIDTH)
            {

                objects->median_angle = (object_detection_start_angle - ((object_detection_start_angle - scan_data->angle)/2));
                objects->start_angle = object_detection_start_angle;
                objects->radial_width = (object_detection_start_angle - scan_data->angle);
                objects->distance = object_distances[stored_object_count];
                objects->linear_width = radial_to_linear_width(objects->radial_width, objects->distance);
                objects->room_number = ++room_number;

                // Reset angle var
                object_detection_start_angle = -1;

                stored_object_count++;

                objects++;
            }
            else
            {
                object_detection_start_angle = -1;
            }


        }

        scan_data++;

    }


    objects = objects_start;

    result.object_count = stored_object_count;
    result.objects = objects;

    if(stored_object_count > 0)
    {
        if(!silent)
        {
            sprintf(str, "Actual Object Count: %d", stored_object_count);
            lcd_printf("%s", str);

            sprintf(str, "%s\n\r", str);
            uart_sendStr(str);
        }

        for(i = 0; i < stored_object_count; ++i)
        {
            if(!silent)
            {
                sprintf(str, "Object %d: Linear Width: %.3f ::: Median Angle: %d ::: Distance: %.3f ::: Start Angle: %d ::: End Angle %d\n\r", i + 1, objects->linear_width,
                        objects->median_angle,
                        objects->distance,
                        objects->start_angle,
                        objects->start_angle - objects->radial_width);
                uart_sendStr(str);
            }


            objects++;
        }

        objects = objects_start;
    }
    else if(!silent)
    {
        sprintf(str, "No objects detected :(\n\r");
        uart_sendStr(str);
    }

    return result;
}
