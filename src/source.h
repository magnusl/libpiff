#ifndef _ISO_SOURCE_H_
#define _ISO_SOURCE_H_

#include <stdint.h>
#include <string>
#include <stdexcept>
#include <vector>

namespace iso {

class Source
{
public:
    Source();
    Source(const uint8_t*, uint64_t);
    Source(const std::vector<uint8_t>&);    

    bool empty() const;
    size_t available() const;
    Source subset(uint64_t);
    void read(void *, size_t);
    void skip(size_t);
    void read(std::string&);
    void read(std::vector<uint8_t>&, size_t);

    template<class T>
    T read(size_t Count = sizeof(T))
    {
        if (available() < Count) {
             throw std::runtime_error("attempt to read outside of array.");
        }
        T value = 0;
        for(size_t i = 0; i < Count; ++i) {
            value = (value << 8) | *_ptr++;
        }
        return value;
    }

    template<class T>
    T peek(size_t Count = sizeof(T))
    {
        if (available() < Count) {
             throw std::runtime_error("attempt to read outside of array.");
        }
        const uint8_t* tmp = _ptr;
        T value = 0;
        for(size_t i = 0; i < Count; ++i) {
            value = (value << 8) | *tmp++;
        }
        return value;
    }

protected:
    const uint8_t* _startp;
    const uint8_t* _endp;
    const uint8_t* _ptr;
};

} // namespace iso

#endif
