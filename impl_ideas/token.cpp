#include "token.h"

Token::Token(Locatable& loc, TokenKind kind, Symbol text)
    : Locatable(loc), Kind(kind), Text(text) {}

std::ostream& operator<<(std::ostream& stream, const Token& tok) {
    stream << static_cast<const Locatable&>(tok) << ' ';

#define KIND_ACTION(KD, STR) \
    case KD:                 \
        stream << STR;       \
        break;

    switch (tok.Kind) {
#include "tokenkinds.def"
    }

#undef KIND_ACTION

    stream << ' ' << *(tok.Text);

    return stream;
}
