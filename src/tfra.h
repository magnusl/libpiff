#ifndef _ISO_TFRA_H_
#define _ISO_TFRA_H_

#include "fullbox.h"
#include <iostream>
#include <vector>

namespace iso {

///
/// \class  TrackFragmentRandomAccessBox
///
class TrackFragmentRandomAccessBox :  public FullBox
{
public:
    struct Sample_t {
        uint64_t    t;
        uint64_t    moof_offset;
        uint32_t    traf_number;
        uint32_t    trun_number;
        uint32_t    sample_number;
    };

    TrackFragmentRandomAccessBox(const Box& box) :
        FullBox(box)
    {
        Source& src = _box.data();
        _id = src.read<uint32_t>();
        uint32_t lengths = src.read<uint32_t>();
        uint32_t length_size_of_traf_num = (lengths >> 4) & 0x03;
        uint32_t length_size_of_trun_num = (lengths >> 2) & 0x03;
        uint32_t length_size_of_sample_num = lengths & 0x03;

        uint32_t number_of_entry = src.read<uint32_t>();
    
        _samples.resize(number_of_entry);
        for(uint32_t i = 0; i < number_of_entry; ++i) {
            Sample_t& sample = _samples[i];
            if (_version == 1) {
                sample.t = src.read<uint64_t>();
                sample.moof_offset = src.read<uint64_t>();
            } else {
                sample.t = src.read<uint32_t>();
                sample.moof_offset = src.read<uint32_t>();
            }
            sample.traf_number = src.read<uint32_t>(length_size_of_traf_num + 1);
            sample.trun_number = src.read<uint32_t>(length_size_of_trun_num + 1);
            sample.sample_number = src.read<uint32_t>(length_size_of_sample_num + 1);
        }
    }
    
    const std::vector<Sample_t>& samples() const {
        return _samples;
    }

protected:
    std::vector<Sample_t> _samples;
    uint32_t _id;
};

#if 0
unsigned int(32) track_ID;
 const unsigned int(26) reserved = 0;
 unsigned int(2) length_size_of_traf_num;
 unsigned int(2) length_size_of_trun_num;
 unsigned int(2) length_size_of_sample_num;
 unsigned int(32) number_of_entry;
 for(i=1; i • number_of_entry; i++){
 if(version==1){
 unsigned int(64) time;
 unsigned int(64) moof_offset;
 }else{
 unsigned int(32) time;
 unsigned int(32) moof_offset;
 }
 unsigned int((length_size_of_traf_num+1) * 8) traf_number;
 unsigned int((length_size_of_trun_num+1) * 8) trun_number;
 unsigned int((length_size_of_sample_num+1) * 8) sample_number;
 } 
#endif

} // namespace iso

#endif
