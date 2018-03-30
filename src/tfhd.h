#ifndef _ISO_TFHD_H_
#define _ISO_TFHD_H_

#include "fullbox.h"
#include <iostream>
#include <assert.h>

namespace iso {

///
/// \class  TrackFragmentHeaderBox
///
class TrackFragmentHeaderBox : public FullBox
{
public:
    TrackFragmentHeaderBox(const Box& box) : FullBox(box)
    {
        Source& src = _box.data();
        _trackId = src.read<uint32_t>();
    
        _base_data_offset = (_flags & 0x000001) ? src.read<uint64_t>() : 0;
        _sample_description_index = (_flags & 0x000002) ? src.read<uint32_t>() : 0;
        _default_sample_duration = (_flags & 0x000008) ? src.read<uint32_t>() : 0;
        _default_sample_size = (_flags & 0x000010) ? src.read<uint32_t>() : 0;
        _default_sample_flags = (_flags & 0x000020) ? src.read<uint32_t>() : 0;
        assert(src.empty());
    }

    uint32_t trackid() const {
        return _trackId;
    }
    
    uint32_t default_sample_duration() const {
        return _default_sample_duration;
    }
    
    uint32_t default_sample_size() const {
        return _default_sample_size;
    }

    uint32_t default_sample_flags() const {
        return _default_sample_flags;
    }

protected:
    uint32_t    _base_data_offset;
    uint32_t    _trackId;
    uint32_t    _sample_description_index;
    uint32_t    _default_sample_duration;
    uint32_t    _default_sample_size;
    uint32_t    _default_sample_flags;
};

} // namespace iso

#endif
