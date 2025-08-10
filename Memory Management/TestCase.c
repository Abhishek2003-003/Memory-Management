void run_tests() 
{
    printf("==== Running Predefined Test Cases ====\n");
    allocate(0, 128);   // mem[0]
    allocate(1, 1024);  // mem[1]
    allocate(2, 4096);  // mem[2]
    print_state();

    deallocate(1);      // should pack mem[2] down
    print_state();

    allocate(1, 512);   // allocate at mem[1] again
    print_state();
    printf("**** Test Cases Completed *****\n\n");
}