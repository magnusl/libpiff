#ifndef _PIFF_SAMPLEENCRYPTIONBOX_H_
#define _PIFF_SAMPLEENCRYPTIONBOX_H_

#include "fullbox.h"
#include <iostream>
#include <assert.h>

namespace piff {

///
/// \class  SampleEncryptionBox
///
class SampleEncryptionBox : public iso::FullBox
{
public:
    struct Sample_t {
        uint8_t iv[16];

        struct SubSample_t {
            uint16_t    BytesOfClearData;
            uint32_t    BytesOfEncryptedData;
        };

        std::vector<SubSample_t> subsamples;
    };

    SampleEncryptionBox(const iso::Box& box) : iso::FullBox(box)
    {
        iso::Source& src = _box.data();

        if (_flags& 0x01) {
            assert(false); 
        }

        uint32_t count      = src.read<uint32_t>();
        if (count) {
            _samples.resize(count);
            
            uint32_t IV_size;
            if (_flags& 0x02) {
                IV_size = 8;
            } else {
                IV_size    = src.available() / count;
            }

            switch(IV_size) {
                case 8:
                    break;
                case 16:
                    break;
                default:    throw std::runtime_error("Invalid IV size.");
            }
            for(uint32_t i = 0; i < count; ++i) {
                memset(_samples[i].iv, 0, sizeof(_samples[i].iv));
                src.read(_samples[i].iv, IV_size);
                if (_flags & 0x02) {
                    uint16_t NumberOfEntries = src.read<uint16_t>();
                    _samples[i].subsamples.resize(NumberOfEntries);
                    for(size_t entry = 0; entry < NumberOfEntries; ++entry) {
                        _samples[i].subsamples[entry].BytesOfClearData = src.read<uint16_t>();
                        _samples[i].subsamples[entry].BytesOfEncryptedData = src.read<uint32_t>();
                    }
                }
            }
        }
    }

    const std::vector<Sample_t>& samples() const {
        return _samples;
    }

protected:
    std::vector<Sample_t> _samples;
};

} // namespace piff

#endif
