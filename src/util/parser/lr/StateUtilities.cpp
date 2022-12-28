#include <stdexcept>

#include "../../../model/expression/BinaryExpression.h"
#include "../../../model/expression/ConditionalExpression.h"
#include "../../../model/expression/ConstantExpression.h"
#include "../../../model/expression/IdentifierExpression.h"
#include "../../../model/expression/IndexExpression.h"
#include "../../../model/expression/MemberExpression.h"
#include "../../../model/expression/UnaryExpression.h"

#include "../../../model/token/ConstantToken.h"
#include "../../../model/token/IdentifierToken.h"

#include "ExpressionConditionUtilities.h"
#include "LookaheadConditionUtilities.h"
#include "StateUtilities.h"

using namespace c4::model::expression;
using namespace c4::model::parser::lr;
using namespace c4::model::token;
using namespace c4::util::parser::lr;
using namespace std;
using namespace std::placeholders;

static shared_ptr<const State> _initialize();

static const shared_ptr<State> _additionReductionState;
static const shared_ptr<State> _additionState;
static const shared_ptr<State> _assignmentReductionState;
static const shared_ptr<State> _assignmentState;
static const shared_ptr<State> _bracketReductionState;
static const shared_ptr<State> _compareEqualReductionState;
static const shared_ptr<State> _compareEqualState;
static const shared_ptr<State> _compareUnequalReductionState;
static const shared_ptr<State> _compareUnequalState;
static const shared_ptr<State> _conditionalAlternativeState;
static const shared_ptr<State> _conditionalState;
static const shared_ptr<State> _constantReducedState;
static const shared_ptr<State> _directMemberAccessState;
static const shared_ptr<State> _directMemberAccessReductionState;
static const shared_ptr<State> _identifierReducedState;
static const shared_ptr<State> _leftBracketState;
static const shared_ptr<State> _leftParenthesisState;
static const shared_ptr<State> _lessThanState;
static const shared_ptr<State> _lessThanReductionState;
static const shared_ptr<State> _logicalAndState;
static const shared_ptr<State> _logicalOrState;
static const shared_ptr<State> _multiplicationState;
static const shared_ptr<State> _multiplicationReductionState;
static const shared_ptr<State> _operatorOrColonState;
static const shared_ptr<State> _operatorOrConditionalReduction;
static const shared_ptr<State> _operatorOrLogicalAndReduction;
static const shared_ptr<State> _operatorOrLogicalOrReduction;
static const shared_ptr<State> _operatorOrRightParenthesisState;
static const shared_ptr<State> _operatorOrRightBracketState;
static const shared_ptr<State> _operatorState;
static const shared_ptr<State> _parenthesesReductionState;
static const shared_ptr<State> _pointerMemberAccessState;
static const shared_ptr<State> _pointerMemberAccessReductionState;
static const shared_ptr<State> _subtractionReductionState;
static const shared_ptr<State> _subtractionState;
static const shared_ptr<State> _unaryAddressOfReductionState;
static const shared_ptr<State> _unaryAddressOfState;
static const shared_ptr<State> _unaryArithmeticNegationReductionState;
static const shared_ptr<State> _unaryArithmeticNegationState;
static const shared_ptr<State> _unaryDereferenceReductionState;
static const shared_ptr<State> _unaryDereferenceState;
static const shared_ptr<State> _unaryLogicNegationReductionState;
static const shared_ptr<State> _unaryLogicNegationState;
static const shared_ptr<State> _unarySizeOfReductionState;
static const shared_ptr<State> _unarySizeOfState;

static const shared_ptr<const State> _initialState = _initialize();
const State &c4::util::parser::lr::INITIAL_STATE = *_initialState;

// Reduction declarations

static shared_ptr<const IExpression> _reduceBinary(
    vector<shared_ptr<const IExpression>> consumed,
    BinaryExpressionType type
);

static shared_ptr<const IExpression> _reduceConditional(
    vector<shared_ptr<const IExpression>> consumed
);

static unique_ptr<const IExpression> _reduceConstant(const Token &token);
static unique_ptr<const IExpression> _reduceIdentifier(const Token &token);
static shared_ptr<const IExpression> _reduceIndex(
    vector<shared_ptr<const IExpression>> consumed
);

static shared_ptr<const IExpression> _reduceMember(
    vector<shared_ptr<const IExpression>> consumed,
    MemberExpressionType type
);

static shared_ptr<const IExpression> _reduceParentheses(
    vector<shared_ptr<const IExpression>> consumed
);

