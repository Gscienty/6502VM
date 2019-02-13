#ifndef _6502VM_VM_H
#define _6502VM_VM_H

#include "machine_code.hpp"

namespace vm6502
{

class vm
{
private:
    reg r;
    memory mem;
    instruction ins;
public:
    vm();
};

}

#endif
