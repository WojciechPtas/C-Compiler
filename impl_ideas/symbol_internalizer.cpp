#include "symbol_internalizer.h"

Symbol SymbolInternalizer::internalize(const std::string& str) {
    // Using the address of unordered_set elements is allowed by the standard:
    // "Rehashing invalidates iterators, changes ordering between elements, and
    // changes which buckets elements appear in, but does not invalidate
    // pointers or references to elements."
    const auto& p = StringSet.emplace(str);
    return std::addressof(*p.first);
}

Symbol SymbolInternalizer::internalize(const char* str) {
    const auto& p = StringSet.emplace(str);
    return std::addressof(*p.first);
}
