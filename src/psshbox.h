#ifndef _PIFF_PSSHBOX_H_
#define _PIFF_PSSHBOX_H_

#include "fullbox.h"
#include "uuid.h"
#include <vector>
#include <stdint.h>
#include <stdexcept>
#include <iostream>
#include <boost/uuid/uuid_io.hpp>

namespace piff {

///
/// \class  ProtectionSystemSpecificHeaderBox
///
template<size_t MaxDataSize = 65536>
class ProtectionSystemSpecificHeaderBox : public iso::FullBox
{
public:
    ProtectionSystemSpecificHeaderBox(const iso::Box& box) :
        iso::FullBox(box)
    {
        iso::Source& src = _box.data();
        _uuid = iso::uuid(src); // SystemID
    
        uint32_t dataSize = src.read<uint32_t>();
        if (dataSize > MaxDataSize) {
            throw std::runtime_error("System specific data exceeds the max allowed size.");
        }
        _data.resize(dataSize);
        if (dataSize) {
            src.read(&_data[0], dataSize);
        }
    }

    boost::uuids::uuid uuid() const { return _uuid; }

protected:
    std::vector<uint8_t>    _data;
    boost::uuids::uuid      _uuid;
};

} // namespace piff

#endif
