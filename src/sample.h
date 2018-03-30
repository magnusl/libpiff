#ifndef _ISO_SAMPLE_H_
#define _ISO_SAMPLE_H_

#include "box.h"
#include "protectedschemeinfo.h"
#include <iostream>
#include <memory>

namespace iso {

///
/// \class  SampleEntry
///
class SampleEntry
{
public:
    SampleEntry(const Box& box) : _box(box)
    {
        Source& src = _box.data();
        src.skip(6);    // reserved
        _data_reference_index = src.read<uint16_t>();
    }

    const std::unique_ptr<ProtectedSchemeInformationBox> & SchemeInfo() const {
        return _scheme;
    }

protected:
    SampleEntry(const SampleEntry&);
    SampleEntry& operator=(const SampleEntry&);

protected:
    Box _box;
    std::unique_ptr<ProtectedSchemeInformationBox> _scheme;
    uint16_t _data_reference_index;
};

class VisualSampleEntry : public SampleEntry
{
public:
    VisualSampleEntry(const Box& box) : SampleEntry(box)
    {
        Source& src = _box.data();
        src.skip(2);     // pre_defined = 0
        src.skip(2);     // reserved = 0
        src.skip(3 * sizeof(uint32_t));// pre_defined = 0
        _width = src.read<uint16_t>();
        _height = src.read<uint16_t>();

        uint32_t horizresolution = src.read<uint32_t>();
        uint32_t vertresolution = src.read<uint32_t>();

        if ((horizresolution !=  0x00480000) || (vertresolution !=  0x00480000)) {

        }
        src.skip(4);    // reserved = 0
        uint16_t frame_count = src.read<uint16_t>();
        
        src.skip(32);   // compressor name
        src.skip(2);
        src.skip(2);

        if (_box.type() == "encv") {
            iso::ObjectSequence sequence(src);
            while(sequence.hasMore()) {
                Box inner = sequence.next();
                if (inner.type() == "sinf") {
                    _scheme = std::make_unique<ProtectedSchemeInformationBox>(inner);
                    break;
                }
            }
            if (!_scheme) {
                throw std::runtime_error(
                    "Encrypted video sample is missing a ProtectedSchemeInformationBox");
            }
        }
    }

protected:
    uint16_t _width;
    uint16_t _height;
};

class AudioSampleEntry : public SampleEntry
{
public:
    AudioSampleEntry(const Box& box) : SampleEntry(box)
    {
        Source& src = _box.data();
        src.skip(sizeof(uint32_t) * 2); // reserved = 0
        _channelCount = src.read<uint16_t>();
        _sampleSize = src.read<uint16_t>();
        src.skip(sizeof(uint16_t));     // pre_defined = 0
        src.skip(sizeof(uint16_t));     // reserved = 0
        _sampleRate = src.read<uint32_t>();

        // if this is a encrypted sample, a Protection Scheme Information Box (sinf) must follow.
        if (_box.type() == "enca") {
            iso::ObjectSequence sequence(src);
            while(sequence.hasMore()) {
                Box inner = sequence.next();
                if (inner.type() == "sinf") {
                    _scheme = std::make_unique<ProtectedSchemeInformationBox>(inner);
                    break;
                }
            }
            if (!_scheme) {
                throw std::runtime_error(
                    "Encrypted audio sample is missing a ProtectedSchemeInformationBox");
            }
        }
    }
    
    uint16_t channelCount() const {
        return _channelCount;
    }

    uint16_t sampleSize()   const {
        return _sampleSize;
    }

    uint32_t sampleRate() const {
        return _sampleRate;
    }

protected:
    AudioSampleEntry(const AudioSampleEntry&);
    AudioSampleEntry& operator=(const AudioSampleEntry&);
 
protected:
    uint32_t _sampleRate;
    uint16_t _channelCount;
    uint16_t _sampleSize;
};

} // namespace iso

#endif
