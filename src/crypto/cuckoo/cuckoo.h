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

#ifndef CUCKOO_H_
#define CUCKOO_H_

#include <cstdint>
#include <cstddef>

#ifndef ROTL
#define ROTL(x,b) (uint64_t)( ((x) << (b)) | ( (x) >> (64 - (b))) )
#endif 
#ifndef SIPROUND
#define SIPROUND \
  do { \
    v0 += v1; v2 += v3; v1 = ROTL(v1,13); \
    v3 = ROTL(v3,16); v1 ^= v0; v3 ^= v2; \
    v0 = ROTL(v0,32); v2 += v1; v0 += v3; \
    v1 = ROTL(v1,17);   v3 = ROTL(v3,21); \
    v1 ^= v2; v3 ^= v0; v2 = ROTL(v2,32); \
  } while(0)
#endif

namespace cuckoo {

typedef struct {
    uint64_t k0;
    uint64_t k1;
    uint64_t k2;
    uint64_t k3;
} siphash_keys;

/// Number of nonces in a cuckoo proof.
constexpr size_t PROOFSIZE = 42;

/// Size in bytes of the header blob used to generate siphash keys.
constexpr size_t HEADERSIZE = 80;

static_assert(
    HEADERSIZE >= sizeof(siphash_keys),
    "Header size must be at least as big as struct siphash_keys"
);

};

#endif  // CUCKOO_H_
