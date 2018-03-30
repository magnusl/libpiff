#ifndef _ISO_MOVIEFRAGMENT_H_
#define _ISO_MOVIEFRAGMENT_H_

#include "box.h"
#include "trackfragment.h"
#include "moviefragmentheader.h"
#include <memory>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace iso {

///
/// \class  MovieFragmentBox
///
class MovieFragmentBox
{
public:
    MovieFragmentBox(const Box& box) : _box(box)
    {
        Source& src = _box.data();
        ObjectSequence sequence(src);
        while(sequence.hasMore()) {
            Box child = sequence.next();
            const Fourcc_t type = child.type();
            if (type == "mfhd") {
                _header = std::make_unique<MovieFragmentHeaderBox>(child);
            } else if (type == "traf") {
                _trackFragments.push_back(
                    std::move(std::make_unique<TrackFragmentBox>(child)));
            }
        }
        if (!_header) {
            throw std::runtime_error("Missing mandatory Movie Fragment Header box.");
        }
    }

    const std::vector<std::unique_ptr<TrackFragmentBox> >& fragments() const {
        return _trackFragments;
    }
    
    const MovieFragmentHeaderBox& header() const {
        return *_header;
    }

protected:
    std::vector<std::unique_ptr<TrackFragmentBox> > _trackFragments;
    std::unique_ptr<MovieFragmentHeaderBox> _header;
    Box _box;
};

} // namespace iso

#endif
 