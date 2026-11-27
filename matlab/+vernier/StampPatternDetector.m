classdef StampPatternDetector < vernier.BitmapPatternDetector
    %STAMPPATTERNDETECTOR Detector of stamps, small square bitmap markers.
    %
    %   detector = vernier.StampPatternDetector(physicalPeriod, filename, snapshotSize)
    %
    %   The stamp bitmap is read from a PNG file and must be square with an
    %   odd number of dots. snapshotSize is the largest diagonal of a stamp in
    %   the image, in pixels. Several stamps can be located in one image: see
    %   getMarkerIds() and getMarkers(), and addBitmap() to recognise more than
    %   one kind of stamp.

    % This file is part of the VERNIER Library.
    %
    % Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

    methods

        function obj = StampPatternDetector(physicalPeriod, filename, snapshotSize, handle)
            % The handle argument is for internal use only.
            if nargin < 4
                if nargin < 3
                    error('vernier:error', ['vernier.StampPatternDetector expects a physical period, ' ...
                        'a bitmap file and a snapshot size.']);
                end
                vernier.Library.load();
                handle = calllib(vernier.Library.alias, 'vernierDetectorNewStamp', ...
                    physicalPeriod, char(filename), int32(snapshotSize));
            end
            obj@vernier.BitmapPatternDetector([], [], handle);
        end

        function addBitmap(obj, filename)
            %ADDBITMAP Adds a stamp bitmap, read from a PNG file of the same
            %   size as the first one.
            status = calllib(vernier.Library.alias, 'vernierDetectorAddStampBitmap', ...
                obj.handle, char(filename));
            vernier.Library.check(status);
        end

    end

end
