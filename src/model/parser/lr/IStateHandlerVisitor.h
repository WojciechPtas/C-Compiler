#pragma once

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                class AcceptingStateHandler;
                class ReducingStateHandler;
                class ShiftingStateHandler;

                class IStateHandlerVisitor {
                public:
                    virtual void visit(
                        const AcceptingStateHandler &handler
                    ) = 0;

                    virtual void visit(
                        const ReducingStateHandler &handler
                    ) = 0;

                    virtual void visit(
                        const ShiftingStateHandler &handler
                    ) = 0;
                };
            }
        }
    }
}
