/*
 * unorderMapUtil.h
 *
 *  Created on: 2019年2月20日
 *      Author: liwei
 */

#ifndef UNORDERMAPUTIL_H_
#define UNORDERMAPUTIL_H_
#include <unordered_map>
#include <stdint.h>
#include <string.h>
static inline uint32_t _hash(const char* s)
{
    uint32_t hash = 1315423911;
    while (*s)
    {
        hash ^= ((hash << 5) + (*s++) + (hash >> 2));
    }
    return (hash & 0x7FFFFFFF);
}
class StrHash
{
public:
    inline uint32_t operator()(const char * s) const
    {
        return _hash(s);
    }
    inline uint32_t operator()(const std::string & s) const
    {
        return _hash(s.c_str());
    }
};
class StrCompare
{
public:
    inline bool operator()(const char* s,const char* d) const
    {
        return strcmp(s,d)==0;
    }
    inline bool operator()(const std::string & s,const std::string & d) const
    {
        return s.size()==d.size()&&memcmp(s.c_str(),d.c_str(),s.size())==0;
    }
};





#endif /* UNORDERMAPUTIL_H_ */
