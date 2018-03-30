#ifndef _ISO_MFRA_H_
#define _ISO_MFRA_H_

#include "box.h"
#include "tfra.h"
#include <iostream>
#include <vector>
#include <memory>

namespace iso {

///
/// \class  MovieFragmentRandomAccessBox
///
class MovieFragmentRandomAccessBox
{
public:
    MovieFragmentRandomAccessBox(const Box& box) : _box(box)
    {
        ObjectSequence sequence(_box.data());
        while(sequence.hasMore()) {
            iso::Box child = sequence.next();
            const Fourcc_t type = child.type();
            if (type == "tfra") {
                _tfras.push_back(std::make_unique<TrackFragmentRandomAccessBox>(child));
            }
        }
    }

    const std::vector<std::unique_ptr<TrackFragmentRandomAccessBox> >& tfras() const {
        return _tfras;
    }

protected:
    Box _box;
    std::vector<std::unique_ptr<TrackFragmentRandomAccessBox> > _tfras;
};

} // namespace iso

#endif // _ISO_MFRA_H_
