#pragma once

#include "IExpression.h"
#include "../token/Keyword.h"

namespace c4 {
    namespace model {
        namespace expression {
            class TypeInSizeof : public IExpression {
            public:
                token::Keyword type;

                TypeInSizeof(const token::Keyword type) :
                    IExpression(nullptr), type(type) { }

                ~TypeInSizeof() { }

                void accept(IExpressionVisitor &visitor) const override {
                    visitor.visit(*this);
                }
                
                bool isTerminal() const override {
                    return true;
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
