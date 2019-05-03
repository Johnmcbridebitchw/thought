// Copyright (c) 2013-2018 John Tromp
// Copyright (c) 2019 The Thought Core developers

#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <unordered_set>

#include "crypto/cuckoo/cyclebase.h"

namespace cuckoo {

void cyclebase::alloc(uint32_t graphSize)
{
    ncuckoo = ((uint32_t)1 << graphSize);
    nedges = ((uint32_t)1 << (graphSize - 1));
    edgemask =  ((uint32_t)nedges - 1);

    maxpathlen = 16 << (graphSize/3);
    cuckoo = (word_t *)calloc(ncuckoo, sizeof(word_t));
    pathcount = (uint32_t *)calloc(ncuckoo, sizeof(uint32_t));

    us = new uint32_t[maxpathlen];
    vs = new uint32_t[maxpathlen];
}

void cyclebase::freemem()  // not a destructor, as memory may have been allocated elsewhere, bypassing alloc()
{
    free(cuckoo);
    free(pathcount);

    delete[] us;
    delete[] vs;
}

void cyclebase::reset()
{
    solutions.clear();
    memset(cuckoo, -1, ncuckoo * sizeof(word_t)); // for prevcycle nil
    memset(pathcount, 0, ncuckoo * sizeof(uint32_t));
    memset(us, 0, maxpathlen * sizeof(uint32_t));
    memset(vs, 0, maxpathlen * sizeof(uint32_t));
    ncycles = 0;
}

int cyclebase::path(uint32_t u0, uint32_t *us) const
{
    int nu;
    for (uint32_t u = us[nu = 0] = u0; pathcount[u]; ) {
        pathcount[u]++;
        u = cuckoo[u];
        if (++nu >= maxpathlen) {
            while (nu-- && us[nu] != u) ;
            if (nu < 0)
                printf("maximum path length exceeded\n");
            else printf("illegal % 4d-cycle from node %d\n", maxpathlen-nu, u0);
            exit(0);
        }
        us[nu] = u;
    }
    return nu;
}

int cyclebase::pathjoin(uint32_t *us, int *pnu, uint32_t *vs, int *pnv)
{
    int nu = *pnu, nv = *pnv;
    int min = nu < nv ? nu : nv;
    for (nu -= min, nv -= min; us[nu] != vs[nv]; nu++, nv++) min--;
    *pnu = nu; *pnv = nv;
    return min;
}

void cyclebase::addedge(uint32_t u0, uint32_t v0)
{
    uint32_t u = u0 << 1, v = (v0 << 1) | 1;
    int nu = path(u, us), nv = path(v, vs);
    if (us[nu] == vs[nv]) {
        pathjoin(us, &nu, vs, &nv);
        int len = nu + nv + 1;
        printf("% 4d-cycle found\n", len);
        cycleedges[ncycles].u = u;
        cycleedges[ncycles].v = v;
        cyclelengths[ncycles++] = len;
        if (len == PROOFSIZE)
            solution(us, nu, vs, nv);
        assert(ncycles < MAXCYCLES);
    } else if (nu < nv) {
        pathcount[us[nu]]++;
        while (nu--)
            cuckoo[us[nu+1]] = us[nu];
        cuckoo[u] = v;
    } else {
        pathcount[vs[nv]]++;
        while (nv--)
            cuckoo[vs[nv+1]] = vs[nv];
        cuckoo[v] = u;
    }
}



void cyclebase::solution(uint32_t *us, int nu, uint32_t *vs, int nv)
{
    solutions.emplace_back();
    auto &proof = solutions.back();
    proof.reserve(PROOFSIZE);

    std::unordered_set<edge> cycle;
    cycle.emplace(us[0], vs[0] - nedges);

    while (nu--)
        cycle.emplace(us[(nu + 1) & ~1], us[nu | 1] - nedges);
        // recordedge(ni++, us[(nu+1)&~1], us[nu|1]); // u's in even position; v's in odd
    while (nv--)
        cycle.emplace(vs[nv | 1], vs[(nv + 1) & ~1] - nedges);
        // recordedge(ni++, vs[nv|1], vs[(nv+1)&~1]); // u's in odd position; v's in even

    for (uint32_t nonce = 0; nonce < nedges; nonce++) {
        edge e(2*sipnode(keys, nonce, 0, edgemask), 2*sipnode(keys, nonce, 1, edgemask)+1);
        if (cycle.find(e) != cycle.end()) {
            proof.emplace_back(nonce);
            cycle.erase(e);
        }
    }

    printf("solution: %lu nonces\n", proof.size());
}

void cyclebase::cycles()
{
    int len, len2;
    auto us2 = new word_t[maxpathlen];
    auto vs2 = new word_t[maxpathlen];
    for (int i=0; i < ncycles; i++) {
        word_t u = cycleedges[i].u, v = cycleedges[i].v;
        int   nu = path(u, us),    nv = path(v, vs);
        word_t root = us[nu]; assert(root == vs[nv]);
        int i2 = prevcycle[i] = cuckoo[root];
        cuckoo[root] = i;
        if (i2 < 0) continue;
        int rootdist = pathjoin(us, &nu, vs, &nv);
        do  {
            printf("chord found at cycleids %d %d\n", i2, i);
            word_t u2 = cycleedges[i2].u, v2 = cycleedges[i2].v;
            int nu2 = path(u2, us2), nv2 = path(v2, vs2);
            word_t root2 = us2[nu2]; assert(root2 == vs2[nv2] && root == root2);
            int rootdist2 = pathjoin(us2, &nu2, vs2, &nv2);
            if (us[nu] == us2[nu2]) {
                len  = sharedlen(us,nu,us2,nu2) + sharedlen(us,nu,vs2,nv2);
                len2 = sharedlen(vs,nv,us2,nu2) + sharedlen(vs,nv,vs2,nv2);
                if (len + len2 > 0) {
#if 0
                    word_t ubranch = us[nu-len], vbranch = vs[nv-len2];
                    addpath(ubranch, vbranch, len+len2);
                    addpath(ubranch, vbranch, len+len2);
#endif
                    printf(" % 4d-cycle found At %d%%\n", cyclelengths[i] + cyclelengths[i2] - 2*(len+len2), (int)(i*100L/ncycles));
                }
            } else {
                int rd = rootdist - rootdist2;
                if (rd < 0) {
                    if (nu+rd > 0 && us2[nu2] == us[nu+rd]) {
                        int len = sharedlen(us,nu+rd,us2,nu2) + sharedlen(us,nu+rd,vs2,nv2);
                        if (len) printf(" % 4d-cycle found At %d%%\n", cyclelengths[i] + cyclelengths[i2] - 2*len, (int)(i*100L/ncycles));
                    } else if (nv+rd > 0 && vs2[nv2] == vs[nv+rd]) {
                        int len = sharedlen(vs,nv+rd,us2,nu2) + sharedlen(vs,nv+rd,vs2,nv2);
                        if (len) printf(" % 4d-cycle found At %d%%\n", cyclelengths[i] + cyclelengths[i2] - 2*len, (int)(i*100L/ncycles));
                    }
                } else if (rd > 0) {
                    if (nu2-rd > 0 && us[nu] == us2[nu2-rd]) {
                        int len = sharedlen(us2,nu2-rd,us,nu) + sharedlen(us2,nu2-rd,vs,nv);
                        if (len) printf(" % 4d-cycle found At %d%%\n", cyclelengths[i] + cyclelengths[i2] - 2*len, (int)(i*100L/ncycles));
                    } else if (nv2-rd > 0 && vs[nv] == vs2[nv2-rd]) {
                        int len = sharedlen(vs2,nv2-rd,us,nu) + sharedlen(vs2,nv2-rd,vs,nv);
                        if (len) printf(" % 4d-cycle found At %d%%\n", cyclelengths[i] + cyclelengths[i2] - 2*len, (int)(i*100L/ncycles));
                    }
                } // else cyles are disjoint
            }
        } while ((i2 = prevcycle[i2]) >= 0);
    }

    delete[] us2;
    delete[] vs2;
}

int cyclebase::sharedlen(uint32_t *us, int nu, uint32_t *vs, int nv)
{
    int len = 0;
    for (; nu-- && nv-- && us[nu] == vs[nv]; len++) ;
    return len;
}

};
