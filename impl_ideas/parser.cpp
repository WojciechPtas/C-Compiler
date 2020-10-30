#include "parser.h"

#include "diagnostic.h"
#include "token.h"

SpecDecl* Parser::parseSpecDecl(DeclKind dKind) {
    Specifier* spec = nullptr;
    Locatable loc(getLoc());
    if (accept(TK_VOID)) {
        spec = new VoidSpecifier(loc);
    } else if (accept(TK_CHAR)) {
        spec = new CharSpecifier(loc);
    } else if (accept(TK_INT)) {
        spec = new IntSpecifier(loc);
    } else if (accept(TK_STRUCT)) {
        Symbol tag = nullptr;
        if (check(TK_IDENTIFIER)) {
            tag = peekToken().Text;
            popToken();
        } else if (!check(TK_LBRACE)) {
            errorloc(getLoc(), "Expected struct declaration list but got `",
                     *peekToken().Text, "'");
        }
        auto* structSpec = new StructSpecifier(loc, tag);

        if (accept(TK_LBRACE)) {
            do {  // parse struct member declarations
                auto* specDecl = parseSpecDecl(DeclKind::CONCRETE);
                structSpec->addComponent(specDecl);
                expect(TK_SEMICOLON, ";");
            } while (!accept(TK_RBRACE));
        }
        spec = structSpec;

    } else {
        errorloc(loc, "Expected type specifier but got `", *peekToken().Text,
                 "'");
    }

    Declarator* decl = parseDeclarator();

    if (!decl->isEmptyDeclarator()) {
        // At all places where non-abstractness is required, declarators are
        // also optional in the grammar, therefore it's okay to accept a simple
        // empty primitive declarator here anyway.

        // TODO check if decl is a valid declarator or abstract declarator and
        //      that it fits with what is required by dKind
    }

    return new SpecDecl(spec, decl);
}

Declarator* Parser::parseNonFunDeclarator(void) {
    switch (peekToken().Kind) {
        case TK_LPAREN: {
            if (checkLookAhead(TK_RPAREN) || checkLookAhead(TK_VOID) ||
                checkLookAhead(TK_CHAR) || checkLookAhead(TK_INT) ||
                checkLookAhead(TK_STRUCT)) {
                // this has to be an abstract function declarator, not a
                // parenthesized declarator, so we add an empty primitive
                // declarator
                return new PrimitiveDeclarator(getLoc(), nullptr);
            }
            popToken();
            auto* res = parseDeclarator();
            expect(TK_RPAREN, ")");
            return res;
        }

        case TK_ASTERISK: {
            Locatable loc(getLoc());
            popToken();
            auto* inner = parseDeclarator();
            return new PointerDeclarator(loc, inner);
        }

        case TK_IDENTIFIER: {
            auto* res = new PrimitiveDeclarator(getLoc(), peekToken().Text);
            popToken();
            return res;
        }
        default:
            return new PrimitiveDeclarator(getLoc(), nullptr);
    }
}

Declarator* Parser::parseDeclarator(void) {
    Declarator* res = parseNonFunDeclarator();
    while (check(TK_LPAREN)) {
        auto* funDecl = new FunctionDeclarator(getLoc(), res);
        res = funDecl;
        popToken();

        if (accept(TK_RPAREN)) {
            continue;
        }

        do {
            auto* param = parseSpecDecl(DeclKind::ANY);
            funDecl->addParameter(param);
        } while (accept(TK_COMMA));

        expect(TK_RPAREN, ")");
    }
    return res;
}