static shared_ptr<const IExpression> _reduceUnary(
    vector<shared_ptr<const IExpression>> consumed,
    UnaryExpressionType type
);

// State initialization helper declarations

static inline void _addAdditiveShifts(State &state);
static inline void _addAssignmentShift(State &state);
static inline void _addComparisonShifts(State &state);
static inline void _addConditionalShift(State &state);
static inline void _addLogicalAndShift(State &state);
static inline void _addLogicalOrShift(State &state);
static inline void _addMultiplicationShift(State &state);
static inline void _addPostfixShifts(State &state);
static inline void _addPrimaryShifts(State &state);
static inline void _addRelationalShift(State &state);
static inline void _addUnaryShifts(State &state);

// States initializer

static shared_ptr<const State> _initialize() {
    auto _initial = make_shared<State>();

    // State: _additionReductionState

    _addMultiplicationShift(*_additionReductionState);
    _additionReductionState->addReduction(
        END_TOKEN | PUNCTUATOR_TOKEN(
            Punctuator::Colon |
            Punctuator::DoubleAnd |
            Punctuator::DoubleEqual |
            Punctuator::DoublePipe |
            Punctuator::Equal |
            Punctuator::ExclamationMarkEqual |
            Punctuator::LessThan |
            Punctuator::QuestionMark
        ),
        3, 2,
        bind(_reduceBinary, _1, BinaryExpressionType::Sum)
    );

    // State: _additionState

    _addUnaryShifts(*_additionState);
    _additionState->addJump(ANY_EXPRESSION, _additionReductionState);

    // State: _assignmentReductionState

    _assignmentReductionState->addReduction(
        ANY_TOKEN,
        3, 2,
        bind(_reduceBinary, _1, BinaryExpressionType::Assignment)
    );

    // State: _assignmentState

    _addUnaryShifts(*_assignmentState);
    _assignmentState->addJump(ANY_EXPRESSION, _assignmentReductionState);

    // State: _bracketReductionState

    _bracketReductionState->addReduction(ANY_TOKEN, 4, 2, _reduceIndex);

    // State: _compareEqualReductionState

    _addRelationalShift(*_compareEqualReductionState);
    _compareEqualReductionState->addReduction(
        END_TOKEN | PUNCTUATOR_TOKEN(
            Punctuator::Colon |
            Punctuator::DoubleAnd |
            Punctuator::DoublePipe |
            Punctuator::Equal |
            Punctuator::QuestionMark
        ),
        3, 2,
        bind(_reduceBinary, _1, BinaryExpressionType::Equal)
    );

    // State: _compareEqualState

    _addUnaryShifts(*_compareEqualState);
    _compareEqualState->addJump(ANY_EXPRESSION, _compareEqualReductionState);

    // State: _compareUnequalReductionState

    _addRelationalShift(*_compareUnequalReductionState);
    _compareUnequalReductionState->addReduction(
        END_TOKEN | PUNCTUATOR_TOKEN(
            Punctuator::Colon |
            Punctuator::DoubleAnd |
            Punctuator::DoublePipe |
            Punctuator::Equal |
            Punctuator::QuestionMark
        ),
        3, 2,
        bind(_reduceBinary, _1, BinaryExpressionType::Unequal)
    );

    // State: _compareUnequalState

    _addUnaryShifts(*_compareUnequalState);
    _compareUnequalState->addJump(
        ANY_EXPRESSION,
        _compareUnequalReductionState
    );

    // State: _conditionalAlternativeState

    _addUnaryShifts(*_conditionalAlternativeState);
    _conditionalAlternativeState->addJump(
        ANY_EXPRESSION,
        _operatorOrConditionalReduction
    );

    // State: _conditionalState

    _addUnaryShifts(*_conditionalState);
    _conditionalState->addJump(ANY_EXPRESSION, _operatorOrColonState);

    // State: _constantReductionState

    _constantReducedState->addReduction(ANY_TOKEN, 1);

    // State: _constantReducedState

    _constantReducedState->addReduction(ANY_TOKEN, 1);

    // State: _directMemberAccessReductionState

    _directMemberAccessReductionState->addReduction(
        ANY_TOKEN,
        3, 2,
        bind(_reduceMember, _1, MemberExpressionType::Direct)
    );

    // State: _directMemberAccessState

    _directMemberAccessState->addShift(
        IDENTIFIER_TOKEN,
        _directMemberAccessReductionState,
        _reduceIdentifier
    );

    // State: _identifierReducedState

    _identifierReducedState->addReduction(ANY_TOKEN, 1);

    // State: _initial

    _addUnaryShifts(*_initial);
    _initial->addJump(ANY_EXPRESSION, _operatorState);

    // state: _leftBracketState

    _addUnaryShifts(*_leftBracketState);
    _leftBracketState->addJump(ANY_EXPRESSION, _operatorOrRightBracketState);

    // State: _leftParenthesisState

    _addUnaryShifts(*_leftParenthesisState);
    _leftParenthesisState->addJump(
        ANY_EXPRESSION,
        _operatorOrRightParenthesisState
    );

    // State: _lessThanReductionState

    _addAdditiveShifts(*_lessThanReductionState);
    _lessThanReductionState->addReduction(
        END_TOKEN | PUNCTUATOR_TOKEN(
            Punctuator::Colon |
            Punctuator::DoubleAnd |
            Punctuator::DoubleEqual |
            Punctuator::DoublePipe |
            Punctuator::Equal |
            Punctuator::ExclamationMarkEqual |
            Punctuator::QuestionMark
        ),
        3, 2,
        bind(_reduceBinary, _1, BinaryExpressionType::LessThan)
    );

    // State: _lessThanState

    _addUnaryShifts(*_lessThanState);
    _lessThanState->addJump(ANY_EXPRESSION, _lessThanReductionState);

    // State: _logicalAndState

    _addUnaryShifts(*_logicalAndState);
    _logicalAndState->addJump(ANY_EXPRESSION, _operatorOrLogicalAndReduction);

    // State: _logicalOrState

    _addUnaryShifts(*_logicalOrState);
    _logicalOrState->addJump(ANY_EXPRESSION, _operatorOrLogicalOrReduction);

    // State: _multiplicationReductionState

    _addPostfixShifts(*_multiplicationReductionState);
    _multiplicationReductionState->addReduction(
        END_TOKEN | PUNCTUATOR_TOKEN(
            Punctuator::Colon |
            Punctuator::DoubleAnd |
            Punctuator::DoubleEqual |
            Punctuator::DoublePipe |
            Punctuator::Equal |
            Punctuator::ExclamationMarkEqual |
            Punctuator::LessThan |
            Punctuator::Minus |
            Punctuator::Plus |
            Punctuator::QuestionMark
        ),
        3, 2,
        bind(_reduceBinary, _1, BinaryExpressionType::Multiplication)
    );

    // State: _multiplicationState

    _addUnaryShifts(*_multiplicationState);
    _multiplicationState->addJump(
        ANY_EXPRESSION,
        _multiplicationReductionState
    );

    // State: _operatorOrColonState

    _addAssignmentShift(*_operatorOrColonState);
    _operatorOrColonState->addShift(
        PUNCTUATOR_TOKEN(Punctuator::Colon),
        _conditionalAlternativeState
    );

    // State: _operatorOrConditionalReduction

    _addConditionalShift(*_operatorOrConditionalReduction);
    _operatorOrConditionalReduction->addReduction(
        END_TOKEN | PUNCTUATOR_TOKEN(Punctuator::Equal),
        5, 3,
        _reduceConditional
    );

    // State: _operatorOrLogicalAndReduction

    _addComparisonShifts(*_operatorOrLogicalAndReduction);
    _operatorOrLogicalAndReduction->addReduction(
        END_TOKEN | PUNCTUATOR_TOKEN(
            Punctuator::Colon |
            Punctuator::DoublePipe |
            Punctuator::Equal |
            Punctuator::QuestionMark
        ),
        3, 2,
        bind(_reduceBinary, _1, BinaryExpressionType::LogicalAnd)
    );

    // State: _operatorOrLogicalOrReduction

    _addLogicalAndShift(*_operatorOrLogicalOrReduction);
    _operatorOrLogicalOrReduction->addReduction(
        END_TOKEN | PUNCTUATOR_TOKEN(
            Punctuator::Colon |
            Punctuator::Equal |
            Punctuator::QuestionMark
        ),
        3, 2,
        bind(_reduceBinary, _1, BinaryExpressionType::LogicalOr)
    );

    // State: _operatorOrRightBracketState
    
    _addAssignmentShift(*_operatorOrRightBracketState);
    _operatorOrRightBracketState->addShift(
        PUNCTUATOR_TOKEN(Punctuator::RightBracket),
        _bracketReductionState
    );

    // State: _operatorOrRightParenthesisState

    _operatorOrRightParenthesisState->addShift(
        PUNCTUATOR_TOKEN(Punctuator::RightParenthesis),
        _parenthesesReductionState
    );

    _addAssignmentShift(*_operatorOrRightParenthesisState);

    // State: _operatorState

    _addAssignmentShift(*_operatorState);

    // State: _parenthesesReductionState

    _parenthesesReductionState->addReduction(
        ANY_TOKEN,
        3, 1,
        _reduceParentheses
    );

    // State: _pointerMemberAccessReductionState

    _pointerMemberAccessReductionState->addReduction(
        ANY_TOKEN,
        3, 2,
        bind(_reduceMember, _1, MemberExpressionType::Pointer)
    );

    // State: _pointerMemberAccessState

    _pointerMemberAccessState->addShift(
        IDENTIFIER_TOKEN,
        _pointerMemberAccessReductionState,
        _reduceIdentifier
    );

    // State: _subtractionReductionState

    _addMultiplicationShift(*_subtractionReductionState);
    _subtractionReductionState->addReduction(
        END_TOKEN | PUNCTUATOR_TOKEN(
            Punctuator::Colon |
            Punctuator::DoubleAnd |
            Punctuator::DoubleEqual |
            Punctuator::DoublePipe |
            Punctuator::Equal |
            Punctuator::ExclamationMarkEqual |
            Punctuator::LessThan |
            Punctuator::QuestionMark
        ),
        3, 2,
        bind(_reduceBinary, _1, BinaryExpressionType::Subtraction)
    );

    // State: _subtractionState

    _addUnaryShifts(*_subtractionState);
    _subtractionState->addJump(ANY_EXPRESSION, _subtractionReductionState);

    // State: _unaryAddressOfReductionState

    _unaryAddressOfReductionState->addReduction(
        ANY_TOKEN,
        2, 1,
        bind(_reduceUnary, _1, UnaryExpressionType::AddressOf)
    );

    // State: _unaryAddressOfState

    _addUnaryShifts(*_unaryAddressOfState);
    _unaryAddressOfState->addJump(
        ANY_EXPRESSION,
        _unaryAddressOfReductionState
    );

    // _unaryArithmeticNegationReductionState

    _unaryArithmeticNegationReductionState->addReduction(
        ANY_TOKEN,
        2, 1,
        bind(_reduceUnary, _1, UnaryExpressionType::AdditiveInverse)
    );

    // State: _unaryArithmeticNegationState

    _addUnaryShifts(*_unaryArithmeticNegationState);
    _unaryArithmeticNegationState->addJump(
        ANY_EXPRESSION,
        _unaryArithmeticNegationReductionState
    );

    // State: _unaryDereferenceReductionState

    _unaryDereferenceReductionState->addReduction(
        ANY_TOKEN,
        2, 1,
        bind(_reduceUnary, _1, UnaryExpressionType::Indirection)
    );

    // State: _unaryDereferenceState

    _addUnaryShifts(*_unaryDereferenceState);
    _unaryDereferenceState->addJump(
        ANY_EXPRESSION,
        _unaryDereferenceReductionState
    );

    // State: _unaryLogicNegationReductionState

    _unaryLogicNegationReductionState->addReduction(
        ANY_TOKEN,
        2, 1,
        bind(_reduceUnary, _1, UnaryExpressionType::LogicalInverse)
    );

    // State: _unaryLogicNegationState

    _addUnaryShifts(*_unaryLogicNegationState);
    _unaryLogicNegationState->addJump(
        ANY_EXPRESSION,
        _unaryLogicNegationReductionState
    );

    // State: _unarySizeOfReductionState

    _unarySizeOfReductionState->addReduction(
        ANY_TOKEN,
        2, 1,
        bind(_reduceUnary, _1, UnaryExpressionType::Sizeof)
    );

    // State: _unarySizeOfState

    _addUnaryShifts(*_unarySizeOfState);
    _unarySizeOfState->addJump(ANY_EXPRESSION, _unarySizeOfReductionState);

    return _initial;
}

