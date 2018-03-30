#include "source.h"
#include <stdexcept>

using namespace std;

namespace iso {

Source::Source() :
    _startp(0),
    _endp(0),
    _ptr(0)
{
    // empty
}

Source::Source(const uint8_t* data, uint64_t size) :
    _startp(data),
    _endp(data+size),
    _ptr(data)
{
    // empty
}

Source::Source(const std::vector<uint8_t>& data) :
    _startp(0),
    _endp(0),
    _ptr(0)
{
    if (!data.empty()) {
        _startp = &data[0];
        _endp = _startp + data.size();
        _ptr = _startp;
    }
}

bool Source::empty() const
{
    return (_ptr < _endp) == false;
}

size_t Source::available() const
{
    return _endp - _ptr;
}

void Source::skip(size_t count)
{
    if (available() < count) {
        throw runtime_error("attempt to read outside of array.");
    }
    _ptr += count;
}

void Source::read(void * dst, size_t size)
{
    if (available() < size) {
        throw runtime_error("attempt to read outside of array.");
    }
    memcpy(dst, _ptr, size);
    _ptr += size;
}

void Source::read(std::vector<uint8_t>& data, size_t count)
{
    if (available() < count) {
        throw runtime_error("attempt to read outside of array.");
    }
    data.resize(count);
    if (count) {
        read(&data[0], count);
    }
}

void Source::read(std::string& str)
{
    // TODO: check length
    char c = read<char>();
    while(c) {
        str += c;
        c = read<char>();
    }
}

Source Source::subset(uint64_t size)
{
    if (size > available()) {
        throw runtime_error("attempt to read outside of array.");
    }
    
    const uint8_t* data = _ptr;
    _ptr += size;

    return Source(data, size);
}

} // namespace iso
