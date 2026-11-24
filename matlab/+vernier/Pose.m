classdef Pose
    %POSE Pose of a pattern: translations x, y, z and intrinsic rotations alpha, beta, gamma.
    %
    %   p = vernier.Pose()                                        null pose
    %   p = vernier.Pose(x, y, alpha)                             2D pose
    %   p = vernier.Pose(x, y, alpha, pixelSize)
    %   p = vernier.Pose(x, y, z, alpha, beta, gamma)             3D pose
    %   p = vernier.Pose(x, y, z, alpha, beta, gamma, pixelSize)
    %
    %   Translations are in the same unit as the period of the pattern, angles
    %   are in radians (alpha about Z, beta about Y, gamma about X), as defined
    %   in [Andre2022pose].

    % This file is part of the VERNIER Library.
    %
    % Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

    properties
        x = 0           % translation along the X axis
        y = 0           % translation along the Y axis
        z = 0           % translation along the Z axis
        alpha = 0       % rotation angle about the Z axis (rad)
        beta = 0        % rotation angle about the Y intrinsic axis (rad)
        gamma = 0       % rotation angle about the X intrinsic axis (rad)
        pixelSize = 1   % pixel-to-meter scale factor
        is3D = true     % true if defined by x, y, z, alpha, beta, gamma, false if only by x, y, alpha
    end

    methods

        function obj = Pose(varargin)
            switch nargin
                case 0
                case {3, 4}
                    obj.x = varargin{1};
                    obj.y = varargin{2};
                    obj.alpha = varargin{3};
                    if nargin == 4
                        obj.pixelSize = varargin{4};
                    end
                    obj.is3D = false;
                case {6, 7}
                    obj.x = varargin{1};
                    obj.y = varargin{2};
                    obj.z = varargin{3};
                    obj.alpha = varargin{4};
                    obj.beta = varargin{5};
                    obj.gamma = varargin{6};
                    if nargin == 7
                        obj.pixelSize = varargin{7};
                    end
                otherwise
                    error('vernier:error', 'vernier.Pose expects 0, 3, 4, 6 or 7 arguments.');
            end
        end

        function s = toString(obj)
            %TOSTRING Returns a string describing the pose.
            vernier.Library.load();
            [status, ~, buffer] = calllib(vernier.Library.alias, 'vernierPoseToString', ...
                obj.toStruct(), blanks(256), 256);
            vernier.Library.check(status);
            s = vernier.Library.toString(buffer);
        end

        function T = getCameraToPatternTransformationMatrix(obj)
            %GETCAMERATOPATTERNTRANSFORMATIONMATRIX 4x4 matrix transforming a 3D point
            %   expressed in the pattern frame to the camera frame.
            T = obj.matrix('vernierPoseCameraToPatternMatrix');
        end

        function T = getPatternToCameraTransformationMatrix(obj)
            %GETPATTERNTOCAMERATRANSFORMATIONMATRIX 4x4 matrix transforming a 3D point
            %   expressed in the camera frame to the pattern frame.
            T = obj.matrix('vernierPosePatternToCameraMatrix');
        end

        function disp(obj)
            for k = 1:numel(obj)
                fprintf('  %s\n', obj(k).toString());
            end
        end

    end

    methods (Access = private)

        function T = matrix(obj, functionName)
            vernier.Library.load();
            [status, ~, values] = calllib(vernier.Library.alias, functionName, obj.toStruct(), zeros(4, 4));
            vernier.Library.check(status);
            T = reshape(values, 4, 4);
        end

    end

    methods (Hidden)

        function s = toStruct(obj)
            % The pose as the VernierPose structure of the C API.
            s = struct('x', obj.x, 'y', obj.y, 'z', obj.z, 'alpha', obj.alpha, ...
                'beta', obj.beta, 'gamma', obj.gamma, 'pixelSize', obj.pixelSize, ...
                'is3D', int32(obj.is3D));
        end

    end

    methods (Static, Hidden)

        function s = emptyStruct()
            % A VernierPose structure for the library to fill in.
            s = vernier.Pose().toStruct();
        end

        function pose = fromStruct(s)
            % A pose from the VernierPose structure filled in by the library.
            pose = vernier.Pose();
            pose.x = double(s.x);
            pose.y = double(s.y);
            pose.z = double(s.z);
            pose.alpha = double(s.alpha);
            pose.beta = double(s.beta);
            pose.gamma = double(s.gamma);
            pose.pixelSize = double(s.pixelSize);
            pose.is3D = s.is3D ~= 0;
        end

    end

end
