// Copyright (c) 2013-2016 John Tromp
// Copyright (c) 2018-2019 The Thought Core developers

#ifndef CUCKOO_VERIFY_H_
#define CUCKOO_VERIFY_H_

#include "crypto/cuckoo/cuckoo.h"

/// Cuckoo cycle-related functions and classes.
namespace cuckoo {

uint32_t sipnode(siphash_keys *keys, uint32_t nonce, uint32_t uorv, uint32_t edgemask);

// set siphash keys from 64 byte char array
void siphash_setkeys(siphash_keys *keys, const unsigned char *keybuf);

// SipHash-2-4 specialized to precomputed key and 8 byte nonces
uint64_t siphash24(const siphash_keys *keys, const uint64_t nonce);

/// Result of cuckoo proof verification.
/// Value descriptions taken from tromp/cuckoo.h
enum verify_code {
    POW_OK,  ///< Valid proof.
    POW_TOO_BIG,  ///< Edge too big.
    POW_TOO_SMALL,  ///< Edges not ascending.
    POW_NON_MATCHING,  ///< Non-matching endpoints.
    POW_BRANCH,  ///< Cycle has a branch.
    POW_DEAD_END,  ///< Cycle dead-ends.
    POW_SHORT_CYCLE  ///< Cycle is too short.
};

/**
 * Verify a cuckoo proof given the proof nonces, keys, and graph size.
 * @param nonces Proof nonces.
 * @param buf Buffer of keys. Must be at least 64 bytes.
 * @param graphSize Cuckoo graph size.
 * @return Verification result.
 */
verify_code verify(uint32_t nonces[PROOFSIZE], const unsigned char *buf, uint32_t graphSize);

};

#endif  // CUCKOO_VERIFY_H_
