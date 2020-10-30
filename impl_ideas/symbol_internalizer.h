#pragma once

#include <string>
#include <unordered_set>

using Symbol = const std::string*;

/// This class provides means to manage encountered strings for later use in
/// the compiler.
/// If such strings are passed to an `internalize()` method, a Symbol that
/// uniquely identifies the string contents is returned.
///
/// Since Symbols are deduplicated, comparisons of Symbols can be implemented
/// via a cheap pointer comparison (`==`).
/// If you want to print the Symbol or access its contents, dereference it to
/// obtain a std::string.
///
/// Symbols are guaranteed to be accessible as long as the SymbolInternalizer
/// is not destroyed.
class SymbolInternalizer {
   public:
    SymbolInternalizer(size_t prealloc_size = 512) : StringSet(prealloc_size) {}

    /// Internalize the given argument. The string is copied and does not need
    /// to be preserved by the caller.
    Symbol internalize(const std::string& str);

    /// Internalize the given argument. The string is copied and does not need
    /// to be preserved by the caller.
    Symbol internalize(const char* str);

   private:
    std::unordered_set<std::string> StringSet;
};
