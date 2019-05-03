// Copyright (c) 2013-2018 John Tromp
// Copyright (c) 2019 The Thought Core developers

#include "crypto/cuckoo/solve.h"
#include "crypto/cuckoo/context.h"

#include "pow.h"

namespace cuckoo {

bool solve(CBlockHeader &header, Consensus::Params const &params)
{
    int easipct = 50;

    uint32_t nnodes = ((uint32_t)1 << params.cuckooGraphSize);
    uint32_t nedges = ((uint32_t)1 << (params.cuckooGraphSize - 1));
    uint32_t edgemask =  ((uint32_t)nedges - 1);

    assert(easipct >= 0 && easipct <= 100);
    word_t easiness = easipct * (word_t)nnodes / 100;
    CContext ctx(easiness, params.cuckooGraphSize);

    for (header.nNonce == 0; header.nNonce <= std::numeric_limits<decltype(header.nNonce)>::max(); header.nNonce += 1) {
        ctx.SetHeader(header);
        ctx.CycleBase();
        ctx.cb.cycles();

        for (auto const &sol : ctx.cb.solutions) {
            if (sol.size() != PROOFSIZE) {
                printf("non-proofsize solution\n");
                continue;
            }
            memcpy(header.cuckooProof, sol.data(), sizeof(uint32_t) * PROOFSIZE);
            if (CheckCuckooProofOfWork(header, params)) {
                return true;
            }
        }
    }
    return false;
}

};
