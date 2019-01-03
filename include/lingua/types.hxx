#ifndef LINGUA_TYPES_HXX
#define LINGUA_TYPES_HXX

#include <unordered_map>
#include <string>

namespace lingua {
    typedef unsigned long tag_t;
    typedef std::unordered_map<std::string, tag_t> Dictionary;
}

#endif // LINGUA_TYPES_HXX
