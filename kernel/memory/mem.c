#include <mem.h>

#define FRAME_SIZE 0x1000 // 4 KiB
#define BITMAP_SIZE 32768 // Example for managing 128 MiB of RAM

uint32_t memory_bitmap[BITMAP_SIZE / 32]; // Bitmap to track memory usage
size_t total_frames;
size_t last_kernel_reserved_frame;

void set_frame_occupied(size_t frame_no){
    memory_bitmap[frame_no / 32] |= (1 << (frame_no % 32)); // Set the frame's bitmap bit to 1
}

void set_frame_free(size_t frame_no){
    memory_bitmap[frame_no / 32] &= ~(1 << (frame_no % 32)); // Set the frame's bitmap bit to 0
}

bool test_frame(size_t frame_no){
    return memory_bitmap[frame_no / 32] & (1 << (frame_no % 32)); // Returns whether the frame is occupied or not 
}

size_t find_free_frame(){
    for (size_t i = 0; i < total_frames; i++){
        if (!test_frame(i)) return i;
    }
    return -1;
}

void* kalloc(){
    size_t free_frame = find_free_frame();
    if (free_frame == (size_t) -1){
        return NULL; // No available memory
    }
    set_frame_occupied(free_frame);
    return (void*) (free_frame * FRAME_SIZE);
}

void kfree(void* frame){
    size_t frame_index = (size_t) frame / FRAME_SIZE;

    if(frame_index > total_frames){
        return;
    }

    set_frame_free(frame_index);
}

void init_physical_memory(){
    total_frames = 128 * 1024 * 1024 / FRAME_SIZE; // Hard coded for now

    for(size_t i = 0; i < (BITMAP_SIZE / 32); i++){
        memory_bitmap[i] = 0;
    }

    // Reserve the first 2MB for the kernel
    last_kernel_reserved_frame = 0x200000 / FRAME_SIZE;
    for(size_t i = 0; i < last_kernel_reserved_frame; i++){
        set_frame_occupied(i);
    }

}

void memcpy(void* src, void* dst, size_t size){
    uint32_t *d = (uint32_t*) dst;
    const uint32_t *s = (const uint32_t*) src;

    // Copy 4 bytes (1 word) at a time if possible
    while (size >= 4) {
        *d++ = *s++;
        size -= 4;
    }

    // Copy remaining bytes if n is not divisible by 4
    uint8_t *d_byte = (uint8_t*) d;
    const uint8_t *s_byte = (const uint8_t*) s;
    while (size--) {
        *d_byte++ = *s_byte++;
    }
}