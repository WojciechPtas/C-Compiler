#pragma once

#include "llvm/IR/IRBuilder.h"

using namespace llvm;

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

                virtual Value* visitLValue(const BinaryExpression &expr) = 0;
                virtual Value* visitLValue(const CallArguments &expr) = 0;
                virtual Value* visitLValue(const CallExpression &expr) = 0;
                virtual Value* visitLValue(const ConditionalExpression &expr) = 0;
                virtual Value* visitLValue(const ConstantExpression &expr) = 0;
                virtual Value* visitLValue(const IdentifierExpression &expr) = 0;
                virtual Value* visitLValue(const IndexExpression &expr) = 0;
                virtual Value* visitLValue(const MemberExpression &expr) = 0;
                virtual Value* visitLValue(const TypeInSizeof &expr) = 0;
                virtual Value* visitLValue(const SizeOfType &expr) = 0;
                virtual Value* visitLValue(const UnaryExpression &expr) = 0;

                virtual Value* visitRValue(const BinaryExpression &expr) = 0;
                virtual Value* visitRValue(const CallArguments &expr) = 0;
                virtual Value* visitRValue(const CallExpression &expr) = 0;
                virtual Value* visitRValue(const ConditionalExpression &expr) = 0;
                virtual Value* visitRValue(const ConstantExpression &expr) = 0;
                virtual Value* visitRValue(const IdentifierExpression &expr) = 0;
                virtual Value* visitRValue(const IndexExpression &expr) = 0;
                virtual Value* visitRValue(const MemberExpression &expr) = 0;
                virtual Value* visitRValue(const TypeInSizeof &expr) = 0;
                virtual Value* visitRValue(const SizeOfType &expr) = 0;
                virtual Value* visitRValue(const UnaryExpression &expr) = 0;
            };
        }
    }
}
