#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""CUDA example: run the same detection on the CPU and on the GPU.

The heavy part of a detection is the phase-retrieval stage (`PatternPhase`):
FFT, spectrum filtering, peak search, inverse FFTs. That stage has a CUDA
implementation (cuFFT + custom kernels) which is selected at runtime with
`setBackend(vernier.Backend.CUDA)`; everything else in the API is unchanged.

The GPU path is only present if the library was built with `-DUSE_CUDA=ON` and
a CUDA device is visible at runtime:

    cmake -DUSE_CUDA=ON ..
    make pyvernier

Run it from the build directory where the compiled `pyvernier` module lives:

    cd build/python && python3 usingCudaBackend.py
"""

import pyvernier as vernier

PHYSICAL_PERIOD = 15.0
TRUE_POSE = vernier.Pose(6.0, 3.0, 0.2, 2.0)  # x, y, alpha, pixelSize


def detect(image, backend):
    """Estimates the pose of the pattern in `image` using the given backend."""
    detector = vernier.PeriodicPatternDetector(PHYSICAL_PERIOD)
    detector.setSigma(1.0)
    detector.setCropFactor(0.4)

    # The backend lives on the phase-retrieval stage. `detector.setBackend(...)`
    # is a shortcut for `detector.getPatternPhase().setBackend(...)`.
    detector.setBackend(backend)

    detector.compute(image)
    if not detector.patternFound():
        raise RuntimeError("pattern not found with backend %s" % backend)
    return detector.get2DPose()


# Render a test image of a periodic pattern at a known pose.
layout = vernier.PeriodicPatternLayout(PHYSICAL_PERIOD, 31, 31)
image = layout.renderOrthographicProjection(TRUE_POSE, 512, 512)
print("True pose:     ", TRUE_POSE)

# The CPU path is always available.
cpu_pose = detect(image, vernier.Backend.CPU)
print("CPU pose:      ", cpu_pose)

# `cudaAvailable()` answers both questions at once: built with CUDA, and a
# device present. Asking for the CUDA backend without it raises VernierError.
if not vernier.cudaAvailable():
    print("CUDA backend:   unavailable "
          "(build with -DUSE_CUDA=ON and run on a machine with a CUDA device)")
    raise SystemExit(0)

cuda_pose = detect(image, vernier.Backend.CUDA)
print("CUDA pose:     ", cuda_pose)

# Both backends compute in double precision, so the two poses should agree to
# well below the measurement resolution.
print("CPU/CUDA difference: dx=%.2e dy=%.2e dalpha=%.2e"
      % (cuda_pose.x - cpu_pose.x,
         cuda_pose.y - cpu_pose.y,
         cuda_pose.alpha - cpu_pose.alpha))
