#ifndef _ISO_TRUN_H_
#define _ISO_TRUN_H_

#include "fullbox.h"
#include "tfhd.h"
#include <assert.h>

namespace iso {

///
/// \class  TrackRunBox
///
class TrackRunBox : public FullBox
{
public:
    struct Sample_t {
        uint32_t    duration;
        uint32_t    size;
        uint32_t    flags;
        uint32_t    composition_time_offset;
    };

    TrackRunBox(const Box& box, const TrackFragmentHeaderBox& header) : FullBox(box)
    {
        Source& src = _box.data();
        uint32_t sample_count = src.read<uint32_t>();
        if (_flags & 0x000001) {
            int32_t data_offset = src.read<int32_t>();
        }
        uint32_t first_sample_flags = (_flags & 0x000004) ? src.read<uint32_t>() : 0;

        _samples.resize(sample_count);
        for(uint32_t i = 0; i < sample_count; ++i) {
            Sample_t& sample = _samples[i];
            sample.duration = (_flags & 0x000100) ? src.read<uint32_t>() : header.default_sample_duration();
            sample.size     = (_flags & 0x000200) ? src.read<uint32_t>() : header.default_sample_size();
            if ((_flags & 0x000004) && (i == 0)) {
                sample.flags = first_sample_flags;
            } else {
                sample.flags    = (_flags & 0x000400) ? src.read<uint32_t>() : header.default_sample_flags();
            }
            sample.composition_time_offset = (_flags & 0x000800) ? src.read<uint32_t>() : 0;
        }
        assert(src.empty());
    }

    const std::vector<Sample_t>& samples() const {
        return _samples;
    }

protected:
    std::vector<Sample_t>   _samples;
};

} // namespace iso

#endif
