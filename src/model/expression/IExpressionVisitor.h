#pragma once

namespace c4 {
    namespace model {
        namespace expression {
            class BinaryExpression;
            class CallExpression;
            class ConditionalExpression;
            class ConstantExpression;
            class IdentifierExpression;
            class IndexExpression;
            class MemberExpression;
            class UnaryExpression;

            class IExpressionVisitor {
            public:
                virtual ~IExpressionVisitor() { }

                virtual void visit(const BinaryExpression &expr) = 0;
                virtual void visit(const CallArguments &expr) = 0;
                virtual void visit(const CallExpression &expr) = 0;
                virtual void visit(const ConditionalExpression &expr) = 0;
                virtual void visit(const ConstantExpression &expr) = 0;
                virtual void visit(const IdentifierExpression &expr) = 0;
                virtual void visit(const IndexExpression &expr) = 0;
                virtual void visit(const MemberExpression &expr) = 0;
                virtual void visit(const UnaryExpression &expr) = 0;
            };
        }
    }
}
