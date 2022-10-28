#include "diagnostic.h"

Locatable::Locatable(const std::string& FileName, size_t Line, size_t Column)
    : FileName(FileName), Line(Line), Column(Column) {}

Locatable::Locatable(const Locatable& loc)
    : FileName(loc.FileName), Line(loc.Line), Column(loc.Column) {}

Locatable::~Locatable(void) {}

Locatable& Locatable::operator=(const Locatable& loc) {
    this->Line = loc.Line;
    this->Column = loc.Column;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Locatable& loc) {
    stream << loc.FileName << ':' << loc.Line << ':' << loc.Column << ":";
    return stream;
}
