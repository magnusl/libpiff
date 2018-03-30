#ifndef _ISO_UUID_H_
#define _ISO_UUID_H_

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>
#include "source.h"

namespace iso {

inline boost::uuids::uuid uuid(const char * str)
{
    boost::uuids::string_generator gen;
    return gen(str);
}

inline boost::uuids::uuid uuid(iso::Source& src)
{
    boost::uuids::uuid u;
    src.read(&u, 16);
    return u;
}

} // namespace iso

#endif