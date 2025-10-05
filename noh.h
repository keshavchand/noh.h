#include <stddef.h>
#include <dlfcn.h>
#include <stdio.h>

#define _IS_USER_DEFINED(MACRO_NAME) MACRO_NAME

#ifndef FN_IMPL_FLAG
#define FN_IMPL_FLAG IMPLEMENTATION
#endif

#ifndef FN_DYNAMIC_LOADER_FLAG
#define FN_DYNAMIC_LOADER_FLAG DYNAMIC_LOADER
#endif

#ifndef FN_NAMESPACE
#define FN_NAMESPACE main
#endif

#define __FN_DYNAMIC_LOADER_SECTION_NAME(ns, type) \
    #ns "_" #type

#define _FN_DYNAMIC_LOADER_SECTION_NAME(ns, type) \
    __FN_DYNAMIC_LOADER_SECTION_NAME(ns, type)

#define _FN_DYNAMIC_LOADER_ATTRIBUTE(ns, type) \
    __attribute__((used, section(_FN_DYNAMIC_LOADER_SECTION_NAME(ns, type))))

#define _FN_DYNAMIC_LOADER_ATTRIBUTE_FUNCTION(ns)\
    _FN_DYNAMIC_LOADER_ATTRIBUTE(ns, dynamic_fn)

#define _FN_DYNAMIC_LOADER_ATTRIBUTE_FUNCTION_NAME(ns)\
    _FN_DYNAMIC_LOADER_ATTRIBUTE(ns, dynamic_fn_name)

#if defined(FN_IMPL_FLAG) && _IS_USER_DEFINED(FN_IMPL_FLAG)
    #define _FN_KEEP(...) { \
        __VA_ARGS__ \
    }
    #define FN(ret, name, ...) \
        ret name(__VA_ARGS__) _FN_KEEP
#else
#if !defined(FN_DYNAMIC_LOADER_FLAG) || !_IS_USER_DEFINED(FN_DYNAMIC_LOADER_FLAG)
    #define _FN_DISCARD(...) /* removed body */
    #define FN(ret, name, ...) \
        ret name(__VA_ARGS__); _FN_DISCARD
#else
    #define _FN_DISCARD(...) /* removed body */
    #define FN(ret, name, ...) \
        _FN_DYNAMIC_LOADER_ATTRIBUTE_FUNCTION_NAME(FN_NAMESPACE) \
        static const char* name##_name  = #name; \
        _FN_DYNAMIC_LOADER_ATTRIBUTE_FUNCTION(FN_NAMESPACE) \
        static ret (*name)(__VA_ARGS__)  = 0; \
        _FN_DISCARD
#endif
#endif


#define __FN_START_STOP_SECTION(ns, section_name) \
    extern void *__start_##ns##_##section_name [];\
    extern void *__stop_##ns##_##section_name []

#define _FN_START_STOP_SECTION(ns, section_name) \
    __FN_START_STOP_SECTION(ns, section_name)

#define __FN_START_SECTION_NAME(ns, section_name) \
    __start_##ns##_##section_name

#define __FN_STOP_SECTION_NAME(ns, section_name) \
    __stop_##ns##_##section_name

#define _FN_START_SECTION_NAME(ns, section_name) \
    __FN_START_SECTION_NAME(ns, section_name)

#define _FN_STOP_SECTION_NAME(ns, section_name) \
    __FN_STOP_SECTION_NAME(ns, section_name)


#if defined(FN_DYNAMIC_LOADER_FLAG) && _IS_USER_DEFINED(FN_DYNAMIC_LOADER_FLAG)
  _FN_START_STOP_SECTION(FN_NAMESPACE, dynamic_fn);
  _FN_START_STOP_SECTION(FN_NAMESPACE, dynamic_fn_name);

#define fnLoadSymbols(ns) \
    _fnLoadSymbols(ns)

#define _fnLoadSymbols(ns) \
    fnLoadSymbols##_##ns
void fnLoadSymbols(FN_NAMESPACE)(const char* tgt) {
    void *tgtHandle = dlopen(tgt, RTLD_NOW);
    if (!tgtHandle) {
        printf("%s", dlerror());
        return;
    }

    typedef void (*voidFn) (void);

    voidFn *fn;
    void **fn_name;
    for (fn = (voidFn*) _FN_START_SECTION_NAME(FN_NAMESPACE, dynamic_fn), fn_name = _FN_START_SECTION_NAME(FN_NAMESPACE, dynamic_fn_name);
          fn < (voidFn*) _FN_STOP_SECTION_NAME(FN_NAMESPACE, dynamic_fn) && fn_name < _FN_STOP_SECTION_NAME(FN_NAMESPACE, dynamic_fn_name);
          fn++, fn_name++) {
         printf("Before: %p %s\n", (void*) *fn, (const char*) *fn_name);
         *fn = (voidFn) dlsym(tgtHandle, (const char*) *fn_name);
         printf("After: %p %s\n", (void*) *fn, (const char*) *fn_name);
    }
}
#else
#define fnLoadSymbols(ns) \
    _fnLoadSymbols(ns)

#define _fnLoadSymbols(ns) \
    fnLoadSymbols##_##ns
void fnLoadSymbols(FN_NAMESPACE)(const char* tgt) {}
#endif
