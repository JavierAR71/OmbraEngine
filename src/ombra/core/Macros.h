#pragma once


#define OMBRA_DISALLOW_COPY(Type)      \
Type(const Type&) = delete;        \
Type& operator=(const Type&) = delete;


#define OMBRA_DISALLOW_COPY_AND_MOVE(Type) \
Type(const Type&) = delete;            \
Type& operator=(const Type&) = delete; \
Type(Type&&) = delete;                 \
Type& operator=(Type&&) = delete;


#define OMBRA_UNUSED(x) (void)(x)

#define OMBRA_FORCE_INLINE inline __attribute__((always_inline))

#define OMBRA_FINAL final

#define OMBRA_DEPRECATED(msg) [[deprecated(msg)]]

#ifdef NDEBUG
    #define OMBRA_ASSERT(condition) ((void)0)
#else
    #include <cassert>
    #define OMBRA_ASSERT(condition) assert(condition)
#endif