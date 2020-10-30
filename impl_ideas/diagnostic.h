#pragma once

#include <iostream>

/// Base class of everything that has a source code location.
struct Locatable {
   public:
    const std::string& FileName;
    size_t Line;
    size_t Column;

    Locatable(const std::string& FileName, size_t Line, size_t Column);
    Locatable(const Locatable& loc);
    Locatable& operator=(const Locatable& loc);
    virtual ~Locatable(void);

    friend std::ostream& operator<<(std::ostream& stream, const Locatable& loc);
};

/// not for external use
/// Base case of variadic template function for noting errors.
template <typename T>
[[noreturn]] void errorf_impl(const T msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}

/// not for external use
/// Recursive case of variadic template function for noting errors.
template <typename T, typename... Args>
[[noreturn]] void errorf_impl(const T msg, Args... args) {
    std::cerr << msg;
    errorf_impl(args...);
}

/// Print an error message that is the concatenation of all template arguments
/// with a prefix indicating the source position of the given locatable.
/// Then, terminate execution with the appropriate error code.
/// This function should be used for most error messages.
///
/// Example use:
/// if (...) {
///     errorloc(loc, "invalid construct: ", x);
/// }
template <typename... Args>
[[noreturn]] void errorloc(const Locatable& loc, Args... args) {
    std::cerr << loc << " error: ";
    errorf_impl(args...);
}

/// Print an error message that is the concatenation of all template arguments
/// without a location.
/// Then, terminate execution with the appropriate error code.
/// Only use this for errors that are not tied to a source location, like
/// invalid command line arguments.
template <typename... Args>
[[noreturn]] void error(Args... args) {
    std::cerr << "error: ";
    errorf_impl(args...);
}
