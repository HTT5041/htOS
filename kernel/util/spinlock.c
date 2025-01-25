#include <spinlock.h>

spinlock_t print_lock = SPINLOCK_UNLOCKED;
spinlock_t global_lock = SPINLOCK_UNLOCKED;

void spinlock_acquire(spinlock_t *lock){
    while(__sync_lock_test_and_set(lock, SPINLOCK_LOCKED));
    set_current_task_lock(true);
}

void spinlock_release(spinlock_t *lock){
    __sync_lock_release(lock);
    set_current_task_lock(false);
    yield(false);
}