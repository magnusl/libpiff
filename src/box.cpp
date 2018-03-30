#include "box.h"
#include <stdexcept>

using namespace std;

namespace iso {

Box::Box(Fourcc_t bt, const Source& source) :
    _type(bt),
    _source(source)
{
}

#if 0
ObjectSequence::ObjectSequence(const void* ptr, size_t size) :
    _source(reinterpret_cast<const uint8_t*>(ptr), size) 
{
    // empty
}
#endif

ObjectSequence::ObjectSequence(Source& source) :
    _source(source)
{
}

bool ObjectSequence::hasMore() const
{
    return (_source.empty() == false);
}

Box::Box(Source& src)
{
    uint64_t size       = src.read<uint32_t>();
    Fourcc_t bt;

    src.read(bt.fourcc, 4);
    size_t fieldSize    = 2 * sizeof(uint32_t);

    if (1 == size) {
        size = src.read<uint64_t>();
        fieldSize += sizeof(uint64_t);
    }
    if (size < fieldSize) {
        throw runtime_error("Invalid box size.");
    }
    
    _type = bt;
    _source = src.subset(size - fieldSize);
}

Box ObjectSequence::next()
{
    uint64_t size       = _source.read<uint32_t>();
    Fourcc_t bt;

    _source.read(bt.fourcc, 4);
    size_t fieldSize    = 2 * sizeof(uint32_t);

    if (1 == size) {
        size = _source.read<uint64_t>();
        fieldSize += sizeof(uint64_t);
    }
    if (size < fieldSize) {
        throw runtime_error("Invalid box size.");
    }
    return Box(bt, _source.subset(size - fieldSize));
}

std::ostream& operator<<(std::ostream& os, const Fourcc_t& fourcc)
{
    os << fourcc.fourcc[0] << fourcc.fourcc[1] << fourcc.fourcc[2] << fourcc.fourcc[3];
    return os;
}

bool operator==(const Fourcc_t& type, const char* name)
{
    return strncmp(type.fourcc, name, 4) == 0;
}

bool operator!=(const Fourcc_t& type, const char* name)
{
    return strncmp(type.fourcc, name, 4) != 0;
}

} // namespace iso
