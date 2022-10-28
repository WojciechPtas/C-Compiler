#pragma once

// Declarators and abstract declarators share a lot of syntactical and
// semantical properties. Therefore, we use the same AST data structures to
// represent both. If at some place, a specific kind of declarators is required
// (by giving one of the below `DeclKind`s), we need to check separately whether
// the declarator satisfies this requirement.
enum class DeclKind { ANY, ABSTRACT, CONCRETE };

/// This class should implement the parser that translates a stream of tokens
/// into an abstract syntax tree.
class Parser {
   public:
   private:
    /// Advance the parsing state to the next token.
    void popToken(void);

    /// Return (a reference to) the current token.
    const Token& peekToken(void);

    /// Return the location of the current token.
    const Locatable& getLoc(void);

    /// Check if the current token has TokenKind `tk`. If it has, advance the
    /// parsing state to the next token, otherwise note an error that `txt` was
    /// expected but something else encountered.
    void expect(TokenKind tk, const char* txt);

    /// Check if the current token has TokenKind `tk`. If it has, advance the
    /// parsing state to the next token and return true. Otherwise, return
    /// false and keep the parsing state unchanged.
    bool accept(TokenKind tk);

    /// Check if the current token has TokenKind `tk`. If it has, return true.
    /// Otherwise, return false. Keep the parsing state unchanged in both cases.
    bool check(TokenKind tk);

    /// Check if the token after the current token has TokenKind `tk` and
    /// return true iff this is the case. Keep the parsing state unchanged in
    /// both cases.
    bool checkLookAhead(TokenKind tk);

    /// Internal methods for use in parseSpecDecl()
    Declarator* parseDeclarator(void);
    Declarator* parseNonFunDeclarator(void);

    /// Parse a type specifier followed by an abstract or non-abstract
    /// declarator (i.e. a typename or a declaration).
    /// The current token is expected to be the first token of the type
    /// specifier
    ///
    /// If `dKind` is `DeclKind::ABSTRACT`: verify that the declarator is a
    /// valid abstract declarator.
    /// If `dKind` is `DeclKind::CONCRETE`: verify that the declarator is a
    /// valid non-abstract declarator.
    /// If `dKind` is `DeclKind::ANY`: do not verify abstractness.
    SpecDecl* parseSpecDecl(DeclKind dKind);
};
