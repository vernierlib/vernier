classdef Detector
    %DETECTOR Factory creating pattern detectors.
    %
    %   detector = vernier.Detector.newInstance(classname)
    %       Creates a detector with default parameters. classname is one of
    %       'PeriodicPattern', 'MegarenaPattern', 'BitmapPattern',
    %       'HPCodePattern' or 'StampPattern'.
    %
    %   detector = vernier.Detector.loadFromJSON(filename)
    %       Creates the detector described by a JSON pattern file.
    %
    %   Both take the compute backend, 'CPU' (the default) or 'CUDA', as an
    %   optional last argument, e.g. vernier.Detector.loadFromJSON(filename, 'CUDA').
    %
    %   Megarena detectors are returned as vernier.MegarenaPatternDetector.
    %   Periodic, bitmap, HP code and stamp detectors, which all derive from
    %   the periodic pattern detector, are returned as
    %   vernier.PeriodicPatternDetector.

    % This file is part of the VERNIER Library.
    %
    % Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

    methods (Static)

        function detector = newInstance(classname, backend)
            if nargin < 2
                backend = 'CPU';
            end
            vernier.Library.load();
            handle = calllib(vernier.Library.alias, 'vernierDetectorNew', ...
                classname, vernier.Library.toBackend(backend));
            detector = vernier.Detector.wrap(handle);
        end

        function detector = loadFromJSON(filename, backend)
            if nargin < 2
                backend = 'CPU';
            end
            vernier.Library.load();
            handle = calllib(vernier.Library.alias, 'vernierDetectorLoadFromJSON', ...
                filename, vernier.Library.toBackend(backend));
            detector = vernier.Detector.wrap(handle);
        end

    end

    methods (Static, Hidden)

        function detector = wrap(handle)
            if isempty(handle) || handle.isNull()
                error('vernier:error', '%s', vernier.Library.lastError());
            end
            % the kind of pattern, read before wrapping it in a class that
            % would free the detector when the temporary goes away
            [status, buffer] = calllib(vernier.Library.alias, 'vernierDetectorGetClassname', ...
                handle, blanks(256), 256);
            vernier.Library.check(status);
            switch vernier.Library.toString(buffer)
                case {'PeriodicPattern', 'BitmapPattern', 'HPCodePattern', 'StampPattern'}
                    detector = vernier.PeriodicPatternDetector([], handle);
                case 'MegarenaPattern'
                    detector = vernier.MegarenaPatternDetector([], [], handle);
                otherwise
                    detector = vernier.PatternDetector(handle);
            end
        end

    end

end
