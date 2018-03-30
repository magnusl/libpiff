#ifndef _ISO_FILETYPEBOX_H_
#define _ISO_FILETYPEBOX_H_

#include "box.h"
#include <stdint.h>

namespace iso {

///
/// \class  FileTypeBox
///
/// \brief  "This box identifies the specifications to which this file complies"
///
class FileTypeBox
{
public:
    FileTypeBox(Box& box) : _box(box)
    {
        Source& src     = box.data();
        src.read(_major_brand.fourcc, 4);
        _minor_version  = src.read<uint32_t>();

        if (_major_brand != "piff") {
            while(!src.empty()) {
                Fourcc_t compatible_brand;
                src.read(compatible_brand.fourcc, 4);
                if (compatible_brand == "piff") {
                    return;
                }
            }
            throw std::runtime_error("Not a PIFF compatible file.");
        }
    }

    Fourcc_t major_brand() const    { return _major_brand; }
    uint32_t minor_version() const  { return _minor_version; }

private:
    Box         _box;
    Fourcc_t    _major_brand;
    uint32_t    _minor_version; 
};

} // namespace iso

#endif
