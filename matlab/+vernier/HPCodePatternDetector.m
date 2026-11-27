classdef HPCodePatternDetector < vernier.PeriodicPatternDetector
    %HPCODEPATTERNDETECTOR Detector of HP codes, periodic QR-like markers.
    %
    %   detector = vernier.HPCodePatternDetector(physicalPeriod, numberHalfPeriods, snapshotSize)
    %
    %   numberHalfPeriods is the size of the code in half periods along one
    %   direction, of the form 4k + 1 (33 or 37 for instance, the default is
    %   37). snapshotSize is the largest size of a code in the image, in pixels
    %   (128 by default). Several codes can be located in one image: see
    %   getMarkerIds() and getMarkers().

    % This file is part of the VERNIER Library.
    %
    % Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

    methods

        function obj = HPCodePatternDetector(physicalPeriod, numberHalfPeriods, snapshotSize, handle)
            % The handle argument is for internal use only.
            if nargin < 4
                if nargin < 1
                    error('vernier:error', 'vernier.HPCodePatternDetector expects a physical period.');
                end
                if nargin < 2 || isempty(numberHalfPeriods)
                    numberHalfPeriods = 37;
                end
                if nargin < 3 || isempty(snapshotSize)
                    snapshotSize = 128;
                end
                vernier.Library.load();
                handle = calllib(vernier.Library.alias, 'vernierDetectorNewHPCode', ...
                    physicalPeriod, int32(numberHalfPeriods), int32(snapshotSize));
            end
            obj@vernier.PeriodicPatternDetector([], handle);
        end

    end

end
