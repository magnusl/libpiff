#ifndef _ISO_MOVIEBOX_H_
#define _ISO_MOVIEBOX_H_

#include "box.h"
#include "movieheader.h"
#include "trackbox.h"
#include <memory>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "uuid.h"
#include "psshbox.h"

namespace iso {

///
/// \class  MovieBox
///
/// \brief  The metadata for a presentation is stored in the single Movie Box which occurs at the top-level of a file. 
///
class MovieBox
{
public:
    MovieBox(const Box& box) : _box(box)
    {
        ObjectSequence sequence(_box.data());
        while(sequence.hasMore()) {
            iso::Box child = sequence.next();
            const Fourcc_t type = child.type();
            if (type == "mvhd") {
                _mvhd = std::make_unique<MovieHeaderBox>(child);
            } else if (type == "trak") {
                _tracks.push_back(std::move(std::make_unique<TrackBox>(child)));
            } else if (type == "uuid") {
                // read the uuid to determine the type
                static boost::uuids::uuid pss = uuid("d08a4f18-10f3-4a82-b6c8-32d8aba183d3");
                if (pss == uuid(child.data())) {
                    _psshs.push_back(
                        std::move(std::make_unique<piff::ProtectionSystemSpecificHeaderBox<> >(child)));
                }
            }
        }
        if (!_mvhd) {
            throw std::runtime_error("Missing mandatory 'Movie Header box'");
        }
        if (_tracks.empty()) {
            throw std::runtime_error("Movie box must contain at least one Track box.");
        }
    }

    const std::vector<std::unique_ptr<TrackBox> >& tracks() const {
        return _tracks;
    }

    const MovieHeaderBox& header() const {
        return *_mvhd;
    }

private:
    Box _box;
    std::vector<std::unique_ptr<piff::ProtectionSystemSpecificHeaderBox<> > > _psshs;
    std::vector<std::unique_ptr<TrackBox> > _tracks;
    std::unique_ptr<MovieHeaderBox>         _mvhd;
};

} // namespace iso

#endif // _ISO_MOVIEBOX_H_
