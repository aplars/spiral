#ifndef SHADERCACHE_H
#define SHADERCACHE_H
namespace sa {
typedef lru<std::tuple<std::set<std::string>, std::string, std::string>, ShaderProgramPtr> ShaderCache;
}
#endif // SHADERCACHE_H
