#pragma once

#include <ostream>
#include <stdexcept>
#include "../../model/expression/IExpressionVisitor.h"
#include "../../model/ConstantType.h"
#include "../../model/expression/BinaryExpression.h"
#include "../../model/expression/ConditionalExpression.h"
#include "../../model/expression/ConstantExpression.h"
#include "../../model/expression/IdentifierExpression.h"
#include "../../model/expression/IndexExpression.h"
#include "../../model/expression/MemberExpression.h"
#include "../../model/expression/UnaryExpression.h"
#include "../../model/expression/CallExpression.h"
#include "../../model/expression/SizeOfType.h"
#include "BinaryExpressionUtilities.h"
#include "MemberExpressionUtilities.h"
#include "UnaryExpressionUtilities.h"
#include "../token/KeywordUtilities.h"
#include "../../model/expression/CallExpression.h"

//perhaps useless now
#include "../token/PrintVisitor.h"
#include <iostream>

namespace c4 {
    namespace util {
        namespace expression {
            class PrintVisitor : public model::expression::IExpressionVisitor {
            public:
                PrintVisitor(std::ostream &outputStream);
                ~PrintVisitor() { }

                void visit(
                    const model::expression::BinaryExpression &expr
                ) override;

                void visit(
                    const model::expression::CallArguments &expr
                ) override;

                void visit(
                    const model::expression::CallExpression &expr
                ) override;

                void visit(
                    const model::expression::ConditionalExpression &expr
                ) override;

                void visit(
                    const model::expression::ConstantExpression &expr
                ) override;

                void visit(
                    const model::expression::IdentifierExpression &expr
                ) override;

                void visit(
                    const model::expression::IndexExpression &expr
                ) override;

                void visit(
                    const model::expression::MemberExpression &expr
                ) override;

                void visit(
                    const model::expression::SizeOfType &expr
                ) override;

                void visit(
                    const model::expression::UnaryExpression &expr
                ) override;

            private:
                std::ostream &outputStream;
            };
        }
    }
}