//

static shared_ptr<const IExpression> _reduceBinary(
    vector<shared_ptr<const IExpression>> consumed,
    BinaryExpressionType type
) {
    // Invariant:   First element of consumed is left expression, second
    //              element is right expression.

    return make_shared<BinaryExpression>(type, consumed[0], consumed[1]);
}

static shared_ptr<const IExpression> _reduceConditional(
    vector<shared_ptr<const IExpression>> consumed
) {
    // Invariant:   First element of consumed is condition, second is then
    //              case, third is else case.

    return make_shared<ConditionalExpression>(
        consumed[0],
        consumed[1],
        consumed[2]
    );
}

static unique_ptr<const IExpression> _reduceConstant(const Token &token) {
    // Invariant:   The token argument (aka lookahead) is a ConstantToken.
    //              Hence, the following dynamic_cast is safe.

    auto constantToken = dynamic_cast<const ConstantToken&>(token);
    return make_unique<ConstantExpression>(
        constantToken.type,
        constantToken.value
    );
}

static unique_ptr<const IExpression> _reduceIdentifier(const Token &token) {
    // Invariant:   The token argument (aka lookahead) is an IdentifierToken.
    //              Hence, the following dynamic_cast is safe.

    auto identifierToken = dynamic_cast<const IdentifierToken&>(token);
    return make_unique<IdentifierExpression>(identifierToken.identifier);
}

