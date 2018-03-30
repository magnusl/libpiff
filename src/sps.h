#ifndef _ISO_SPS_H_
#define _ISO_SPS_H_

#include "source.h"
#include <assert.h>

namespace iso {

namespace avc {

class BitSource
{
public:
    BitSource(Source& source) :
        _source(source),
        _cnt(0)
    {

    }
    
    uint8_t bit()
    {
        if (!_cnt) {
            _byte = _source.read<uint8_t>();
            _cnt = 8;
        }
        --_cnt;
        return (_byte >> _cnt) & 0x01;
    }

    uint64_t bits(size_t count)
    {
        uint64_t value = 0;
        for(size_t i = 0; i < count; ++i) {
            value = (value << 1) | bit();
        }
        return value;
    }
    
    template<class T>
    T read()
    {
        size_t count = sizeof(T) * 8;
        T value = 0;
        for(size_t i = 0; i < count; ++i) {
            value = (value << 1) | bit();
        }
        return value;
    }

protected:
    Source& _source;
    uint8_t _byte;
    uint8_t _cnt;
};

uint64_t ExpGolombCoded(BitSource& src)
{
    int leadingZeroBits = -1;
    for(size_t b = 0; !b; leadingZeroBits++) {
        b = src.bit();
    }
    if (leadingZeroBits < 1) {
        return 0;
    }
    return (2 << leadingZeroBits) - 1 + src.bits(leadingZeroBits);
}

///
/// \class  SequenceParameterSet
///
class SequenceParameterSet
{
public:
    SequenceParameterSet(Source& src)
    {
        BitSource bs(src);
        
        _profile_idc                = bs.bits(8);
        _constraint_set0_flag       = bs.bit();
        _constraint_set1_flag       = bs.bit();
        _constraint_set2_flag       = bs.bit();
        uint8_t reserved_zero_5bits = bs.bits(5);
        assert(reserved_zero_5bits == 0);
        _level_idc                  = bs.bits(8);
        _seq_parameter_set_id       = ExpGolombCoded(bs);
        _log2_max_frame_num_minus4  = ExpGolombCoded(bs);
        _pic_order_cnt_type         = ExpGolombCoded(bs);
        
        if (_pic_order_cnt_type == 0) {
            _log2_max_pic_order_cnt_lsb_minus4 = ExpGolombCoded(bs);
        } else if (_pic_order_cnt_type == 1) {
            uint8_t delta_pic_order_always_zero_flag = bs.bit();
            assert(delta_pic_order_always_zero_flag == 0);
            uint32_t offset_for_non_ref_pic = ExpGolombCoded(bs);           // should be signed
            uint32_t offset_for_top_to_bottom_field = ExpGolombCoded(bs);   // should be signed
            uint32_t num_ref_frames_in_pic_order_cnt_cycle = ExpGolombCoded(bs);
            for(uint32_t i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; ++i) {
                ExpGolombCoded(bs); // offset_for_ref_frame, signed
            }
        }
        _num_ref_frames = ExpGolombCoded(bs);
        _gaps_in_frame_num_value_allowed_flag = bs.bit();
        _pic_width_in_mbs_minus1 = ExpGolombCoded(bs);
        _pic_height_in_map_units_minus1 = ExpGolombCoded(bs);
        _frame_mbs_only_flag = bs.bit();
        if (!_frame_mbs_only_flag) {
            _mb_adaptive_frame_field_flag = bs.bit();
        }
        _direct_8x8_inference_flag = bs.bit();
        _frame_cropping_flag = bs.bit();

        if (_frame_cropping_flag) {
            _frame_crop_left_offset = ExpGolombCoded(bs);
            _frame_crop_right_offset= ExpGolombCoded(bs);
            _frame_crop_top_offset = ExpGolombCoded(bs);
            _frame_crop_bottom_offset = ExpGolombCoded(bs);
        }
        _vui_parameters_present_flag = bs.bit();
        if (_vui_parameters_present_flag) {
            assert(false);
        }
    }

private:
    uint8_t     _profile_idc;
    uint8_t     _level_idc;
    uint8_t     _constraint_set0_flag : 1;
    uint8_t     _constraint_set1_flag : 1;
    uint8_t     _constraint_set2_flag : 1;
    uint8_t     _gaps_in_frame_num_value_allowed_flag : 1;
    uint8_t     _frame_mbs_only_flag  : 1;
    uint8_t     _mb_adaptive_frame_field_flag : 1;
    uint8_t     _direct_8x8_inference_flag : 1;
    uint8_t     _frame_cropping_flag : 1;
    uint8_t     _vui_parameters_present_flag : 1;

    uint32_t    _seq_parameter_set_id;
    uint32_t    _log2_max_frame_num_minus4;
    uint32_t    _pic_order_cnt_type;
    uint32_t    _log2_max_pic_order_cnt_lsb_minus4;
    uint32_t    _num_ref_frames;
    uint32_t    _pic_width_in_mbs_minus1;
    uint32_t    _pic_height_in_map_units_minus1;

    // frame cropping
    uint32_t _frame_crop_left_offset;
    uint32_t _frame_crop_right_offset;
    uint32_t _frame_crop_top_offset;
    uint32_t _frame_crop_bottom_offset;
};

} // namespace avc

} // namespace iso

#endif
