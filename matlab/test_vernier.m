function test_vernier()
%TEST_VERNIER Tests of the Matlab bindings of the VERNIER library.
%
%   Run from the directory holding the +vernier package, the C API library and
%   the test images (build/matlab after a CMake build):
%       matlab -batch test_vernier
%
%   The images of data/ were rendered from the C++ layouts at known poses (see
%   c/data/README.md): the bindings expose the detectors only, so the tests
%   cannot render their own images.

% This file is part of the VERNIER Library.
%
% Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

tests = {@testLibrary, @testPose, @testPeriodicPattern, @testNonSquareImage, ...
    @testImageTypes, @testCameraImage, @testParameters, @testMegarenaPattern, ...
    @testStampPattern, @testHPCodePattern, @testMarkersInCameraImages, ...
    @testFactories, @testErrors, @testBackend, @testUnload};
for k = 1:numel(tests)
    fprintf('%s... ', func2str(tests{k}));
    tests{k}();
    fprintf('ok\n');
end
fprintf('All %d tests passed.\n', numel(tests));
end

function testLibrary()
vernier.Library.load();
assert(libisloaded(vernier.Library.alias));
assert(ischar(vernier.Library.version()) && ~isempty(vernier.Library.version()));
end

function testPose()
p = vernier.Pose(6, 3, 0.2, 2);
assert(~p.is3D && p.x == 6 && p.y == 3 && p.alpha == 0.2 && p.pixelSize == 2);
assert(~isempty(strfind(p.toString(), 'x=6')));
q = vernier.Pose(1, 2, 3, 0.1, 0.2, 0.3);
assert(q.is3D && q.z == 3 && q.gamma == 0.3 && q.pixelSize == 1);
T = q.getCameraToPatternTransformationMatrix() * q.getPatternToCameraTransformationMatrix();
assert(isequal(size(T), [4 4]) && max(abs(T(:) - reshape(eye(4), [], 1))) < 1e-12);
end

function testPeriodicPattern()
image = imread(fullfile('data', 'periodicPattern.pgm'));
assert(isequal(size(image), [512 512]));
truth = vernier.Pose(6, 3, 0.2, 2);
detector = vernier.PeriodicPatternDetector(15);
detector.setSigma(1);
detector.setCropFactor(0.4);
detector.compute(image);
assert(detector.patternFound() && detector.patternCount() == 1);
assertPoseNear(detector.get2DPose(), truth, 0.05);
end

function testNonSquareImage()
% a row/column mix-up in the bindings would give a wrong pose
image = imread(fullfile('data', 'nonSquarePattern.pgm'));
assert(isequal(size(image), [300 420]));
truth = vernier.Pose(1.5, -2.5, 0.4, 1.2);
detector = vernier.PeriodicPatternDetector(8);
detector.compute(image);
assert(detector.patternFound());
assertPoseNear(detector.get2DPose(), truth, 0.05);
end

function testImageTypes()
image = imread(fullfile('data', 'periodicPattern.pgm'));
truth = vernier.Pose(6, 3, 0.2, 2);
scaled = double(image) / 255;
inputs = {scaled, single(scaled), uint16(round(65535 * scaled)), cat(3, image, image, image)};
detector = vernier.PeriodicPatternDetector(15);
for k = 1:numel(inputs)
    detector.compute(inputs{k});
    assert(detector.patternFound());
    assertPoseNear(detector.get2DPose(), truth, 0.05);
end
end

function testCameraImage()
% a real camera image, straight from imread as an RGB uint8 array
image = imread('megarenaPatternImage_12bits_9um.jpg');
detector = vernier.MegarenaPatternDetector(9, 12);
detector.compute(image);
assert(detector.patternFound());
pose = detector.get2DPose();
% the pose the C++ library estimates from the same image
assertPoseNear(pose, vernier.Pose(-34827.3, -26759.6, 2.99895, 0.611845), 0.1);
end

