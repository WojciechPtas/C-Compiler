#include <string>
#include "ConstantExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class DecimalConstantExpression :
                public ConstantExpression<std::string> {
            public:
                DecimalConstantExpression(const std::string &value);
                virtual ~DecimalConstantExpression() { }
            };
        }
    }
}
