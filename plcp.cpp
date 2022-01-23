#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

struct coords {
    double x;
    double y;
};  

vector<string> split(string str, char delimiter) {
    vector <string> splited_strings(2);
    splited_strings[0] = str.substr(0, str.find(delimiter));
    splited_strings[1] = str.substr(str.find(delimiter)+1, str.length());
    return splited_strings;
}

vector<coords> read(string filename) {
    vector<coords>v;
    ifstream file(filename);
    if (!file.is_open()){
        throw runtime_error("Could not open file");
    }

    string row;
    while (getline(file, row)) {
        int pos;
        while (row.find(',') != string::npos) {
            pos = row.find(',');
            row[pos] = '.';
        }
        v.push_back(
            coords() = {
                stod(split(row, ';')[0]),
                stod(split(row, ';')[1])
            });
    }
    file.close();
    return v;
}

int main(int argc, char** argv)
{
    string datapath;
    string filename;
    cout << "Enter path to data: ";
    getline(cin, datapath);
    cout << "Path to data: " << datapath << endl;
    cout << "Enter output file name: ";
    getline(cin, filename);
    vector<coords>data = read(datapath);
    
    // for (auto el : data) {
    //     cout << el.x << "\t" << el.y << endl;
    // }

    const int radius = 30;
    coords sum = {0, 0};
    Mat image = imread("template.png", IMREAD_COLOR);
    for (int i = 0; i < data.size(); i++) {
        int xpos = (data[i].x * 930) + 1002 - radius / 2;
        int ypos = (-data[i].y * 930) + 1080 - radius / 2;
        sum.x += xpos;
        sum.y += ypos;
        circle(image, Point(xpos, ypos), radius + 2, Scalar(0, 0, 0, 255), -radius + 2, LINE_AA);
        circle(image, Point(xpos, ypos), radius, Scalar(0, 0, 255, 255), -radius, LINE_AA);
    }
    circle(image, Point(sum.x / data.size(), sum.y / data.size()), radius + 2, Scalar(0, 0, 0, 255), -radius + 2, LINE_AA);
    circle(image, Point(sum.x / data.size(), sum.y / data.size()), radius, Scalar(255, 0, 0, 255), -radius, LINE_AA);
    imwrite(filename.append(".jpg"), image, vector<int>{IMWRITE_JPEG_QUALITY, 100});
    getchar();
    return 0;
}