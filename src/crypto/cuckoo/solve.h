// Copyright (c) 2013-2018 John Tromp
// Copyright (c) 2019 The Thought Core developers

#ifndef CUCKOO_SOLVE_H_
#define CUCKOO_SOLVE_H_

#include <vector>
#include <memory>

#include "crypto/cuckoo/cuckoo.h"
#include "consensus/params.h"

namespace cuckoo {

bool solve(CBlockHeader &header, Consensus::Params const &params);

};

#endif  // CUCKOO_SOLVE_H_
