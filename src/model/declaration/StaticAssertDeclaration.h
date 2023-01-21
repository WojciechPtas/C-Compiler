#include "IDeclaration.h"
#include "../expression/IdentifierExpression.h"
namespace c4::model::declaration{

    class StaticAssertDeclaration : public IDeclaration{
        private:
        model::expression::IdentifierExpression expr;
        public:
        StaticAssertDeclaration(model::expression::IdentifierExpression expr) : expr(expr) {};
    };
}