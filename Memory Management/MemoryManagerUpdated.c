#include <stddef.h>  

#define memory_size 102400
#define max_pointers 100

unsigned char memory[memory_size]; //creating block of 100k
void* mem[max_pointers];           // User pointers
int size_arr[max_pointers];        // Sizes of each allocation
int used = 0;                      // Used bytes count

// Function to show remaining free memory
void print_remaining() 
{
    printf("Remaining memory: %d bytes\n", memory_size - used);
}

//****

//  handles overlapping memory
void memorymove(void *destination, const void *source, int count)
{
 //shifts all the datas after the sealocated space to the deallocated space for efficient packing
    unsigned char *destination_ptr = (unsigned char*)destination;
    const unsigned char *source_ptr = (const unsigned char*)source;

    if (destination_ptr < source_ptr) { // normal copy forward
        for (int i = 0; i < count; i++) {
            destination_ptr[i] = source_ptr[i];
        }
    } else { // overlapping copy backwards
        for (int i = count - 1; i >= 0; i--) {
            destination_ptr[i] = source_ptr[i];
        }
    }
    //this allows remaining large block of memory to be continuos
}

void* allocate(int n, int size)
{
 //checks if entered conditions are suitable for allocating memory
    if (n < 0 || n >= max_pointers) {
        printf("Index out of bounds. Use n between 0 and 99.\n");
        print_remaining();
        return NULL;
    }
    if (size <= 0 || size > memory_size - used) {
        printf("Insufficient memory.\n");
        print_remaining();
        return NULL;
    }
    if (mem[n] != NULL) {
        printf("mem[%d] is already allocated! Deallocate first.\n", n);
        print_remaining();
        return NULL;
    }
//when conditions are met here it starts allocating
    void* ptr = (void*)(memory + used); 
    mem[n] = ptr;
    size_arr[n] = size;
    used += size;

    printf("Allocated %d bytes at mem[%d]\n", size, n);
    print_remaining();
    return ptr;
}

void deallocate(int n)
{
 //checks if the variable is allocated already and can be deallocated 
    if (n < 0 || n >= max_pointers || mem[n] == NULL) {
        printf("Invalid or unallocated index: %d\n", n);
        print_remaining();
        return;
    }

    int sz = size_arr[n];
    unsigned char *block_start = (unsigned char*)mem[n];
    int bytes_after = used - ((block_start + sz) - memory);

    //  call the function to Shift all data of memory after this block down (for efficient packing)
    if (bytes_after > 0) {
        memorymove(block_start, block_start + sz, bytes_after);
    }

    // Adjust pointers after n
    for (int i = n + 1; i < max_pointers; ++i) {
        if (mem[i]) {
            mem[i] = (unsigned char*)mem[i] - sz;
        }
    }

    mem[n] = NULL;
    size_arr[n] = 0;
    used -= sz;
    printf("Deallocated mem[%d] of size %d. Memory packed.\n", n, sz);
    print_remaining();
}

void print_state()
{
    printf("Memory used: %d bytes\n", used);
    print_remaining();
    for (int i = 0; i < max_pointers; ++i) {
        if (mem[i]) {
            printf(" mem[%d]: %p, size %d\n", i, mem[i], size_arr[i]);
        }
    }
    printf("\n");
}

int main() 
{
    //  clear everything 
    for (int i = 0; i < max_pointers; i++) {
        mem[i] = NULL;
        size_arr[i] = 0;
    }
    for (int i = 0; i < memory_size; i++) {
        memory[i] = 0;
    }

    while (1) {
        int choice;
        printf("\n1. Allocate\n2. Deallocate\n3. Print state\n4. Exit\nChoose: ");
        scanf("%d", &choice);
        if (choice == 4) break;
        if (choice == 1) {
            int n, size;
            printf("Enter mem[n] index to allocate (n should be lesser than 100): ");
            scanf("%d", &n);
            printf("Enter size to allocate (in bytes): ");
            scanf("%d", &size);
            allocate(n, size);
        } else if (choice == 2) {
            int n;
            printf("Enter mem[n] index to deallocate: ");
            scanf("%d", &n);
            deallocate(n);
        } else if (choice == 3) {
            print_state();
        } else {
            printf("invalid choice\n");
        }
    }

    return 0;
}
