#include <stdexcept>

#include "../../model/expression/IdentifierExpression.h"
#include "../../model/expression/MemberExpression.h"
#include "MemberReduction.h"

using namespace c4::model::expression;
using namespace c4::util::parser;
using namespace std;

MemberReduction::MemberReduction(MemberExpressionType type) :
    PureReduction(3, 2), type(type) { }

shared_ptr<const IExpression> MemberReduction::reduce(
    const vector<shared_ptr<const IExpression>> &consumedExpressions
) const {
    auto container = consumedExpressions[0];
    auto member = consumedExpressions[1];

    // NOTE:    Since MemberExpression expects member to be an
    //          IdentifierExpression, it is necessary to ensure this. Also,
    //          there is no way to know whether member is a actually an
    //          IdentifierExpression without either implementing a pretty
    //          stupid visitor or doing a dynamic_cast.
    //          Since the visitor code would be unecessary because it basically
    //          does the same as the dynamic_cast, we prefer the latter here.

    auto identifier = dynamic_pointer_cast<const IdentifierExpression>(member);

    if (identifier == nullptr) {
        // FIXME:   Add error handling.

        throw logic_error("Expected identifier");
    }

    return make_shared<MemberExpression>(this->type, container, identifier);
}
