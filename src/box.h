#ifndef _ISO_BOX_H_
#define _ISO_BOX_H_

#include <stdint.h>
#include <ostream>
#include "source.h"

namespace iso {

struct Fourcc_t {
    Fourcc_t()
    {
    }
    
    Fourcc_t(char a, char b, char c, char d) {
        fourcc[0] = a;
        fourcc[1] = b;
        fourcc[2] = c;
        fourcc[3] = d;
    }

    char fourcc[4];
};

std::ostream& operator<<(std::ostream&, const Fourcc_t&);
bool operator==(const Fourcc_t&, const char*);
bool operator!=(const Fourcc_t&, const char*);

///
/// \struct     Box
///
class Box
{
public:
    Box(Fourcc_t type, const Source&);
    Box(Source&);

    Fourcc_t type() const  { return _type; }
    Source& data()          { return _source; }

protected:
    Fourcc_t    _type;
    Source      _source;
};

///
/// \class  ObjectSequence
///
class ObjectSequence
{
public:
    //ObjectSequence(const void *, size_t);
    ObjectSequence(Source&);

    bool hasMore() const;
    Box next();

protected:
    Source& _source;
};

} // namespace iso

#endif
