#pragma once

#include "IExpression.h"
#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;

namespace c4 {
namespace model {
namespace expression {

class CallArguments : public IExpression {
    public:
        vector<shared_ptr<const IExpression>> arguments;

        CallArguments() 
        : IExpression(nullptr)
        {}

        CallArguments(shared_ptr<const IExpression> singleArgument) : CallArguments() {
            addArgument(singleArgument);
        }

        CallArguments(shared_ptr<const CallArguments> args, shared_ptr<const IExpression> nextArg) 
        : CallArguments(*args) {
            addArgument(nextArg);
        }
        
        ~CallArguments() { }
        
        void addArgument(const shared_ptr<const IExpression> &expr) {
            arguments.push_back(expr);
        }

        void accept(IExpressionVisitor &visitor) const override {
            visitor.visit(*this);
        }

        virtual ctype::CTypedValue getLValue(IExpressionCodeGenVisitor &cg) const override {
            throw std::logic_error("These should not exist");
            return ctype::CTypedValue::invalid();
        }

        virtual ctype::CTypedValue getRValue(IExpressionCodeGenVisitor &cg) const override {
            throw std::logic_error("These should not exist");
            return ctype::CTypedValue::invalid();
        }
    };






}
}
}