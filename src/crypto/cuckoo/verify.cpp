/*
     The MIT License (MIT)

    Copyright (c) 2013-2019 John Tromp

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

*/
#include "crypto/cuckoo/verify.h"
#include "compat/endian.h"

namespace cuckoo {

uint64_t siphash24(const siphash_keys *keys, const uint64_t nonce) {
  uint64_t v0 = keys->k0, v1 = keys->k1, v2 = keys->k2, v3 = keys->k3 ^ nonce;
  SIPROUND; SIPROUND;
  v0 ^= nonce;
  v2 ^= 0xff;
  SIPROUND; SIPROUND; SIPROUND; SIPROUND;
  return (v0 ^ v1) ^ (v2  ^ v3);
}

void siphash_setkeys(siphash_keys *keys, const unsigned char *keybuf) {
  keys->k0 = htole64(((uint64_t *)keybuf)[0]);
  keys->k1 = htole64(((uint64_t *)keybuf)[1]);
  keys->k2 = htole64(((uint64_t *)keybuf)[2]);
  keys->k3 = htole64(((uint64_t *)keybuf)[3]);
}

uint32_t sipnode(siphash_keys *keys, uint32_t nonce, uint32_t uorv, uint32_t edgemask) {
	return (siphash24(keys, 2*nonce + uorv) & edgemask) << 1 | uorv;
}

verify_code verify(uint32_t nonces[PROOFSIZE], const unsigned char *buf, uint32_t graphSize) {

  uint32_t nnodes = ((uint32_t)1 << graphSize);
  uint32_t nedges = ((uint32_t)1 << (graphSize - 1));
  uint32_t edgemask =  ((uint32_t)nedges - 1);

  siphash_keys keys;
  siphash_setkeys(&keys, buf);
  uint32_t uvs[2*PROOFSIZE];
  uint32_t xor0=0,xor1=0;
  for (uint32_t n = 0; n < PROOFSIZE; n++) {
    if (nonces[n] > nnodes)
      return POW_TOO_BIG;
    if (n && nonces[n] <= nonces[n-1])
      return POW_TOO_SMALL;
	xor0 ^= uvs[2 * n] = sipnode(&keys, nonces[n], 0, edgemask);
	xor1 ^= uvs[2 * n + 1] = sipnode(&keys, nonces[n], 1, edgemask);
  }
  if (xor0|xor1)              // matching endpoints imply zero xors
    return POW_NON_MATCHING;
  uint32_t n = 0, i = 0, j;
  do {                        // follow cycle
    for (uint32_t k = j = i; (k = (k+2) % (2*PROOFSIZE)) != i; ) {
      if (uvs[k] == uvs[i]) { // find other edge endpoint identical to one at i
        if (j != i)           // already found one before
          return POW_BRANCH;
        j = k;
      }
    }
    if (j == i) return POW_DEAD_END;  // no matching endpoint
    i = j^1;
    n++;
  } while (i != 0);           // must cycle back to start or we would have found branch
  return n == PROOFSIZE ? POW_OK : POW_SHORT_CYCLE;
}

};
