#pragma once

#include <iostream>
#include <execinfo.h>
#include <cstdlib>
#include <cxxabi.h>
#include <dlfcn.h>
#include <sstream>

namespace debug {

inline void print_stack_trace() {
  constexpr int kBacktraceDepth = 15;
  void* backtrace_addrs[kBacktraceDepth];

  int trace_size = backtrace(backtrace_addrs, kBacktraceDepth);

  for (int i = 0; i < trace_size; ++i) {
    Dl_info info;
    dladdr(backtrace_addrs[i], &info);

    std::stringstream cmd(std::ios_base::out);
    cmd << "atos -o " << info.dli_fname << " -l " << std::hex
      << reinterpret_cast<uint64_t>(info.dli_fbase) << ' '
      << reinterpret_cast<uint64_t>(backtrace_addrs[i]);

    FILE* atos = popen(cmd.str().c_str(), "r");

    constexpr int kBufferSize = 200;
    char buffer[kBufferSize];

    fgets(buffer, kBufferSize, atos);
    pclose(atos);

    std::cout << buffer;
  }
  std::cout << std::flush;
}

}

#define DEBUG_LOG
#define DEBUG_ASSERT
#define DEBUG_ERROR

#ifdef DEBUG_LOG
// printf("\033[33;44myellow on blue\n");

#define DEBUG_YELLOW "\033[33;33m"
#define DEBUG_RESET "\033[0m"

#define LOG(msg, ...) \
    do { \
        printf("%s[%s]%s: ", DEBUG_YELLOW, __PRETTY_FUNCTION__, DEBUG_RESET); \
        printf(msg, ##__VA_ARGS__); \
        printf("\n"); \
    } while (0)

#else
#define LOG(msg, ...) 
#endif // DEBUG_LOG

#ifdef DEBUG_ASSERT
#define DEBUG_RED     "\033[1;31m"
#define DEBUG_RESET   "\033[0m"

#define ASSERT(condition, msg, ...) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "%sAssertion failed in %s:%s %s | ", DEBUG_RED, __PRETTY_FUNCTION__, DEBUG_RESET, #condition); \
            fprintf(stderr, msg, ##__VA_ARGS__); \
            fprintf(stderr, "\n"); \
            debug::print_stack_trace(); \
            std::abort(); \
        } \
    } while (0)

/*std::cerr << DEBUG_RED << "Assertion failed in " << __PRETTY_FUNCTION__ << ": " \*/
/*<< DEBUG_RESET << #condition << ", " << msg << "\n"; \*/
/*std::abort(); \*/
#else
#define ASSERT(condition, msg, ...) 
#endif // DEBUG_ASSERT

#ifdef DEBUG_ERROR
#define DEBUG_RED     "\033[1;31m"
#define DEBUG_RESET   "\033[0m"

#define ERROR(msg, ...) \
    do { \
        fprintf(stderr, "%s[ERROR -> %s]%s: ", DEBUG_RED, __PRETTY_FUNCTION__, DEBUG_RESET); \
        fprintf(stderr, msg, ##__VA_ARGS__); \
        fprintf(stderr, "\n"); \
        std::abort(); \
    } while (0)

#else
#define ERROR(msg, ...)  
#endif // DEBUG_ERROR

