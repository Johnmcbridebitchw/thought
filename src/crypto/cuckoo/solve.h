// Copyright (c) 2013-2018 John Tromp
// Copyright (c) 2019 The Thought Core developers

#ifndef CUCKOO_SOLVE_H_
#define CUCKOO_SOLVE_H_

#include <vector>
#include <memory>

#include "crypto/cuckoo/cuckoo.h"

namespace cuckoo {

std::unique_ptr<std::vector<uint32_t>> solve(CBlockHeader &header, uint32_t graphSize);

};

#endif  // CUCKOO_SOLVE_H_
