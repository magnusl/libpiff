#ifndef _ISO_FULLBOX_H_
#define _ISO_FULLBOX_H_

#include "box.h"

namespace iso {

///
/// \class  FullBox
///
class FullBox
{
public:
    FullBox(const Box& box) : _box(box)
    {
        Source& src = _box.data();
        _version    = src.read<uint8_t>();
        _flags      = src.read<uint32_t>(3 /* 24 bits */);
    }

    uint32_t flags() const  { return _flags; }
    uint8_t version() const { return _version; }

protected:
    Box         _box;
    uint32_t    _flags;
    uint8_t     _version;
};

} // namespace iso

#endif