function available = cudaAvailable()
%CUDAAVAILABLE True if the library was built with CUDA support (-DUSE_CUDA=ON)
%   and a CUDA device is present, i.e. if the 'CUDA' backend can be selected
%   with the setBackend method of the detectors.
%
%   detector = vernier.MegarenaPatternDetector(9, 12);
%   if vernier.cudaAvailable()
%       detector.setBackend('CUDA');
%   end

% This file is part of the VERNIER Library.
%
% Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

vernier.Library.load();
available = calllib(vernier.Library.alias, 'vernierCudaAvailable') ~= 0;
end
