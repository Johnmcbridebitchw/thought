// Copyright (c) 2013-2018 John Tromp
// Copyright (c) 2019 The Thought Core developers

#include "crypto/cuckoo/context.h"

namespace cuckoo {

CContext::CContext(word_t easy_ness, uint32_t graphSize)
{
    easiness = easy_ness;
    uint32_t nedges = ((uint32_t)1 << (graphSize - 1));
    edgemask = ((uint32_t)nedges - 1);
    cb.alloc(graphSize);
    assert(cb.cuckoo != 0);
    assert(cb.us != 0);
    assert(cb.vs != 0);
}

CContext::~CContext()
{
    cb.freemem();
}

void CContext::SetHeader(CBlockHeader const &header)
{
    unsigned char hash[32];
    hash_blockheader(header, hash);
    siphash_setkeys(&sip_keys, hash);
    cb.keys = &sip_keys;
    cb.reset();
}

void CContext::CycleBase()
{
    for (word_t nonce = 0; nonce < easiness; nonce++) {
        word_t u = sipnode(&sip_keys, nonce, 0, edgemask);
        word_t v = sipnode(&sip_keys, nonce, 1, edgemask);
        cb.addedge(u, v);
    }
}

};
