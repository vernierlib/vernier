#include "Vernier.hpp"
#include <opencv2/imgcodecs.hpp>
#include <fstream>
#include <sstream>

using namespace vernier;
using namespace std;

/** This example compares the two carrier peak searches of the megarena detector:
 * the strongest bin of the spectrum (the default) and the ring search, which
 * scores each peak against the bins at the same distance from the centre
 * (setBool("ringPeakSearch", true)).
 *
 * It reads a list of pictures, each with the megarena's period, from a CSV file
 * whose first row names its columns, among them "file" and "pitch", runs both
 * searches on every picture and writes, per picture and search, the pose and
 * the two phase planes the detector fitted:
 *
 *   comparingPeakSearch <manifest.csv> <picture-dir> <code-size> <out.csv>
 */
int main(int argc, char** argv) {
    if (argc != 5) {
        cerr << "usage: comparingPeakSearch <manifest.csv> <picture-dir> <code-size> <out.csv>" << endl;
        return 2;
    }
    ifstream manifest(argv[1]);
    string directory = argv[2];
    int codeSize = stoi(argv[3]);
    ofstream out(argv[4]);

    string line;
    getline(manifest, line);
    vector<string> columns;
    {
        stringstream header(line);
        string column;
        while (getline(header, column, ',')) {
            columns.push_back(column);
        }
    }
    auto columnIndex = [&](const string& name) {
        return (int) (find(columns.begin(), columns.end(), name) - columns.begin());
    };
    int fileColumn = columnIndex("file");
    int pitchColumn = columnIndex("pitch");

    // One detector per search, kept from picture to picture so the FFT plans
    // are made once.
    MegarenaPatternDetector maximum(1.0, codeSize), ring(1.0, codeSize);
    ring.setBool("ringPeakSearch", true);

    out << "file,search,found,x,y,alpha,pixel_size,plane1_a,plane1_b,plane2_a,plane2_b,"
        << "peak1_col,peak1_row,peak2_col,peak2_row,quality1,quality2" << endl;
    int count = 0;
    while (getline(manifest, line)) {
        stringstream row(line);
        vector<string> fields;
        string field;
        while (getline(row, field, ',')) {
            fields.push_back(field);
        }
        string file = fields[fileColumn];
        double pitch = stod(fields[pitchColumn]);

        cv::Mat image = cv::imread(directory + "/" + file, cv::IMREAD_UNCHANGED);
        if (image.empty()) {
            cerr << "cannot read " << file << endl;
            continue;
        }
        for (auto search : {make_pair(string("maximum"), &maximum), make_pair(string("ring"), &ring)}) {
            MegarenaPatternDetector& detector = *search.second;
            detector.setDouble("physicalPeriod", pitch);
            detector.compute(image);
            Pose pose = detector.get2DPose();
            PhasePlane plane1 = detector.getPlane1(), plane2 = detector.getPlane2();
            PatternPhase* phase = (PatternPhase*) detector.getObject("patternPhase");
            Eigen::Vector3d peak1 = phase->getPeak1(), peak2 = phase->getPeak2();
            out << file << "," << search.first << "," << detector.patternFound() << ","
                << pose.x << "," << pose.y << "," << pose.alpha << "," << pose.pixelSize << ","
                << plane1.getA() << "," << plane1.getB() << ","
                << plane2.getA() << "," << plane2.getB() << ","
                << peak1.x() << "," << peak1.y() << "," << peak2.x() << "," << peak2.y() << ","
                << detector.getDouble("quality1") << "," << detector.getDouble("quality2") << endl;
        }
        if (++count % 50 == 0) {
            cerr << count << " pictures" << endl;
        }
    }
    return 0;
}
