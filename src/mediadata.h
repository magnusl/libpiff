#ifndef _ISO_MEDIADATABOX_H_
#define _ISO_MEDIADATABOX_H_

#include "box.h"
#include <iostream>
#include <vector>

namespace iso {

///
/// \class  MediaDataBox
///
class MediaDataBox
{
public:
    MediaDataBox(const Box& box) : _box(box)
    {
    }

    Source& data() {
        return _box.data();
    }
protected:
    Box _box;
};

} // namespace iso

#endif // _ISO_MEDIADATABOX_H_