static shared_ptr<const IExpression> _reduceIndex(
    vector<shared_ptr<const IExpression>> consumed
) {
    // Invariant:   The first element of consumed is the container, the second
    //              one is the index.

    return make_shared<IndexExpression>(consumed[0], consumed[1]);
}

static shared_ptr<const IExpression> _reduceMember(
    vector<shared_ptr<const IExpression>> consumed,
    MemberExpressionType type
) {
    // Invariant:   The first element of consumed is the container, the second
    //              one is the member.
    //              Also, the second one is an IdentifierExpression.

    auto container = consumed[0];
    auto member = dynamic_pointer_cast<const IdentifierExpression>(
        consumed[1]
    );

    if (member == nullptr) {
        throw logic_error("Unexpected member expression");
    }

    return make_shared<MemberExpression>(type, container, member);
}

static shared_ptr<const IExpression> _reduceParentheses(
    vector<shared_ptr<const IExpression>> consumed
) {
    // Invariant:   consumed contains exactly one expression.
    //
    // NOTE:    Since parentheses do not exist in the AST and are only required
    //          for the "linear" source code representation, we can simply
    //          forward the consumed expression without adjustments.

    return consumed[0];
}

static shared_ptr<const IExpression> _reduceUnary(
    vector<shared_ptr<const IExpression>> consumed,
    UnaryExpressionType type
) {
    // Invariant:   consumed constains exactly one expression.

    return make_unique<UnaryExpression>(type, consumed[0]);
}

