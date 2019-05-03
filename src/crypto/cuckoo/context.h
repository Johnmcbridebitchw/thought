// Copyright (c) 2013-2018 John Tromp
// Copyright (c) 2019 The Thought Core developers

#ifndef CUCKOO_CONTEXT_H_
#define CUCKOO_CONTEXT_H_

#include "crypto/cuckoo/cyclebase.h"
#include "primitives/block.h"

namespace cuckoo {

class CContext
{
public:
    siphash_keys sip_keys;
    word_t easiness;
    cyclebase cb;
    uint32_t edgemask;

    CContext(word_t easy_ness, uint32_t graphSize);
    ~CContext();

    void SetHeader(CBlockHeader const &header);

    void CycleBase();
};

};

#endif  // CUCKOO_CONTEXT_H_
