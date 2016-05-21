#ifndef SHADERCACHE_H
#define SHADERCACHE_H
namespace sa {
typedef lru<std::string, ShaderProgramPtr> ShaderCache;
}
#endif // SHADERCACHE_H
