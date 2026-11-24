classdef PeriodicPatternDetector < vernier.PatternDetector
    %PERIODICPATTERNDETECTOR Detector of periodic patterns.
    %
    %   detector = vernier.PeriodicPatternDetector(physicalPeriod)
    %
    %   Estimates the pose of a periodic pattern modulo its period, with
    %   subpixel resolution.

    % This file is part of the VERNIER Library.
    %
    % Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

    methods

        function obj = PeriodicPatternDetector(physicalPeriod, handle)
            % The handle argument is for internal use only.
            if nargin < 1 || isempty(physicalPeriod)
                physicalPeriod = 1.0;
            end
            if nargin < 2
                vernier.Library.load();
                handle = calllib(vernier.Library.alias, 'vernierDetectorNewPeriodic', physicalPeriod);
            end
            obj@vernier.PatternDetector(handle);
        end

        function pose = get3DPosePerspective(obj, focalLength)
            %GET3DPOSEPERSPECTIVE 3D pose of the pattern assuming a perspective
            %   projection with the given focal length (in pixels).
            [status, pose] = calllib(vernier.Library.alias, 'vernierDetectorGet3DPosePerspective', ...
                obj.handle, focalLength, vernier.Pose.emptyStruct());
            vernier.Library.check(status);
            pose = vernier.Pose.fromStruct(pose);
        end

        function period = getPhysicalPeriod(obj)
            [status, period] = calllib(vernier.Library.alias, 'vernierDetectorGetPhysicalPeriod', ...
                obj.handle, 0);
            vernier.Library.check(status);
            period = double(period);
        end

        function setPhysicalPeriod(obj, physicalPeriod)
            obj.setParameter('vernierDetectorSetPhysicalPeriod', physicalPeriod);
        end

        function setSigma(obj, sigma)
            %SETSIGMA Sets the standard deviation of the spectral filter.
            obj.setParameter('vernierDetectorSetSigma', sigma);
        end

        function setCropFactor(obj, cropFactor)
            %SETCROPFACTOR Sets the fraction of the phase map used for the plane regression.
            obj.setParameter('vernierDetectorSetCropFactor', cropFactor);
        end

        function setMaxFrequency(obj, maxFrequency)
            obj.setParameter('vernierDetectorSetMaxFrequency', maxFrequency);
        end

        function setMinFrequency(obj, minFrequency)
            obj.setParameter('vernierDetectorSetMinFrequency', minFrequency);
        end

        function setMinPeakPower(obj, minPeakPower)
            obj.setParameter('vernierDetectorSetMinPeakPower', minPeakPower);
        end

        function setSmoothingKernelSize(obj, smoothingKernelSize)
            status = calllib(vernier.Library.alias, 'vernierDetectorSetSmoothingKernelSize', ...
                obj.handle, int32(smoothingKernelSize));
            vernier.Library.check(status);
        end

    end

    methods (Access = private)

        function setParameter(obj, functionName, value)
            if ~isnumeric(value) || ~isscalar(value)
                error('vernier:error', 'the value must be a real scalar.');
            end
            status = calllib(vernier.Library.alias, functionName, obj.handle, value);
            vernier.Library.check(status);
        end

    end

end
