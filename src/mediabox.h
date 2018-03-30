#ifndef _ISO_MEDIABOX_H_
#define _ISO_MEDIABOX_H_

#include "box.h"
#include "handlerreference.h"
#include "mediainformation.h"
#include <memory>

namespace iso {

class MediaBox
{
public:
    MediaBox(Box& box) :
        _box(box)
    {
        iso::ObjectSequence sequence(_box.data());
        while(sequence.hasMore()) {
            Box child = sequence.next();
            const Fourcc_t type = child.type();
            if (type == "mdhd") {

            } else if (type == "hdlr") {
                _hdlr = std::make_unique<HandlerReferenceBox>(child);
            } else if (type == "minf") {
                _minf = std::make_unique<MediaInformationBox>(child, *this);
            }
        }
        if (!_hdlr) {
            throw std::runtime_error("Media box must contain a Handler Reference box.");
        }
        if (!_minf) {
            throw std::runtime_error("Media box must contain a Media Information box.");
        }
    }

    const HandlerReferenceBox& handler() const {
        return *_hdlr;
    }
    
   const MediaInformationBox& information() const {
        return *_minf;     
   }

protected:
    Box _box;
    std::unique_ptr<HandlerReferenceBox> _hdlr;
    std::unique_ptr<MediaInformationBox> _minf;
};

} // namespace iso

#endif
