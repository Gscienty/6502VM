#include "vm.hpp"

namespace vm6502
{

vm::vm() 
    : mem(this->r)
    , ins(this->mem.get_stack(), this->r)
{

}

}
