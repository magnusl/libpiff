#ifndef _ISO_SAMPLEDESCRIPTION_H_
#define _ISO_SAMPLEDESCRIPTION_H_

#include "fullbox.h"
#include "sample.h"
#include <vector>
#include <memory>

namespace iso {

class SampleTableBox;

///
/// \class SampleDescriptionBox
///
class SampleDescriptionBox : public FullBox
{
public:
    SampleDescriptionBox(const Box& box, const SampleTableBox& parent);

    const std::vector<std::unique_ptr<VisualSampleEntry> >& visual_samples() const {
        return _visual_samples;
    }

    const  std::vector<std::unique_ptr<AudioSampleEntry> >& audio_samples() const {
        return _audio_samples;
    }

protected:
    const SampleTableBox& _parent;
    std::vector<std::unique_ptr<VisualSampleEntry> > _visual_samples;
    std::vector<std::unique_ptr<AudioSampleEntry> > _audio_samples;
};

} // namespace iso

#endif
