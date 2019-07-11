#ifndef QTRESTWRAPPERCAPI_GLOBAL_H
#define QTRESTWRAPPERCAPI_GLOBAL_H

#if defined(QTRESTWRAPPERCAPI_LIBRARY)
#  define QTRESTWRAPPERCAPISHARED_EXPORT extern "C" __declspec(dllexport)
#else
#  define QTRESTWRAPPERCAPISHARED_EXPORT extern "C" __declspec(dllimport)
#endif

#if defined(__BORLANDC__)
    typedef unsigned char uint8_t;
    typedef __int64 int64_t;
    typedef unsigned long uintptr_t;
#elif defined(_MSC_VER)
    typedef unsigned char uint8_t;
    typedef __int64 int64_t;
#else
    #include <stdint.h>
#endif

#endif // QTRESTWRAPPERCAPI_GLOBAL_H
