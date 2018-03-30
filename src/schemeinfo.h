#ifndef _ISO_SCHEMEINFO_H_
#define _ISO_SCHEMEINFO_H_

#include "box.h"
#include "trackencryption.h"
#include <iostream>
#include <memory>

namespace iso {

///
/// \class  SchemeInformationBox
///
class SchemeInformationBox
{
public:
    SchemeInformationBox(const Box& box) : _box(box)
    {
        Source& src = _box.data();
        ObjectSequence sequence(src);
        while(sequence.hasMore()) {
            Box child = sequence.next();
            const Fourcc_t type = child.type();
            static boost::uuids::uuid te = uuid("8974dbce-7be7-4c51-84f9-7148f9882554");
            if (te == uuid(child.data())) {
                _encryption = std::make_unique<piff::TrackEncryptionBox>(child);
            }
        }
    }

    const std::unique_ptr<piff::TrackEncryptionBox>& encryption() const {
        return _encryption;
    }

protected:
    Box _box;
    std::unique_ptr<piff::TrackEncryptionBox> _encryption;
};

} // namespace iso

#endif
