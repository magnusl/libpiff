#ifndef _ISO_TRACKBOX_H_
#define _ISO_TRACKBOX_H_

#include "box.h"
#include "trackheader.h"
#include "mediabox.h"
#include <memory>
#include <stdexcept>
#include <assert.h>

namespace iso {

///
/// \class  TrackBox
///
class TrackBox
{
public:
    TrackBox(const Box& box) : _box(box)
    {
        iso::ObjectSequence sequence(_box.data());
        while(sequence.hasMore()) {
            Box child = sequence.next();
            Fourcc_t type = child.type();
            if (type == "tkhd") {
                _tkhd = std::make_unique<TrackHeaderBox>(child);
            } else if (type == "mdia") {
                _mdia = std::make_unique<MediaBox>(child);
            }
        }
        if (!_mdia) {
            throw std::runtime_error("Missing mandatory 'Media Box' inside Track box.");
        }
        if (!_tkhd) {
            throw std::runtime_error("Track box must contain a Track Header box.");
        }
    }

    const MediaBox& media() const {
        return *_mdia;
    }
    
    const TrackHeaderBox& header() const {
        return *_tkhd;
    }

protected:
    Box _box;
    std::unique_ptr<TrackHeaderBox> _tkhd;
    std::unique_ptr<MediaBox>       _mdia;
};

} // namespace iso

#endif
