#pragma once

namespace L2{

        template<typename T>
        void L2_Parse_Stack<T>::push(T item){
                instr_elements.push_back(std::move(item));
        }

        template<typename T>
        template<typename expected_T>
        std::shared_ptr<expected_T> L2_Parse_Stack<T>::downcast_pop(){
                expected_T* ptr;

                T p = pop();
                if((ptr = dynamic_cast<expected_T*>(p.get()))){
                        //p.release();
                        return std::shared_ptr<expected_T>(p, ptr);
                }
                else {
                        std::stringstream ss;
                        ss << std::string("bad downcast to ")
                           <<  typeid(expected_T).name()
                           << " from "
                           << typeid(T).name()
                           << " with ast dump: ";
                        p->dump(ss);
                        throw std::logic_error(ss.str());
                }
        }

        template<typename T>
        T L2_Parse_Stack<T>::pop(){
                if(instr_elements.empty()){
                        throw std::logic_error("tried to pop from empty instr stack!");
                }
                auto tmp = std::move(instr_elements.back());
                instr_elements.pop_back();
                return tmp;
        }

        template<typename T>
        const T& L2_Parse_Stack<T>::peek() const{
                return instr_elements.back();
        }

        template <typename T>
        void L2_Parse_Stack<T>::NUKE(){
                instr_elements.clear();
        }

}
