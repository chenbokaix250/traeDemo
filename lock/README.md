# C++中锁的概念、分类及应用场景

## 锁的基本概念

锁是多线程编程中用于保护共享资源的同步机制，防止多个线程同时访问和修改共享数据而导致的数据竞争和不一致性问题。在C++中，锁是实现线程同步的基本工具。

## 锁的分类

### 1. 互斥锁 (Mutex)

最基本的锁类型，提供独占访问。

```cpp
#include <mutex>

std::mutex mtx;

void function() {
    mtx.lock();    // 获取锁
    // 临界区代码
    mtx.unlock();  // 释放锁
}
```

**应用场景**：保护简单的共享资源，如计数器、缓存等。

### 2. 递归锁 (Recursive Mutex)

允许同一线程多次获取锁而不会死锁。

```cpp
#include <mutex>

std::recursive_mutex rmtx;

void recursive_function(int depth) {
    rmtx.lock();
    // 临界区代码
    if (depth > 0) {
        recursive_function(depth - 1);  // 递归调用，再次获取锁
    }
    rmtx.unlock();
}
```

**应用场景**：递归函数中需要保护共享资源。

### 3. 读写锁 (Shared Mutex)

允许多个线程同时读取，但写入时需要独占。

```cpp
#include <shared_mutex>

std::shared_mutex rwmtx;

void reader() {
    std::shared_lock<std::shared_mutex> lock(rwmtx);  // 共享锁
    // 读取操作
}

void writer() {
    std::unique_lock<std::shared_mutex> lock(rwmtx);  // 独占锁
    // 写入操作
}
```

**应用场景**：读多写少的场景，如配置信息、缓存等。

### 4. 自旋锁 (Spinlock)

不会让线程进入睡眠状态，而是在一个循环中不断尝试获取锁。

```cpp
#include <atomic>

class spinlock {
    std::atomic<bool> flag = {false};
public:
    void lock() {
        while(flag.exchange(true, std::memory_order_acquire));
    }
    void unlock() {
        flag.store(false, std::memory_order_release);
    }
};
```

**应用场景**：锁竞争不激烈且临界区执行时间非常短的场景。

### 5. 条件变量 (Condition Variable)

用于线程间的通知机制，通常与互斥锁配合使用。

```cpp
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });  // 等待条件满足
    // 条件满足后执行的代码
}

void signal() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();  // 通知一个等待的线程
}
```

**应用场景**：生产者-消费者模式，线程池等需要线程间协作的场景。

## 锁的RAII封装

C++提供了几种RAII风格的锁封装，自动管理锁的获取和释放：

### 1. std::lock_guard

最简单的RAII锁封装，构造时获取锁，析构时释放锁。

```cpp
{
    std::lock_guard<std::mutex> lock(mtx);
    // 临界区代码
} // 离开作用域时自动释放锁
```

### 2. std::unique_lock

比lock_guard更灵活，支持延迟锁定、定时锁定、手动解锁等操作。

```cpp
{
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock); // 不立即锁定
    // 其他操作
    lock.lock(); // 手动锁定
    // 临界区代码
    lock.unlock(); // 手动解锁
    // 其他操作
} // 离开作用域时自动释放锁（如果仍被锁定）
```

### 3. std::shared_lock

用于读写锁的共享模式（读模式）。

```cpp
{
    std::shared_lock<std::shared_mutex> lock(rwmtx);
    // 读取操作
}
```

## 锁的应用场景

1. **数据保护**：防止多线程同时修改共享数据
2. **线程同步**：确保线程按特定顺序执行
3. **资源管理**：限制对有限资源的访问
4. **单例模式**：确保只创建一个实例
5. **读写分离**：允许多读单写的并发访问模式

## 锁的注意事项

1. **死锁**：多个线程互相等待对方持有的锁
2. **活锁**：线程不断地改变状态，但无法继续执行
3. **优先级反转**：低优先级线程持有高优先级线程需要的锁
4. **锁的粒度**：锁保护的范围大小，影响并发性能
5. **锁的开销**：频繁的锁操作会带来性能损失

在您提供的`PreciseTimer`类中，使用了互斥锁和条件变量来保护线程安全的定时器操作，这是一个典型的线程同步应用场景。