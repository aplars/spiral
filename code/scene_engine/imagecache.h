#ifndef IMAGECACHE_H
#define IMAGECACHE_H
#include <adt/lru.h>
namespace sa {
typedef lru<std::string, Image> ImageCache;
}
#endif // IMAGECACHE_H
