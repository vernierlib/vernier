% Minimal example of the Matlab bindings of the VERNIER library.
%
% Run from the directory holding the +vernier package, the C API library and
% the images (build/matlab after a CMake build):
%     matlab -batch vernier_example

% This file is part of the VERNIER Library.
%
% Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

%% Measuring the absolute pose of a megarena pattern in a camera image

image = imread('megarenaPatternImage_12bits_9um.jpg');  % RGB images are converted to grayscale

detector = vernier.MegarenaPatternDetector(9, 12);      % physical period (um), code size (bits)
if vernier.cudaAvailable()                              % library built with CUDA and a device is present
    detector.setBackend('CUDA');
end
detector.compute(image);

if detector.patternFound()
    pose = detector.get2DPose();
    fprintf('Megarena pose:  %s\n', pose.toString());
else
    fprintf('Pattern not found...\n');
end

%% Measuring the pose of a periodic pattern, modulo its period

image = imread(fullfile('data', 'periodicPattern.pgm'));

detector = vernier.PeriodicPatternDetector(15.0);       % physical period
detector.setSigma(1.0);
detector.setCropFactor(0.4);
detector.compute(image);

if detector.patternFound()
    pose = detector.get2DPose();
    fprintf('True pose:      %s\n', vernier.Pose(6.0, 3.0, 0.2, 2.0).toString());
    fprintf('Estimated pose: %s\n', pose.toString());
else
    fprintf('Pattern not found...\n');
end
