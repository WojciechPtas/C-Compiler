#include <string>
#include "ConstantExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class StringLiteralExpression :
                public ConstantExpression<std::string> {
            public:
                StringLiteralExpression(const std::string &value);
                virtual ~StringLiteralExpression() { }
            };
        }
    }
}
