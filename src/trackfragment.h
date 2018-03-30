#ifndef _ISO_TRACKFRAGMENT_H_
#define _ISO_TRACKFRAGMENT_H_

#include "box.h"
#include "uuid.h"
#include "sampleencryptionbox.h"
#include "trun.h"
#include "tfhd.h"
#include <iostream>
#include <vector>

namespace iso {

///
/// \class  TrackFragmentBox
///
class TrackFragmentBox
{
public:
    TrackFragmentBox(const Box& box) : _box(box)
    {
        Source& src = _box.data();
        ObjectSequence sequence(src);
        while(sequence.hasMore()) {
            iso::Box child = sequence.next();
            const Fourcc_t type = child.type();
            if (type == "tfhd") {
                _header = std::make_unique<TrackFragmentHeaderBox>(child);
            } else if (type == "uuid") {
                static boost::uuids::uuid pss = uuid("A2394F52-5A9B-4f14-A244-6C427C648DF4");
                if (pss == uuid(child.data())) {
                   _encryption = std::make_unique<piff::SampleEncryptionBox>(child);
                }
            } else if (type == "trun") {
                if (!_header) {
                    throw std::runtime_error("Track fragment header must be located before Track run boxes.");
                }
                _trackRuns.push_back(std::make_unique<TrackRunBox>(child, header()));
            }
        }
        if (!_header) {
            throw std::runtime_error("Track fragment box must contain a Track fragment header.");            
        }
    }

    const TrackFragmentHeaderBox& header() const {
        return *_header;
    }

    const std::unique_ptr<piff::SampleEncryptionBox>& encryption() const {
        return _encryption;
    }

    const std::vector<std::unique_ptr<TrackRunBox> >& trackRuns() const {
        return _trackRuns;
    }

protected:
    Box _box;
    std::unique_ptr<piff::SampleEncryptionBox> _encryption;
    std::unique_ptr<TrackFragmentHeaderBox> _header;
    std::vector<std::unique_ptr<TrackRunBox> > _trackRuns;
};

} // namespace iso

#endif
