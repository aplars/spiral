#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H
namespace sa {
typedef lru<std::string, TexturePtr> TextureCache;
}
#endif // TEXTURECACHE_H
