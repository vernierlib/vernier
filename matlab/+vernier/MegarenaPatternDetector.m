classdef MegarenaPatternDetector < vernier.PeriodicPatternDetector
    %MEGARENAPATTERNDETECTOR Detector of megarena patterns.
    %
    %   detector = vernier.MegarenaPatternDetector(physicalPeriod, codeSize)
    %
    %   Megarena patterns are periodic patterns with an embedded binary code,
    %   which gives the absolute position over the whole pattern. codeSize is
    %   the number of bits of the code (e.g. 12).

    % This file is part of the VERNIER Library.
    %
    % Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

    methods

        function obj = MegarenaPatternDetector(physicalPeriod, codeSize, handle)
            % The handle argument is for internal use only.
            if nargin < 3
                if nargin < 2
                    error('vernier:error', 'vernier.MegarenaPatternDetector expects a physical period and a code size.');
                end
                vernier.Library.load();
                handle = calllib(vernier.Library.alias, 'vernierDetectorNewMegarena', ...
                    physicalPeriod, int32(codeSize));
            end
            obj@vernier.PeriodicPatternDetector([], handle);
        end

    end

end
