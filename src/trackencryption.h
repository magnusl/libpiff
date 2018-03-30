#ifndef _PIFF_TRACKENCRYPTION_H_
#define _PIFF_TRACKENCRYPTION_H_

#include "fullbox.h"
#include "uuid.h"
#include <iostream>
#include <boost/uuid/uuid_io.hpp>

namespace piff {

///
/// \class  TrackEncryptionBox
///
class TrackEncryptionBox : public iso::FullBox
{
public:
    TrackEncryptionBox(const iso::Box& box) : iso::FullBox(box)
    {
        iso::Source& src = _box.data();
                
        _defaultAlgorithmID = src.read<uint32_t>(3);
        _defaultIVSize      = src.read<uint8_t>();
        _defaultKID         = uuid(src);
    }

    uint32_t defaultAlgorithmID() const {
        return _defaultAlgorithmID;
    }

    boost::uuids::uuid defaultKID() const {
        return _defaultKID;
    }

    uint8_t defaultIVSize() const {
        return _defaultIVSize;
    }       

protected:
    boost::uuids::uuid  _defaultKID;
    uint32_t            _defaultAlgorithmID;
    uint8_t             _defaultIVSize;
};

} // namespace piff

#endif
