#ifndef _ISO_MEDIAINFORMATION_H_
#define _ISO_MEDIAINFORMATION_H_

#include "box.h"
#include "sampletable.h"
#include "datainformation.h"
#include <memory>
#include <stdexcept>
#include <assert.h>

namespace iso {

class MediaBox;

///
/// \class  MediaInformationBox
///
class MediaInformationBox
{
public:
    MediaInformationBox(Box box, MediaBox& parent) :
        _box(box),
        _parent(parent)
    {
        iso::ObjectSequence sequence(_box.data());
        while(sequence.hasMore()) {
            iso::Box child = sequence.next();
            const Fourcc_t type = child.type();
            if (type == "stbl") {
                _stbl = std::make_unique<SampleTableBox>(child, *this);
            } else if (type == "dinf") {
                _dinf = std::make_unique<DataInformationBox>(child);
            }
        }
        if (!_stbl) {
            throw std::runtime_error("Missing mandatory 'Sample Table box' inside Media Information box.");
        }
        if (!_dinf) {
            throw std::runtime_error("Media box must contain a Data Information box.");
        }
    }
    
    const SampleTableBox& stbl() const {
        // this is a mandatory box, so _stbl cannot be invalid
        return *_stbl;
    }

    const DataInformationBox& data() const {
        return *_dinf;
    }

    const MediaBox& parent() const { return _parent; }

private:
    MediaBox&                           _parent;
    std::unique_ptr<SampleTableBox>     _stbl;
    std::unique_ptr<DataInformationBox> _dinf;
    Box                                 _box;
};

} // namespace iso

#endif