function testParameters()
detector = vernier.PeriodicPatternDetector(15);
assert(detector.getPhysicalPeriod() == 15);
assert(strcmp(detector.getClassname(), 'PeriodicPattern'));
detector.setSigma(2.5);
detector.setCropFactor(0.5);
detector.setMinFrequency(2);
detector.setMaxFrequency(100);
detector.setMinPeakPower(0.1);
detector.setSmoothingKernelSize(3);
detector.setPhysicalPeriod(12);
assert(detector.getPhysicalPeriod() == 12);
assert(ischar(detector.toString()) && ~isempty(detector.toString()));
end

function testMegarenaPattern()
image = imread(fullfile('data', 'megarenaPattern.pgm'));
truth = vernier.Pose(-15 * 400, -15 * 200, 0.2, 1);
detector = vernier.MegarenaPatternDetector(10, 12);
assert(isa(detector, 'vernier.PeriodicPatternDetector'));
detector.compute(image);
assert(detector.patternFound());
assertPoseNear(detector.get2DPose(), truth, 0.01);
end

function testStampPattern()
image = imread(fullfile('data', 'stampPattern.pgm'));
truth = vernier.Pose(20, -10, 0.3, 1);
detector = vernier.StampPatternDetector(5, fullfile('data', 'stamp.png'), 460);
assert(isa(detector, 'vernier.BitmapPatternDetector') && isa(detector, 'vernier.PeriodicPatternDetector'));
assert(strcmp(detector.getClassname(), 'StampPattern'));
detector.compute(image);
assert(detector.patternFound() && detector.patternCount() == 1);
% the stamp has the id of its bitmap, the first one
assert(isequal(detector.getMarkerIds(), 0));
assert(detector.patternFound(0) && ~detector.patternFound(7));
assertPoseNear(detector.get2DPose(0), truth, 0.05);
assertPoseNear(detector.get2DPose(), truth, 0.05);
markers = detector.getMarkers();
assert(isa(markers, 'containers.Map') && markers.Count == 1 && markers.isKey(0));
assertPoseNear(markers(0), truth, 0.05);
detector.addBitmap(fullfile('data', 'stamp.png'));

% a stamp is a bitmap pattern too
detector = vernier.BitmapPatternDetector(5, fullfile('data', 'stamp.png'));
assert(strcmp(detector.getClassname(), 'BitmapPattern'));
detector.compute(image);
assert(detector.patternFound());
assertPoseNear(detector.get2DPose(), truth, 0.05);
end

function testHPCodePattern()
image = imread(fullfile('data', 'hpcodePattern.pgm'));
truth = vernier.Pose(12, -7, 0.5, 1);
detector = vernier.HPCodePatternDetector(8, 33, 256);
assert(isa(detector, 'vernier.PeriodicPatternDetector'));
assert(strcmp(detector.getClassname(), 'HPCodePattern'));
detector.compute(image);
assert(detector.patternFound() && detector.patternCount() == 1);
assert(isequal(detector.getMarkerIds(), 0));
assertPoseNear(detector.get2DPose(0), truth, 0.05);
end

function testMarkersInCameraImages()
% two HP codes in a real image, with the poses the C++ library estimates
image = imread('HPCodeImage_20um.png');
detector = vernier.HPCodePatternDetector(20, 33, 256);
detector.compute(image);
assert(detector.patternCount() == 2);
assert(isequal(detector.getMarkerIds(), [0; 1]));
assertPoseNear(detector.get2DPose(0), vernier.Pose(325.407, 131.286, 0.0109354, 2.40894), 0.1);
assertPoseNear(detector.get2DPose(1), vernier.Pose(107.308, 327.016, 1.58144, 2.40657), 0.1);
markers = detector.getMarkers();
assert(markers.Count == 2);
assertPoseNear(markers(1), vernier.Pose(107.308, 327.016, 1.58144, 2.40657), 0.1);

% a stamp in a real image
image = imread('stampPatternImage_15um.jpg');
detector = vernier.StampPatternDetector(15, 'stampPattern.png', 820);
detector.compute(image);
assert(detector.patternFound() && isequal(detector.getMarkerIds(), 0));
assertPoseNear(detector.get2DPose(0), vernier.Pose(392.09, 11.1902, 0.000135, 0.937845), 0.2);
end

