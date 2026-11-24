classdef PatternDetector < handle
    %PATTERNDETECTOR Base class of the pattern detectors.
    %
    %   Detectors are created with the constructors of the subclasses
    %   (vernier.PeriodicPatternDetector, vernier.MegarenaPatternDetector,
    %   vernier.BitmapPatternDetector, vernier.StampPatternDetector,
    %   vernier.HPCodePatternDetector) or with the factory vernier.Detector
    %   (newInstance, loadFromJSON).
    %
    %   Typical use:
    %       detector.compute(image);
    %       if detector.patternFound()
    %           pose = detector.get2DPose();
    %       end

    % This file is part of the VERNIER Library.
    %
    % Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

    properties (SetAccess = private, Hidden)
        handle = []   % lib.pointer to the detector of the C API
    end

    methods

        function obj = PatternDetector(handle)
            % Wraps a detector created through the C API (internal use).
            if nargin > 0
                if isempty(handle) || handle.isNull()
                    error('vernier:error', '%s', vernier.Library.lastError());
                end
                obj.handle = handle;
            end
        end

        function delete(obj)
            if ~isempty(obj.handle) && libisloaded(vernier.Library.alias)
                calllib(vernier.Library.alias, 'vernierDetectorFree', obj.handle);
                obj.handle = [];
            end
        end

        function compute(obj, image)
            %COMPUTE Detects the patterns in an image and estimates their poses.
            %   The image is a 2-D matrix: double or single with values in [0, 1],
            %   uint8 or uint16 (scaled to [0, 1]) or logical. RGB images
            %   (M-by-N-by-3) are converted to grayscale.
            image = vernier.PatternDetector.toGrayscale(image);
            status = calllib(vernier.Library.alias, 'vernierDetectorCompute', obj.handle, ...
                image, size(image, 1), size(image, 2));
            vernier.Library.check(status);
        end

        function found = patternFound(obj, id)
            %PATTERNFOUND True if patterns have been detected and localized.
            if nargin < 2
                id = -1;
            end
            [status, found] = calllib(vernier.Library.alias, 'vernierDetectorPatternFound', ...
                obj.handle, id, int32(0));
            vernier.Library.check(status);
            found = found ~= 0;
        end

        function n = patternCount(obj)
            %PATTERNCOUNT Number of detected patterns.
            [status, n] = calllib(vernier.Library.alias, 'vernierDetectorPatternCount', ...
                obj.handle, int32(0));
            vernier.Library.check(status);
            n = double(n);
        end

        function pose = get2DPose(obj, id)
            %GET2DPOSE 2D pose (x, y, alpha) of the pattern, as a vernier.Pose.
            if nargin < 2
                id = -1;
            end
            pose = obj.poseOf('vernierDetectorGet2DPose', id);
        end

        function pose = get3DPose(obj, id)
            %GET3DPOSE Most likely 3D pose of the pattern, as a vernier.Pose.
            if nargin < 2
                id = -1;
            end
            pose = obj.poseOf('vernierDetectorGet3DPose', id);
        end

        function poses = getAll3DPoses(obj, id)
            %GETALL3DPOSES Possible 3D poses of the pattern in case of ambiguities,
            %   as a column array of vernier.Pose.
            if nargin < 2
                id = -1;
            end
            % the count first, then the poses themselves
            [status, ~, count] = calllib(vernier.Library.alias, 'vernierDetectorGetAll3DPoses', ...
                obj.handle, id, [], 0, int32(0));
            vernier.Library.check(status);
            count = double(count);
            if count == 0
                poses = vernier.Pose.empty(0, 1);
                return;
            end
            buffer = repmat(vernier.Pose.emptyStruct(), count, 1);
            [status, buffer, ~] = calllib(vernier.Library.alias, 'vernierDetectorGetAll3DPoses', ...
                obj.handle, id, buffer, count, int32(0));
            vernier.Library.check(status);
            for k = count:-1:1
                poses(k, 1) = vernier.Pose.fromStruct(buffer(k));
            end
        end

        function ids = getMarkerIds(obj)
            %GETMARKERIDS Ids of the markers found by a stamp or HP code
            %   detector, as a column vector in increasing order. Pass one of
            %   them to patternFound(id) and get2DPose(id).
            [status, ~, count] = calllib(vernier.Library.alias, 'vernierDetectorGetMarkerIds', ...
                obj.handle, [], 0, int32(0));
            vernier.Library.check(status);
            count = double(count);
            if count == 0
                ids = zeros(0, 1);
                return;
            end
            [status, ids, ~] = calllib(vernier.Library.alias, 'vernierDetectorGetMarkerIds', ...
                obj.handle, zeros(count, 1, 'int32'), count, int32(0));
            vernier.Library.check(status);
            ids = double(ids(:));
        end

        function markers = getMarkers(obj)
            %GETMARKERS Poses of the markers found by a stamp or HP code
            %   detector, as a containers.Map from marker id to vernier.Pose.
            markers = containers.Map('KeyType', 'double', 'ValueType', 'any');
            ids = obj.getMarkerIds();
            for k = 1:numel(ids)
                markers(ids(k)) = obj.get2DPose(ids(k));
            end
        end

        function setBackend(obj, backend)
            %SETBACKEND Selects the compute backend of the detector, 'CPU' (the
            %   default) or 'CUDA'. CUDA needs a library built with
            %   -DUSE_CUDA=ON and a CUDA device (see vernier.cudaAvailable). An
            %   unavailable backend raises an error and leaves the current one
            %   in place.
            status = calllib(vernier.Library.alias, 'vernierDetectorSetBackend', ...
                obj.handle, vernier.Library.toBackend(backend));
            vernier.Library.check(status);
        end

        function backend = getBackend(obj)
            %GETBACKEND Compute backend used by the detector, 'CPU' or 'CUDA'.
            [status, backend] = calllib(vernier.Library.alias, 'vernierDetectorGetBackend', ...
                obj.handle, int32(0));
            vernier.Library.check(status);
            backend = vernier.Library.fromBackend(backend);
        end

        function name = getClassname(obj)
            %GETCLASSNAME Kind of pattern detected, e.g. 'PeriodicPattern'.
            name = obj.stringOf('vernierDetectorGetClassname');
        end

        function s = toString(obj)
            %TOSTRING Returns a string describing the detector.
            s = obj.stringOf('vernierDetectorToString');
        end

        function description = getDescription(obj)
            %GETDESCRIPTION Description of the pattern, as read from its JSON file.
            description = obj.stringOf('vernierDetectorGetDescription');
        end

        function author = getAuthor(obj)
            %GETAUTHOR Author of the pattern, as read from its JSON file.
            author = obj.stringOf('vernierDetectorGetAuthor');
        end

        function date = getDate(obj)
            %GETDATE Date of the pattern, as read from its JSON file.
            date = obj.stringOf('vernierDetectorGetDate');
        end

        function unit = getUnit(obj)
            %GETUNIT Length unit of the pattern, as read from its JSON file.
            unit = obj.stringOf('vernierDetectorGetUnit');
        end

        function disp(obj)
            fprintf('  %s\n', obj.toString());
        end

    end

    methods (Access = protected)

        function pose = poseOf(obj, functionName, id)
            [status, pose] = calllib(vernier.Library.alias, functionName, ...
                obj.handle, id, vernier.Pose.emptyStruct());
            vernier.Library.check(status);
            pose = vernier.Pose.fromStruct(pose);
        end

        function text = stringOf(obj, functionName)
            [status, buffer] = calllib(vernier.Library.alias, functionName, ...
                obj.handle, blanks(1024), 1024);
            vernier.Library.check(status);
            text = vernier.Library.toString(buffer);
        end

    end

    methods (Static, Hidden)

        function image = toGrayscale(image)
            % Converts an image to the 2-D double array the C API expects,
            % with the same scaling as the C++ cv::Mat interface.
            if isinteger(image)
                image = double(image) / double(intmax(class(image)));
            else
                image = double(image);
            end
            if ndims(image) == 3 && size(image, 3) == 3
                image = 0.299 * image(:, :, 1) + 0.587 * image(:, :, 2) + 0.114 * image(:, :, 3);
            end
        end

    end

end
