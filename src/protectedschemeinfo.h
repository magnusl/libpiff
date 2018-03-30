#ifndef _ISO_PROTECTEDSCHEMEINFO_H_
#define _ISO_PROTECTEDSCHEMEINFO_H_

#include "box.h"
#include "schemeinfo.h"
#include <iostream>
#include <memory>

namespace iso {

class OriginalFormatBox
{
public:
    OriginalFormatBox(const Box& box) : _box(box)
    {
        Source& src = _box.data();
        src.read(&_format.fourcc, 4);
    }
   
    Fourcc_t data_format() const { return _format; }

protected:
    Box _box;
    Fourcc_t _format;
};


///
/// \class  ProtectedSchemeInformationBox
///
class ProtectedSchemeInformationBox
{
public:
    ProtectedSchemeInformationBox(const Box& box) :
        _box(box)
    {
        iso::ObjectSequence sequence(_box.data());
        while(sequence.hasMore()) {
            Box b = sequence.next();
            Fourcc_t type = b.type();
            if (type == "frma") {
                _original = std::make_unique<OriginalFormatBox>(b);
            } else if (type == "schi") {
                _scheme = std::make_unique<SchemeInformationBox>(b);
            }
        }
    }

    const std::unique_ptr<OriginalFormatBox>& original() const {
        return _original;
    }

protected:
    Box _box;
    std::unique_ptr<OriginalFormatBox> _original;
    std::unique_ptr<SchemeInformationBox> _scheme;
};

} // namespace iso

#endif
