#pragma once
#include <cstdint>
#include <cstddef>
#include "Allocator.hpp"
#include "Assert.hpp"

namespace Sol {

template <typename T>
struct Array {
  T* data = nullptr;
  size_t cap = 0;
  size_t len = 0;

static Array<T> get(size_t size) {
    Array a;
    a.init(size);
    return a;
}
void init(size_t size) {
    cap = size;
	len = 0;
    data = (T*)(lin_alloc(size * sizeof(T)));
}
void empty() {
    data = nullptr;
    cap = 0;
    len = 0;
    SCRATCH->cut(cap * sizeof(T));
}
T* to_append() {
    DEBUG_ASSERT(len <= cap, "ARRAY OVERFLOW");
    ++len;
    return data + len - 1;
}
void append(T *t) {
    DEBUG_ASSERT(len < cap, "ARRAY OVERFLOW");
    data[len] = *t;
    ++len;
}
void append_cpy(T t) {
    DEBUG_ASSERT(len < cap, "ARRAY OVERFLOW");
    data[len] = t;
    ++len;
}
void copy_here(T* from, size_t count, size_t offset) {
    DEBUG_ASSERT(cap - len >= count + offset, "Insufficient size in Array for copy_here()");
    mem_cpy(data + offset, from, count * sizeof(T));
    len += count;
}
T& operator[](size_t i) {
    DEBUG_ASSERT(i < cap, "OUT OF BOUND ACCESS ON ARRAY");
  return data[i];
}
};

template <typename T>
struct DynArray {
	size_t len = 0;
	size_t cap = 0;
	T* data = nullptr;

    static DynArray get(size_t cap_) {
        DynArray ret;
        ret.init(cap_);
        return ret;
    }
	void init(size_t cap_) {
		if (cap_ == 0)
            return;
		cap = cap_;
		data = (T*)(heap_alloc(cap * sizeof(T)));
	}

	void kill() {
		heap_free(data);
		len = 0;
		cap = 0;
	}
	void print() {
		for(size_t i = 0; i < len; ++i) {
		  std::cout << data[i] << '\n';
		}
	}
    void reserve(size_t count) {
        len += count;
        ASSERT(len <= cap, "List::alloc len + count > capacity");
    }
    void zero() {
        memset(data + len, 0, cap - len);
    }
	void grow_zero(size_t count) {
		grow(count);
		zero();
	}
	void pop() {
		if (!len) return;

		--len;
	}
    void push(T *t) {
        if (cap == len) grow();

        data[len] = *t;
        ++len;
    }
	void push_cpy(T t) {
		if (cap == len) grow();

		data[len] = t;
		++len;
	}
    T* to_push() {
        if (len == cap)
            grow();
        ++len;
        return data + len - 1;
    }
	void resize(size_t size) {
        T* old_data = data;

        cap = size;
        data = (T*)heap_alloc(cap * sizeof(T));
        len = len > cap ? cap : len;

        mem_cpy(data, old_data, len * sizeof(T));
        heap_free(old_data);
	}
	void grow(size_t size) {
        if (size <= cap)
            return;

        cap += size;
        T* old_data = data;
        data = (T*)heap_alloc(cap * sizeof(T));

        mem_cpy(data, old_data, len * sizeof(T));
        heap_free(old_data);
	}
	void grow() {
        cap *= 2;
        T* old_data = data;
        data = (T*)heap_alloc(cap * sizeof(T));

        mem_cpy(data, old_data, len * sizeof(T));
        heap_free(old_data);
	}
	T* last() {
		return &data[len - 1];
	}
	T& operator[](size_t i) {
        DEBUG_ASSERT(i < len, "OUT OF BOUNDS ACCESS ON DynArray");
		return data[i];
	}
};

} // namespace Sol
