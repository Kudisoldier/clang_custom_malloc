#define _DEFAULT_SOURCE
#include "tests.h"

//Heap init test
void* test1() {
    printf("Test 1: Heap init test \n");

    void* heap = heap_init(2); // Должно выделится минимальное значение для хипа 2*4096

    if (heap) printf("Test passed! \n");
    else printf("Test failed! \n");

    debug_heap(stdout, heap);
    printf("\n");

    return heap;
}

//Memory allocation test
void test2(void* heap) {
    printf("Test 2: Memory allocation test \n");

    void* allocated_memory = _malloc(2); // Должно выделиться 24 байта
    struct block_header* first_block = block_get_header(allocated_memory);

    if (allocated_memory && first_block->is_free == false && first_block->capacity.bytes == 24) printf("Test passed! \n");
    else printf("Test failed! \n");

    debug_heap(stdout, heap);
    _free(allocated_memory);
    debug_heap(stdout, heap);

    printf("\n");
}

//Free block test
void test3(void* heap) {
    printf("Test 3: Free block test \n");
    void* allocated_mem1 = _malloc(200);
    void* allocated_mem2 = _malloc(200);

    debug_heap(stdout, heap);
    _free(allocated_mem1);
    debug_heap(stdout, heap);

    struct block_header* first_block = block_get_header(allocated_mem1);
	struct block_header* second_block = block_get_header(allocated_mem2);

    if (allocated_mem1 && allocated_mem2 && first_block->is_free == true && second_block->is_free == false) printf("Test passed! \n");
    else printf("Test failed! \n");

    _free(allocated_mem2);
    printf("\n");

}

//Free double blocks
void test4(void* heap) {
    printf("Test 4: Free double blocks test \n");
    void* allocated_mem1 = _malloc(200);
    void* allocated_mem2 = _malloc(200);
    void* allocated_mem3 = _malloc(200);

    debug_heap(stdout, heap);
    _free(allocated_mem1);
    _free(allocated_mem2);
    debug_heap(stdout, heap);

    struct block_header* first_block = block_get_header(allocated_mem1);
	struct block_header* second_block = block_get_header(allocated_mem2);
    struct block_header* third_block = block_get_header(allocated_mem3);

    if (allocated_mem1 && allocated_mem2 && allocated_mem3 && first_block->is_free == true &&
    second_block->is_free == true && third_block->is_free == false) printf("Test passed! \n");
    else printf("Test failed! \n");

    _free(allocated_mem3);
    printf("\n");
}

//Extend heap test
void test5(void* heap) {
    printf("Test 5: Extend heap test \n");

    debug_heap(stdout, heap);

    void* allocated_mem1 = _malloc(6000);
    void* allocated_mem2 = _malloc(6000);

    debug_heap(stdout, heap);

    struct block_header* first_block = block_get_header(allocated_mem1);
	struct block_header* second_block = block_get_header(allocated_mem2);

    if (allocated_mem1 && allocated_mem2 && first_block->is_free == false && second_block->is_free == false) printf("Test passed! \n");
    else printf("Test failed! \n");

    _free(allocated_mem2);
    _free(allocated_mem1);

    printf("\n");
}

//Extend heap new region test
void test6(void* heap) {
    printf("Test 6: Extend heap new region test \n");
    debug_heap(stdout, heap);

    struct block_header* first_block = (struct block_header*) heap;

    void* addr = (uint8_t*) first_block + size_from_capacity(first_block->capacity).bytes;

    void* res = mmap(addr, 20000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, 0, 0); //выделим память сразу за последним блоком

    void* allocated_mem1 = _malloc(60000);
    struct block_header* second_block = block_get_header(allocated_mem1);
    debug_heap(stdout, heap);

    // проверка на то что новый выделенный блок выделился в новом регионе
    if( res != MAP_FAILED && allocated_mem1 && second_block->is_free == false &&
    ((long int) allocated_mem1 -  (long int) addr) > 1000000) printf("Test passed! \n");
    else printf("Test failed! \n");

    _free(allocated_mem1);
}
