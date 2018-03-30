#include "sampledescription.h"
#include "sampletable.h"
#include "mediainformation.h"
#include "mediabox.h"
#include "sample.h"
#include <stdexcept>
#include <iostream>

namespace iso {

SampleDescriptionBox::SampleDescriptionBox(const Box& box, const SampleTableBox& parent) :
    FullBox(box),
    _parent(parent)
{   
    Source& src = _box.data();
    uint32_t entry_count = src.read<uint32_t>();

    const MediaInformationBox& minf = _parent.parent();
    const MediaBox& mdia = minf.parent();
    const HandlerReferenceBox& hdlr = mdia.handler();

    iso::ObjectSequence sequence(_box.data());
    for(uint32_t i = 0; i < entry_count; ++i) {
        Box sample = sequence.next();
        Fourcc_t type = sample.type();
        if (type == "encv") {
            if (_audio_samples.empty()) {
                _visual_samples.push_back(std::make_unique<VisualSampleEntry>(sample));
            } else {
                throw std::runtime_error("The sample description box MUST NOT contain entries of more than one type");
            }
        } else if (type == "enca") {    
            if (_visual_samples.empty()) {
                _audio_samples.push_back(std::make_unique<AudioSampleEntry>(sample));
            } else {
                throw std::runtime_error("The sample description box MUST NOT contain entries of more than one type");
            }
        }
    }
}

} // namespace iso
