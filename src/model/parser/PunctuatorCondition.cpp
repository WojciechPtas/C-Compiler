#include <stdexcept>

#include "../token/PunctuatorToken.h"
#include "ITokenConditionVerifier.h"
#include "PunctuatorCondition.h"

using namespace c4::model::parser;
using namespace c4::model::token;
using namespace std;

// --- Internal declarations ---

class _Verifier : public ITokenConditionVerifier {
public:
    _Verifier(Punctuator punctuator);
    virtual ~_Verifier() { }

    bool isApplying() const override;

    void visit(const CharacterConstantToken &token) override;
    void visit(const DecimalConstantToken &token) override;
    void visit(const ErrorToken &token) override;
    void visit(const IdentifierToken &token) override;
    void visit(const KeywordToken &token) override;
    void visit(const PunctuatorToken &token) override;
    void visit(const StringLiteralToken &token) override;

private:
    const Punctuator punctuator;
    bool applying;
    bool executed;
};

// --- Exported definitions ---

PunctuatorCondition::PunctuatorCondition(Punctuator punctuator) :
    punctuator(punctuator) { }

unique_ptr<ITokenConditionVerifier> PunctuatorCondition::createVerifier() const {
    return make_unique<_Verifier>(this->punctuator);
}

// --- Corresponding ITokenConditionVerifier ---

_Verifier::_Verifier(Punctuator punctuator) : punctuator(punctuator) {
    this->applying = false;
    this->executed = false;
}

bool _Verifier::isApplying() const {
    if (!this->executed) {
        throw logic_error("Verifier not executed yet");
    }

    return this->applying;
}

void _Verifier::visit(const CharacterConstantToken &token) {
    (void) token;

    if (this->executed) {
        throw logic_error("Already executed");
    }

    this->executed = true;
}

void _Verifier::visit(const DecimalConstantToken &token) {
    (void) token;

    if (this->executed) {
        throw logic_error("Already executed");
    }

    this->executed = true;
}

void _Verifier::visit(const ErrorToken &token) {
    (void) token;
    
    if (this->executed) {
        throw logic_error("Already executed");
    }

    this->executed = true;
}

void _Verifier::visit(const IdentifierToken &token) {
    (void) token;
    
    if (this->executed) {
        throw logic_error("Already executed");
    }

    this->executed = true;
}

void _Verifier::visit(const KeywordToken &token) {
    (void) token;
    
    if (this->executed) {
        throw logic_error("Already executed");
    }

    this->executed = true;
}

void _Verifier::visit(const PunctuatorToken &token) {
    if (this->executed) {
        throw logic_error("Already executed");
    }

    this->applying = token.getPunctuator() == this->punctuator;
    this->executed = true;
}

void _Verifier::visit(const StringLiteralToken &token) {
    (void) token;

    if (this->executed) {
        throw logic_error("Already executed");
    }

    this->executed = true;
}
