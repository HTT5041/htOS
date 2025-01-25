#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <scheduling.h>

typedef volatile int spinlock_t;

#define SPINLOCK_LOCKED 1
#define SPINLOCK_UNLOCKED 0

void spinlock_acquire(spinlock_t *lock);
void spinlock_release(spinlock_t *lock);

// ------ Global Spinlocks ------ //
extern spinlock_t print_lock;
extern spinlock_t global_lock;

#endif