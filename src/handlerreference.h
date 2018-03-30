#ifndef _ISO_HANDLERREFERENCE_H_
#define _ISO_HANDLERREFERENCE_H_

#include "fullbox.h"
#include <iostream>

namespace iso {

///
/// \class  HandlerReferenceBox
///
class HandlerReferenceBox : public FullBox
{
public:
    HandlerReferenceBox(Box box) : FullBox(box)
    {
        Source& src = _box.data();
        src.skip(sizeof(uint32_t));     // pre_defined
        src.read(_type.fourcc, 4);       // handler_type
        src.skip(sizeof(uint32_t) * 3); // reserved
        src.read(_name); 
    }

    Fourcc_t    _type;
    std::string _name;
};

} // namespace iso

#endif
