#pragma once

#include <emmintrin.h>
#include <math.h>
#include <iostream>

#include "Basic.hpp"
#include "external/wyhash.h"


template <typename T>
inline uint64_t calculate_hash(const T &value, size_t seed = 0) {
    return wyhash(&value, sizeof(T), seed, _wyp);
}

template <size_t N>
inline uint64_t calculate_hash(const char (&value)[N], size_t seed = 0) {
    return wyhash(value, strlen(value), seed, _wyp);
}

// template <> TODO: idk what this does, but I get a compiler error uncommented
inline uint64_t calculate_hash(const char *value, size_t seed) {
    return wyhash(value, strlen(value), seed, _wyp);
}

inline uint64_t hash_bytes(void *data, size_t len, size_t seed = 0) {
    return wyhash(data, len, seed, _wyp);
}

inline void checked_mul(size_t &res, size_t mul) {
    DEBUG_ASSERT(UINT64_MAX / res < mul, "usize checked mul overflow");
    res = res * mul;
}

inline void next_pow_2(size_t &cap) {
    if (cap <= 16)
        cap = 16;

    size_t log = log2(cap);
    if (pow(2, log) == cap) {
    } else
        cap = pow(2, log + 1);
}

struct BitMask {
    uint16_t mask;

    // methods for checking bits...
    inline uint32_t count_trailing_zeros() { return __builtin_ctzl(mask); }
};

inline void make_group_empty(uint8_t *bytes) {
    __m128i ctrl = _mm_set1_epi8(EMPTY);
    _mm_store_si128(reinterpret_cast<__m128i *>(bytes), ctrl);
}
struct Group {
    __m128i ctrl;

    static inline Group get_from_index(u64 index, u8 *data) {
        Group ret;
        ret.ctrl = *reinterpret_cast<__m128i*>(data + index);
        return ret;
    }
    static inline Group get_empty() {
        Group ret;
        ret.ctrl = _mm_set1_epi8(EMPTY);
        return ret;
    }
    inline BitMask is_empty() {
        __m128i empty = _mm_set1_epi8(EMPTY);
        __m128i res = _mm_cmpeq_epi8(ctrl, empty);
        uint16_t mask = _mm_movemask_epi8(res);
        return BitMask{mask};
    }
    inline BitMask is_special() {
        uint16_t mask = _mm_movemask_epi8(ctrl);
        return BitMask{mask};
    }
    inline BitMask is_full() {
        BitMask mask = is_special();
        uint16_t invert = ~mask.mask;
        return BitMask{invert};
    }
    inline void fill(uint8_t *bytes) {
        _mm_store_si128(reinterpret_cast<__m128i *>(bytes), ctrl);
    }
    inline BitMask match_byte(uint8_t byte) {
        __m128i to_match = _mm_set1_epi8(byte);
        __m128i match = _mm_cmpeq_epi8(ctrl, to_match);
        BitMask mask;
        mask.mask = (uint16_t)_mm_movemask_epi8(match);
        return mask;
    }
};
