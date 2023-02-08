#pragma once

#include "../CType/CTypedValue.h"

namespace c4 {
    namespace model {
        namespace expression {
            class BinaryExpression;
            class CallExpression;
            class CallArguments;
            class ConditionalExpression;
            class ConstantExpression;
            class IdentifierExpression;
            class IndexExpression;
            class MemberExpression;
            class SizeOfType;
            class TypeInSizeof;
            class UnaryExpression;

            class IExpressionCodeGenVisitor {
            public:
                virtual ~IExpressionCodeGenVisitor() { }

                virtual ctype::CTypedValue visitLValue(const BinaryExpression &expr) = 0;
                virtual ctype::CTypedValue visitLValue(const CallExpression &expr) = 0;
                virtual ctype::CTypedValue visitLValue(const ConditionalExpression &expr) = 0;
                virtual ctype::CTypedValue visitLValue(const ConstantExpression &expr) = 0;
                virtual ctype::CTypedValue visitLValue(const IdentifierExpression &expr)= 0;
                virtual ctype::CTypedValue visitLValue(const IndexExpression &expr) = 0;
                virtual ctype::CTypedValue visitLValue(const MemberExpression &expr) = 0;
                virtual ctype::CTypedValue visitLValue(const SizeOfType &expr) = 0;
                virtual ctype::CTypedValue visitLValue(const UnaryExpression &expr) = 0;

                virtual ctype::CTypedValue visitRValue(const BinaryExpression &expr) = 0;
                virtual ctype::CTypedValue visitRValue(const CallExpression &expr) = 0;
                virtual ctype::CTypedValue visitRValue(const ConditionalExpression &expr) = 0;
                virtual ctype::CTypedValue visitRValue(const ConstantExpression &expr) = 0;
                virtual ctype::CTypedValue visitRValue(const IdentifierExpression &expr) = 0;
                virtual ctype::CTypedValue visitRValue(const IndexExpression &expr) = 0;
                virtual ctype::CTypedValue visitRValue(const MemberExpression &expr) = 0;
                virtual ctype::CTypedValue visitRValue(const SizeOfType &expr) = 0;
                virtual ctype::CTypedValue visitRValue(const UnaryExpression &expr) = 0;
            };
        }
    }
}
