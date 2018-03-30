#ifndef _ISO_TRACKHEADERBOX_H_
#define _ISO_TRACKHEADERBOX_H_

#include "fullbox.h"

namespace iso {

class TrackHeaderBox : public FullBox
{
public:
    TrackHeaderBox(Box& box) : FullBox(box)
    {
        Source& src = _box.data();
        if (1 == _version) {
            _ctime      = src.read<uint64_t>();
            _mtime      = src.read<uint64_t>();
            _trackid    = src.read<uint32_t>();
            (void) src.read<uint32_t>();
            _duration   = src.read<uint64_t>();
        } else {
            _ctime      = src.read<uint32_t>();
            _mtime      = src.read<uint32_t>();
            _trackid    = src.read<uint32_t>();
            (void) src.read<uint32_t>();
            _duration   = src.read<uint32_t>();
        }
        // ignore rest for now
    }

    uint64_t ctime()        const { return _ctime; }
    uint64_t mtime()        const { return _mtime; }
    uint32_t trackid()      const { return _trackid; }
    uint64_t duration()     const { return _duration; }

protected:
    uint64_t    _ctime;
    uint64_t    _mtime;
    uint32_t    _trackid;
    uint64_t    _duration;
};

} // namespace iso

#endif // _ISO_TRACKHEADERBOX_H_
