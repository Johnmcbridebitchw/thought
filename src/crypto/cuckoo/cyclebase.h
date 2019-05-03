// Copyright (c) 2013-2018 John Tromp
// Copyright (c) 2019 The Thought Core developers

#ifndef CUCKOO_CYCLEBASE_H_
#define CUCKOO_CYCLEBASE_H_

#include <vector>
#include <cstring>

#include "crypto/cuckoo/cuckoo.h"

namespace cuckoo {

struct edge
{
    uint32_t u;
    uint32_t v;
    edge() : u(0), v(0) { }
    edge(uint32_t x, uint32_t y) : u(x), v(y) { }

    inline bool operator==(edge const &other) const
    {
        return 0 == memcmp(this, &other, sizeof(edge));
    }
};

struct cyclebase
{
    static constexpr uint32_t MAXCYCLES = 64;  // single byte
    // should avoid different values of MAXPATHLEN in different threads of one process
    uint32_t maxpathlen;

    int ncycles;
    uint32_t ncuckoo;
    uint32_t nedges;
    uint32_t edgemask;
    word_t *cuckoo;
    uint32_t *pathcount;
    edge cycleedges[MAXCYCLES];
    uint32_t cyclelengths[MAXCYCLES];
    uint32_t prevcycle[MAXCYCLES];
    uint32_t *us;
    uint32_t *vs;

    siphash_keys const *keys;

    std::vector<std::vector<uint32_t>> solutions;

    void alloc(uint32_t graphSize);
    void freemem();

    void reset();

    int path(uint32_t u0, uint32_t *us) const;
    int pathjoin(uint32_t *us, int *pnu, uint32_t *vs, int *pnv);

    void addedge(uint32_t u0, uint32_t v0);

    void solution(uint32_t *us, int nu, uint32_t *vs, int nv);
    int sharedlen(uint32_t *us, int nu, uint32_t *vs, int nv);

    void cycles();
};

};

namespace std {

/// Implementation of std::hash<cuckoo::edge>.
template<> struct hash<cuckoo::edge>
{
    typedef cuckoo::edge argument_type;
    typedef uint32_t result_type;
    result_type operator()(argument_type const &e) const noexcept
    {
        return e.u ^ e.v;
    }
};

};

#endif  // CUCKOO_CYCLEBASE_H_