function testFactories()
detector = vernier.Detector.newInstance('PeriodicPattern');
assert(isa(detector, 'vernier.PeriodicPatternDetector'));
detector = vernier.Detector.newInstance('StampPattern');
assert(strcmp(class(detector), 'vernier.StampPatternDetector'));
assert(strcmp(detector.getClassname(), 'StampPattern'));
detector = vernier.Detector.newInstance('HPCodePattern');
assert(strcmp(class(detector), 'vernier.HPCodePatternDetector'));
detector = vernier.Detector.newInstance('BitmapPattern');
assert(strcmp(class(detector), 'vernier.BitmapPatternDetector'));

% the megarena detector reads the code size from 'codeSize'
filename = [tempname() '.json'];
file = fopen(filename, 'w');
fprintf(file, '{ "MegarenaPattern": { "period": 14.0, "codeSize": 12, "description": "test pattern", "unit": "um" } }');
fclose(file);
detector = vernier.Detector.loadFromJSON(filename);
delete(filename);
assert(isa(detector, 'vernier.MegarenaPatternDetector'));
assert(detector.getPhysicalPeriod() == 14);
assert(strcmp(detector.getDescription(), 'test pattern') && strcmp(detector.getUnit(), 'um'));
end

function testErrors()
assertError(@() vernier.Detector.newInstance('NoSuchPattern'), 'vernier:error');
assertError(@() vernier.Detector.loadFromJSON('no_such_file.json'), 'vernier:error');
detector = vernier.PeriodicPatternDetector(15);
assertError(@() detector.compute([]), 'vernier:error');
assertError(@() detector.compute(rand(4, 4, 2)), 'vernier:error');
assertError(@() detector.setSigma('a'), 'vernier:error');
assertError(@() vernier.MegarenaPatternDetector(9), 'vernier:error');
assertError(@() vernier.StampPatternDetector(5), 'vernier:error');
assertError(@() vernier.StampPatternDetector(5, 'no_such_file.png', 460), 'vernier:error');
assertError(@() vernier.HPCodePatternDetector(8, 34, 256), 'vernier:error');
% only stamp and HP code detectors have markers
assertError(@() detector.getMarkerIds(), 'vernier:error');
end

function testBackend()
detector = vernier.PeriodicPatternDetector(15);
assert(strcmp(detector.getBackend(), 'CPU'));
detector.setBackend('cpu');
assert(strcmp(detector.getBackend(), 'CPU'));
assertError(@() detector.setBackend('GPU'), 'vernier:error');
available = vernier.cudaAvailable();
assert(islogical(available) && isscalar(available));
detector = vernier.Detector.newInstance('MegarenaPattern', 'CPU');
assert(strcmp(detector.getBackend(), 'CPU'));
if ~available
    % a rejected backend leaves the current one in place
    assertError(@() detector.setBackend('CUDA'), 'vernier:error');
    assert(strcmp(detector.getBackend(), 'CPU'));
    assertError(@() vernier.Detector.newInstance('PeriodicPattern', 'CUDA'), 'vernier:error');
else
    detector.setBackend('CUDA');
    assert(strcmp(detector.getBackend(), 'CUDA'));
end
end

function testUnload()
% the library can be released and loaded again
vernier.Library.unload();
assert(~libisloaded(vernier.Library.alias));
detector = vernier.PeriodicPatternDetector(15);
assert(libisloaded(vernier.Library.alias));
assert(detector.getPhysicalPeriod() == 15);
end

function assertPoseNear(pose, truth, tolerance)
error = abs([pose.x - truth.x, pose.y - truth.y, pose.alpha - truth.alpha]);
assert(all(error < tolerance), 'estimated pose %s too far from %s', pose.toString(), truth.toString());
end

function assertError(f, identifier)
try
    f();
catch err
    assert(strcmp(err.identifier, identifier), 'unexpected error %s: %s', err.identifier, err.message);
    return;
end
error('test:noError', 'an error %s was expected', identifier);
end
