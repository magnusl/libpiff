#ifndef _ISO_DATAINFORMATION_H_
#define _ISO_DATAINFORMATION_H_

#include "box.h"
#include "fullbox.h"
#include <iostream>

namespace iso {

class DataEntryUrlBox : public FullBox
{
public:
    DataEntryUrlBox(const Box& box) : FullBox(box)
    {
        Source& src = _box.data();
        
        if (false == selfcontained()) {
            src.read(_location);
        }
    }

    bool selfcontained() const {
        return _flags & 0x01;
    }

    const std::string& location() const {
        return _location;
    }

protected:
    std::string _location;
};

///
/// \class  DataReferenceBox
///
/// PIFF Constraint:
///     The data reference box MUST contain a single entry with the self-contained flag set 
///
class DataReferenceBox : public FullBox
{
public:
    DataReferenceBox(const Box& box) : FullBox(box)
    {
        Source& src = _box.data();
        uint32_t entry_count = src.read<uint32_t>();
        if (entry_count != 1) {
            throw std::runtime_error(
                "A data reference must contain a single entry with the self-contained flag set.");
        }
        
        Box entry(src);
        const Fourcc_t type = entry.type();
        if (type == "url ") {
            DataEntryUrlBox url(entry);
            if (!url.selfcontained()) {
                throw std::runtime_error(
                    "PIFF requires that the data reference entry is self contained.");
            }
        }
    }
};

///
/// \class  DataInformationBox
///
class DataInformationBox
{
public:
    DataInformationBox(const Box& box) : _box(box)
    {
        ObjectSequence sequence(_box.data());
        while(sequence.hasMore()) {
            iso::Box child = sequence.next();
            const Fourcc_t type = child.type();
            if (type == "dref") {
                DataReferenceBox dref(child);
            } else {
                // handle "url " and "urn "?
            }
        }
    }

protected:
    Box _box;
};

} // namespace iso

#endif
