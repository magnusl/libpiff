#ifndef _ISO_AVC_H_
#define _ISO_AVC_H_

#include "box.h"
#include "sample.h"
#include "sps.h"
#include <assert.h>
#include <vector>
#include <iostream>

namespace iso {

class AVCDecoderConfigurationRecord
{
public:
    
    struct SequenceParameterSet {
        std::vector<uint8_t> sequenceParameterSetNALUnit;
    };

    struct PictureParameterSet {
        std::vector<uint8_t> pictureParameterSetNALUnit;
    };

    AVCDecoderConfigurationRecord(const Box& box) : _box(box)
    {
        Source& src = _box.data();
        _config_version = src.read<uint8_t>();
        assert(_config_version == 1);
        _profile_indication = src.read<uint8_t>();
        _profile_compatibility = src.read<uint8_t>();
        _level_indication = src.read<uint8_t>();
        
        uint8_t b = src.read<uint8_t>();
        assert((b & 0xfc) == 0xfc);     // '111111'b
        _length_size_minus_one = b & 0x03;
        b = src.read<uint8_t>();
        assert((b & 0xe0) == 0xe0);
        uint8_t numOfSequenceParameterSets = b & 0x1f;
        
        _sps.resize(numOfSequenceParameterSets);
        for(uint8_t i = 0; i < numOfSequenceParameterSets; ++i) {
            uint16_t sequenceParameterSetLength = src.read<uint16_t>();
            src.read(_sps[i].sequenceParameterSetNALUnit, sequenceParameterSetLength);
            
            // try to decode it
            iso::Source spsSource(_sps[i].sequenceParameterSetNALUnit);
            iso::avc::SequenceParameterSet sps(spsSource);
        }

        uint8_t numOfPictureParameterSets = src.read<uint8_t>();
        _pps.resize(numOfPictureParameterSets);
        for(uint8_t i = 0; i < numOfPictureParameterSets; ++i) {
            uint16_t pictureParameterSetLength = src.read<uint16_t>();
            src.read(_pps[i].pictureParameterSetNALUnit, pictureParameterSetLength);
        }
    }

protected:
    Box _box;
    uint8_t     _config_version;
    uint8_t     _profile_indication;
    uint8_t     _profile_compatibility;
    uint8_t     _level_indication;
    uint8_t     _length_size_minus_one;

    std::vector<SequenceParameterSet> _sps;
    std::vector<PictureParameterSet> _pps;
};

///
/// \class  AVCSampleEntry
///
class AVCSampleEntry : public VisualSampleEntry
{
public:
    AVCSampleEntry(const Box& box) : VisualSampleEntry(box)
    {

    }
};

}

#endif
