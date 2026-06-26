#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Configuration 
#define BUFFER_MAX_SIZE     1000   // Max bytes buffer can hold            
#define PRINT_THRESHOLD     50     // Min bytes needed before printing     
#define WAKEUP_INTERVAL     10     // Wake-up check every 10 seconds       
#define SIMULATION_SECONDS  60     // Total simulated seconds to run       

//Global Data Structure 
typedef struct {
    unsigned char data[BUFFER_MAX_SIZE];
    int count;  // Number of bytes currently stored 
} 
ByteBuffer;

ByteBuffer g_buffer = { .count = 0 };  // Global buffer, starts empty 

// Function Declarations 
void timer_task(int current_second);
void wakeup_task(int second);
void print_hex(unsigned char *arr, int length, int second); // *arr - Pointer 
void add_bytes_to_buffer(unsigned char *bytes, int num_bytes); // *bytes - Pointer 
void delete_printed_bytes(int num_bytes);

// Main Function
int main() {
    srand((unsigned int)time(0));  // Random number generator 

    printf("=================================================\n");
    printf("Question 2 Timer Function \n");
    printf("=================================================\n");
    printf("Timer  : Adds 0-5 random bytes every second\n");
    printf("Wakeup : Every 10s, print & delete last 50 bytes (if >= 50)\n");
    printf("=================================================\n");

    //Simulate time from second 1 to SIMULATION_SECONDS 
    for (int second = 1; second <= SIMULATION_SECONDS; second++) {

        //Timer Task: runs every second
        timer_task(second);

        //Wake-up Task: runs every 10 seconds 
        if (second % WAKEUP_INTERVAL == 0) {
            wakeup_task(second);
        }
    }

    printf("=================================================\n");
    printf("Simulation complete.\n");
    printf("Bytes remaining in buffer: %d\n", g_buffer.count);
    printf("=================================================\n");

    return 0;
}

/* TIMER TASK - Simulates external sensor data arriving every second.
 * Generates 0–5 random bytes and adds them to global buffer.*/
void timer_task(int current_second) {
    // Generate random count of bytes: 0 to 5 
    int num_bytes = rand() % 6;  // Number generated - 0, 1, 2, 3, 4, or 5 

    if (num_bytes == 0) {
        printf("[Second %2d] Timer: No bytes generated (0 bytes)\n", current_second);
        return;
    }

    // Generate random byte values 
    unsigned char new_bytes[5];
    for (int i = 0; i < num_bytes; i++) {
        new_bytes[i] = (unsigned char)(rand() % 256);  // Random byte 0x00–0xFF 
    }

    // Add to global buffer 
    add_bytes_to_buffer(new_bytes, num_bytes);

    // Log what was added 
    printf("[Second %2d] Timer: Added %d byte(s) → ", current_second, num_bytes);
    for (int i = 0; i < num_bytes; i++) {
        printf("0x%02X ", new_bytes[i]);
    }
    printf("| Buffer size: %d\n", g_buffer.count);
}

/* WAKE-UP TASK - Runs every 10 seconds.
 * If buffer has >= 50 bytes: prints last 50 bytes in hex,
 * then deletes those 50 bytes from the buffer.*/
void wakeup_task(int current_second) {
    printf("\n[Second %2d] *** WAKE-UP TASK TRIGGERED ***\n", current_second);
    printf("Buffer currently holds: %d byte(s)\n", g_buffer.count);

    if (g_buffer.count >= PRINT_THRESHOLD) {
        printf(">= 50 bytes found. Printing last 50 bytes:\n\n");

        // Find start index of last 50 bytes 
        int start_index = g_buffer.count - PRINT_THRESHOLD;

        // Print the last 50 bytes in hex 
        print_hex(&g_buffer.data[start_index], PRINT_THRESHOLD, current_second);

        // Delete the printed 50 bytes from buffer 
        delete_printed_bytes(PRINT_THRESHOLD);

        printf("\nDeleted 50 bytes. Buffer size now: %d\n", g_buffer.count);
    } 
    else {
        printf("The Total number of bytes is < 50 bytes. \nNothing printed.\n\n");
    }
}

// PRINT HEX - Prints given byte array in hex format, 10 bytes per line.
void print_hex(unsigned char *arr, int len, int second) {
    printf("[ Hex Dump at second %d ]\n", second);
    printf(" ");
    for (int i = 0; i < len; i++) {
        printf("0x%02X ", arr[i]);

        // New line every 10 bytes
        if ((i + 1) % 10 == 0) {
            printf("\n ");
        }
    }
}

/* ADD BYTES TO BUFFER - Appends new bytes to the global buffer.
 * Drops bytes if buffer is full (overflow protection).*/
void add_bytes_to_buffer(unsigned char *bytes, int num_bytes) {
    for (int i = 0; i < num_bytes; i++) {
        if (g_buffer.count >= BUFFER_MAX_SIZE) {
            // Buffer full — protect against overflow 
            printf("WARNING -  Buffer full ! Deleting byte.\n");
            break;
        }
        g_buffer.data[g_buffer.count] = bytes[i];
        g_buffer.count++;
    }
}

/* DELETE PRINTED BYTES - Removes the last N bytes from the global buffer
 * by shifting remaining data and reducing count. */ 
void delete_printed_bytes(int num_bytes) {
    if (num_bytes > g_buffer.count) {
        num_bytes = g_buffer.count;  // Safety check 
    }
    int new_count = g_buffer.count - num_bytes;
    g_buffer.count = new_count;

    // Zero out deleted portion for cleanliness 
    memset(&g_buffer.data[new_count], 0, num_bytes);
}
