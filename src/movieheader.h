#ifndef _ISO_MOVIEHEADERBOX_H_
#define _ISO_MOVIEHEADERBOX_H_

#include "fullbox.h"

namespace iso {

///
/// \class  MovieHeaderBox
///
/// \brief "This box defines overall information which is media-independent, 
///         and relevant to the entire presentation considered as a whole."
///
class MovieHeaderBox : public FullBox
{
public:
    MovieHeaderBox(Box& box) : FullBox(box)
    {
        Source& src = _box.data();
        if (1 == _version) {
            _ctime      = src.read<uint64_t>();
            _mtime      = src.read<uint64_t>();
            _timescale  = src.read<uint32_t>();
            _duration   = src.read<uint64_t>();
        } else {
            _ctime      = src.read<uint32_t>();
            _mtime      = src.read<uint32_t>();
            _timescale  = src.read<uint32_t>();
            _duration   = src.read<uint32_t>();
        }
        // ignore rest for now
    }

    uint64_t ctime()        const { return _ctime; }
    uint64_t mtime()        const { return _mtime; }
    uint32_t timescale()    const { return _timescale; }
    uint64_t duration()     const { return _duration; }

protected:
    uint64_t    _ctime;
    uint64_t    _mtime;
    uint32_t    _timescale;
    uint64_t    _duration;
};

} // namespace iso

#endif // _ISO_MOVIEHEADERBOX_H_
