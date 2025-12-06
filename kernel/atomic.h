#pragma once

#include <stdint.h>
#include "loop.h"

template <typename T>
class AtomicPtr
{
    volatile T *ptr;

public:
    AtomicPtr() : ptr(nullptr) {}
    explicit AtomicPtr(T *x) : ptr(x) {}

    AtomicPtr<T> &operator=(T v)
    {
        __atomic_store_n(ptr, v, __ATOMIC_SEQ_CST);
        return *this;
    }

    operator T() const
    {
        return __atomic_load_n(ptr, __ATOMIC_SEQ_CST);
    }

    T fetch_add(T inc)
    {
        return __atomic_fetch_add(ptr, inc, __ATOMIC_SEQ_CST);
    }

    T add_fetch(T inc)
    {
        return __atomic_add_fetch(ptr, inc, __ATOMIC_SEQ_CST);
    }

    void set(T v)
    {
        __atomic_store_n(ptr, v, __ATOMIC_SEQ_CST);
    }

    T get()
    {
        return __atomic_load_n(ptr, __ATOMIC_SEQ_CST);
    }

    T exchange(T v)
    {
        T ret;
        __atomic_exchange(ptr, &v, &ret, __ATOMIC_SEQ_CST);
        return ret;
    }
};

template <typename T>
class Atomic
{
    volatile T value;

public:
    explicit Atomic(T x) : value(x) {}

    Atomic<T> &operator=(T v)
    {
        __atomic_store_n(&value, v, __ATOMIC_SEQ_CST);
        return *this;
    }

    operator T() const
    {
        return __atomic_load_n(&value, __ATOMIC_SEQ_CST);
    }

    T fetch_add(T inc)
    {
        return __atomic_fetch_add(&value, inc, __ATOMIC_SEQ_CST);
    }

    T add_fetch(T inc)
    {
        return __atomic_add_fetch(&value, inc, __ATOMIC_SEQ_CST);
    }

    void set(T v)
    {
        __atomic_store_n(&value, v, __ATOMIC_SEQ_CST);
    }

    T get()
    {
        return __atomic_load_n(&value, __ATOMIC_SEQ_CST);
    }

    T exchange(T v)
    {
        T ret;
        __atomic_exchange(&value, &v, &ret, __ATOMIC_SEQ_CST);
        return ret;
    }
};

// 64-bit specializations: keep your original constraint
template <>
class Atomic<uint64_t>
{
public:
    Atomic() = delete;
    explicit Atomic(uint64_t) = delete;
};

template <>
class Atomic<int64_t>
{
public:
    Atomic() = delete;
    explicit Atomic(int64_t) = delete;
};

template <typename T>
class LockGuard
{
    T &it;

public:
    inline explicit LockGuard(T &it) : it(it)
    {
        it.lock();
    }
    inline ~LockGuard()
    {
        it.unlock();
    }
};

template <typename T>
class LockGuardP
{
    T *it;

public:
    inline explicit LockGuardP(T *it) : it(it)
    {
        if (it)
        {
            it->lock();
        }
    }
    inline ~LockGuardP()
    {
        if (it)
        {
            it->unlock();
        }
    }
};

class NoLock
{
public:
    inline void lock() {}
    inline void unlock() {}
};

extern "C" void pause();

class Interrupts
{
public:
    static bool isDisabled()
    {
        // We don't track this yet on AArch64.
        return false;
    }

    static bool disable()
    {
        // Stub: don't actually mask IRQs yet.
        // Return "wasDisabled" = false.
        return false;
    }

    static void restore(bool /*wasDisabled*/)
    {
        // Nothing to do in stub.
    }

    template <typename Work>
    static inline void protect(Work work)
    {
        // Just run the work without fiddling with IRQs for now.
        work();
    }
};

class SpinLock
{
    Atomic<bool> taken{false};

public:
    SpinLock() = default;
    SpinLock(const SpinLock &) = delete;
    SpinLock &operator=(const SpinLock &) = delete;

    bool isMine()
    {
        return taken.get();
    }

    void lock()
    {
        while (true)
        {
            if (!taken.exchange(true))
            {
                return;
            }
            iAmStuckInALoop(true);
        }
    }

    void unlock()
    {
        taken.set(false);
        asm volatile("sev" ::: "memory");
    }

    friend class Semaphore; // if you have one later
};
