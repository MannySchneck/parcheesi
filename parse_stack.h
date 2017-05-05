#include <memory>
#include <deque>

namespace L2{
        template <typename T>
        class L2_Parse_Stack{

        public:
                void push(T item);

                template<typename expected_T>
                        std::shared_ptr<expected_T> downcast_pop();

                T pop();

                const T& peek() const;

                void NUKE();

        private:
                std::deque<T> instr_elements;
        };

}

#include "parse_stack.tpp"
