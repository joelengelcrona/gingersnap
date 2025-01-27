/**
 * Guest memory layout:
 *
 * +============================+========================+================+
 * |  Loadable program headers  | <-- Guest stack (1MiB) | Guest heap --> |
 * +============================+========================+================+
 * ^                                                     ^                ^
 * |                                                     |                |
 * Address 0                                             Initial stack pointer (grows downwards)
 *                                                       |                |
 *                                                       Initial curr_alloc_adr (grows upwards)
 *                                                                        |
 *                                                                        mmu->memory_size
 *
 * Note that instead of having the stack and the heap growing towards eachother,
 * like they do in traditional OS'es, this emulator and mmu implements them
 * differently. This is to avoid supporting emulation of big allocations, which
 * would use the mmap syscall instead of brk/sbrk and to safely allow for
 * allocatons of big chunks of memory on the heap without overwriting the stack.
 * It will however lead to diffing values returned by the brk/sbrk syscall, but
 * this should not impact the execution flow in any meaningful way.
 */


#ifndef MMU_H
#define MMU_H

#include "../utils/vector.h"

#include "adr_map.h"

// Amount of bytes in single block
// TODO: Tune this value for performance
#define DIRTY_BLOCK_SIZE  64

typedef struct dirty_state dirty_state_t;
typedef struct mmu         mmu_t;

static const uint8_t MMU_PERM_EXEC  = 1 << 0;
static const uint8_t MMU_PERM_WRITE = 1 << 1;
static const uint8_t MMU_PERM_READ  = 1 << 2;
static const uint8_t MMU_PERM_RAW   = 1 << 3; // Read after write.

static const uint8_t MMU_ALLOC_NO_ERROR            = 0; // No error.
static const uint8_t MMU_ALLOC_ERROR_MEM_FULL      = 1; // Emulator memory is already full.
static const uint8_t MMU_ALLOC_ERROR_WOULD_OVERRUN = 2; // Allocation would overrun the memory size.

static const uint8_t MMU_READ_NO_ERROR               = 0; // No error.
static const uint8_t MMU_READ_ERROR_NO_PERM          = 1; // Attempted to read from an address with no read permission.
static const uint8_t MMU_READ_ERROR_ADR_OUT_OF_RANGE = 2; // Attempted to read from an address which is outside emulator memory.

static const uint8_t MMU_WRITE_NO_ERROR               = 0; // No error.
static const uint8_t MMU_WRITE_ERROR_NO_PERM          = 1; // Attempted to write from an address with no read permission.
static const uint8_t MMU_WRITE_ERROR_ADR_OUT_OF_RANGE = 2; // Attempted to write from an address which is outside emulator memory.

struct dirty_state {
    void (*make_dirty)(dirty_state_t* state, size_t address);
    void (*print)(dirty_state_t* state);
    void (*clear)(dirty_state_t* state);

    // Keeps track of blocks of memory that have been dirtied
    size_t*  dirty_blocks;
    uint64_t nb_dirty_blocks;

    // Bytes are grouped together into blocks to avoid having to do large number
    // of memsets to reset guest memory. If a byte is written to it is
    // considered dirty and the entire block which contains the dirty byte is
    // considered dirty.
    //
    // One entry in the dirty_bitmap tracks the state of 64 blocks. One block
    // per bit. 0 = clean, 1 = dirty.
    uint64_t* dirty_bitmap;
    uint64_t  nb_max_dirty_bitmaps;
};

struct mmu {
    size_t    (*allocate)(mmu_t* mmu, size_t size, uint8_t* error);
    void      (*set_permissions)(mmu_t* mmu, size_t start_adress, uint8_t permission, size_t size);
    uint8_t   (*write)(mmu_t* mmu, size_t destination_adress, const uint8_t* source_buffer, size_t size);
    uint8_t   (*read)(mmu_t* mmu, uint8_t* destination_buffer, const size_t source_adress, size_t size);
    vector_t* (*search)(mmu_t* mmu, const uint64_t needle, const char size_letter);
    void      (*print)(mmu_t* mmu, size_t start_adr, const size_t range, const char size_letter);
    uint64_t  (*virt_to_mapped)(mmu_t* mmu, uint64_t virt_adr);

    // The size of the emulator memory
    size_t memory_size;

    // TODO: Check perfomance loss when using vector instead of uint8_t* as
    // memory data type. Same goes for perms.
    //
    // The memory of the emulator
    uint8_t* memory;

    // Memory permissions, each byte corresponds to the byte with the same offset in the guest memory block
    uint8_t* permissions;

    // Counter tracking number of allocated bytes in guest memory. Acts as the virtual base address of next allocation
    // for the guest. Virtual address.
    //
    // memory[current_allocation - 1] == last allocated address in guest memory
    size_t curr_alloc_adr;

    // Where in the MMU buffer the stack starts. Never changes once set.
    size_t initial_stack_adr_mapped;

    // Virtual address where the stack starts. Never changes once set.
    size_t initial_stack_adr_virt;

    // Tracker of memory blocks which have been touched by program execution.
    dirty_state_t* dirty_state;

    // Address translation mappings of virtual program header addresses to actual emulator memory.
    adr_map_t** adr_maps;

    // Number of address transation mappings in use. Should be one per loaded program header.
    uint64_t nb_adr_maps;
};

mmu_t*
mmu_create(const size_t memory_size, const size_t base_alloc_adr);

void
mmu_destroy(mmu_t* mmu);

void
print_permissions(uint8_t perms);

#endif // MMU_H
