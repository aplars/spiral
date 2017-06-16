#pragma once
#include <adt/lru.h>

namespace sa {
typedef lru<std::string, Image> ImageCache;
}
