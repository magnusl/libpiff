#ifndef _ISO_SAMPLETABLE_H_
#define _ISO_SAMPLETABLE_H_

#include "box.h"
#include "sampledescription.h"
#include <memory>

namespace iso {

class MediaInformationBox;

///
/// \class  SampleTableBox
///
class SampleTableBox
{
public:
    SampleTableBox(Box& box, MediaInformationBox& parent) :
        _box(box),
        _parent(parent)
    {
        Source& src = _box.data();
        ObjectSequence sequence(src);
        while(sequence.hasMore()) {
            Box child = sequence.next();
            const Fourcc_t type = child.type();
            if (type == "stsd") {
                _description = std::make_unique<SampleDescriptionBox>(child, *this);
            }
        }
    }

    const std::unique_ptr<SampleDescriptionBox>& description() {
        return _description;
    }
    
    const MediaInformationBox& parent() const { return _parent; }

protected:
    Box _box;
    MediaInformationBox& _parent;
    std::unique_ptr<SampleDescriptionBox> _description;
};

} // namespace iso

#endif