//

static inline void _addAdditiveShifts(State &state) {
    _addMultiplicationShift(state);

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::Plus),
        _additionState
    );

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::Minus),
        _subtractionState
    );
}

static inline void _addAssignmentShift(State &state) {
    _addConditionalShift(state);

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::DoubleEqual),
        _assignmentState
    );
}

static inline void _addComparisonShifts(State &state) {
    _addRelationalShift(state);

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::DoubleEqual),
        _compareEqualState
    );

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::ExclamationMarkEqual),
        _compareUnequalState
    );
}

static inline void _addConditionalShift(State &state) {
    _addLogicalOrShift(state);

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::QuestionMark),
        _conditionalState
    );
}

static inline void _addLogicalAndShift(State &state) {
    _addComparisonShifts(state);

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::DoubleAnd),
        _logicalAndState
    );
}

static inline void _addLogicalOrShift(State &state) {
    _addLogicalAndShift(state);

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::DoublePipe),
        _logicalOrState
    );
}

static inline void _addMultiplicationShift(State &state) {
    _addPostfixShifts(state);

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::Asterisk),
        _multiplicationState
    );
}

static inline void _addPostfixShifts(State &state) {
    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::LeftBracket),
        _leftBracketState
    );

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::Dot),
        _directMemberAccessState
    );

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::DashGreaterThan),
        _pointerMemberAccessState
    );
}

static inline void _addPrimaryShifts(State &state) {
    state.addShift(CONSTANT_TOKEN, _constantReducedState, _reduceConstant);
    state.addShift(
        IDENTIFIER_TOKEN,
        _identifierReducedState,
        _reduceIdentifier
    );

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::LeftParenthesis),
        _leftParenthesisState
    );
}

static inline void _addRelationalShift(State &state) {
    _addAdditiveShifts(state);

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::LessThan),
        _lessThanState
    );
}

static inline void _addUnaryShifts(State &state) {
    _addPrimaryShifts(state);

    state.addShift(PUNCTUATOR_TOKEN(Punctuator::And), _unaryAddressOfState);
    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::Minus),
        _unaryArithmeticNegationState
    );

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::Asterisk),
        _unaryDereferenceState
    );

    state.addShift(
        PUNCTUATOR_TOKEN(Punctuator::ExclamationMark),
        _unaryLogicNegationState
    );

    state.addShift(KEYWORD_TOKEN(Keyword::Sizeof), _unarySizeOfState);
}
