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

%% Locating HP code markers in a camera image

image = imread('HPCodeImage_20um.png');

detector = vernier.HPCodePatternDetector(20.0, 33, 256);   % physical period (um), code size (half periods), snapshot size (pixels)
detector.compute(image);

ids = detector.getMarkerIds();
fprintf('Found %d HP codes\n', numel(ids));
for id = ids'
    fprintf('  Code %d at %s\n', id, detector.get2DPose(id).toString());
end

%% Locating a stamp in a camera image

image = imread('stampPatternImage_15um.jpg');

detector = vernier.StampPatternDetector(15.0, 'stampPattern.png', 820);   % physical period (um), bitmap, snapshot size (pixels)
detector.compute(image);

markers = detector.getMarkers();                           % containers.Map from stamp id to pose
for id = cell2mat(markers.keys())
    fprintf('Stamp %d at %s\n', id, markers(id).toString());
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
