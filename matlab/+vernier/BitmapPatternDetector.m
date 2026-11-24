classdef BitmapPatternDetector < vernier.PeriodicPatternDetector
    %BITMAPPATTERNDETECTOR Detector of bitmap patterns.
    %
    %   detector = vernier.BitmapPatternDetector(physicalPeriod, filename)
    %
    %   Bitmap patterns are periodic patterns whose dots follow a bitmap read
    %   from a PNG file (white dots on a black background), which gives the
    %   absolute position over the pattern.

    % This file is part of the VERNIER Library.
    %
    % Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

    methods

        function obj = BitmapPatternDetector(physicalPeriod, filename, handle)
            % The handle argument is for internal use only.
            if nargin < 3
                if nargin < 2
                    error('vernier:error', 'vernier.BitmapPatternDetector expects a physical period and a bitmap file.');
                end
                vernier.Library.load();
                handle = calllib(vernier.Library.alias, 'vernierDetectorNewBitmap', ...
                    physicalPeriod, char(filename));
            end
            obj@vernier.PeriodicPatternDetector([], handle);
        end

    end

end
