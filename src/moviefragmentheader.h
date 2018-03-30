#ifndef _ISO_MOVIEFRAGMENTHEADER_H_
#define _ISO_MOVIEFRAGMENTHEADER_H_

#include "fullbox.h"
#include <assert.h>

namespace iso {

class MovieFragmentHeaderBox : public FullBox
{
public:
    MovieFragmentHeaderBox(const Box& box) : FullBox(box)
    {
        Source& src = _box.data();
        _sequenceNumber = src.read<uint32_t>();
        assert(src.empty());
    }

    uint32_t sequenceNumber() const {
        return _sequenceNumber;
    }

protected:
    uint32_t _sequenceNumber;
};

} // namespace iso

#endif
