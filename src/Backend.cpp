/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.
 */

#include "Common.hpp"

#ifdef USE_CUDA
#include "CudaPhaseEngine.hpp"
#endif

namespace vernier {

    bool cudaAvailable() {
#ifdef USE_CUDA
        return CudaPhaseEngine::available();
#else
        return false;
#endif
    }

    void requireBackendAvailable(Backend backend) {
        if (backend != Backend::CUDA) {
            return;
        }
#ifdef USE_CUDA
        if (!CudaPhaseEngine::available()) {
            throw Exception("CUDA backend requested but no CUDA device is available.");
        }
#else
        throw Exception("CUDA backend requested but the library was built without USE_CUDA.");
#endif
    }

}
