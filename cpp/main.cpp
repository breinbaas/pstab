// #include <pybind11/pybind11.h>
#include "include/rapidjson/document.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

using namespace std;
using namespace rapidjson;

struct Soil
{
    string code;
    string color;
    double y_dry;
    double y_sat;
    double cohesion;
    double friction_angle;
};

struct Point
{
    double x;
    double z;
};

struct SoilPolygon
{
    vector<Point> points;
    string soilcode;
};

struct PhreaticLine
{
    vector<Point> points;
};

struct BishopSearchGrid
{
    float x_left;
    float z_bottom;
    float width;
    float height;
    float tangents_top;
    float tangents_bottom;
    float minimum_slip_plane_length;
};

double bishop(const string &model)
{
    // for now read a test file
    ifstream file("./test/model.json");
    stringstream buffer{};
    buffer << file.rdbuf();

    // create a document using rapidjson
    Document document;
    string json = buffer.str();
    document.Parse(buffer.str().c_str()); // will become: document.Parse(model.c_str());
    assert(document.IsObject());

    // PARSE SOILCOLLECTION
    vector<Soil> soils = {};
    Value &v_soilcollection = document["soilcollection"]["soils"];
    assert(v_soilcollection.IsArray());
    for (SizeType i = 0; i < v_soilcollection.Size(); i++)
    {
        soils.push_back(Soil{
            v_soilcollection[i]["code"].GetString(),
            v_soilcollection[i]["color"].GetString(),
            v_soilcollection[i]["y_dry"].GetDouble(),
            v_soilcollection[i]["y_sat"].GetDouble(),
            v_soilcollection[i]["cohesion"].GetDouble(),
            v_soilcollection[i]["friction_angle"].GetDouble(),
        });
    }

    // PARSE SOILPROFILE 2
    vector<SoilPolygon> soilpolygons = {};
    Value &v_soilprofile2 = document["soilprofile2"]["soilpolygons"];
    assert(v_soilprofile2.IsArray());

    for (SizeType i = 0; i < v_soilprofile2.Size(); i++)
    {
        Value &v_points = v_soilprofile2[i]["points"];
        vector<Point> points;
        for (SizeType i = 0; i < v_points.Size(); i++)
        {
            double x = v_points[i][0].GetDouble();
            double z = v_points[i][1].GetDouble();
            points.push_back(Point{x, z});
        }
        string soilcode = v_soilprofile2[i]["soilcode"].GetString();

        soilpolygons.push_back(SoilPolygon{points, soilcode});
    }

    // PARSE PHREATIC LINE
    vector<Point> points = {};
    Value &v_phreatic_line = document["phreatic_line"];
    assert(v_phreatic_line.IsArray());
    for (SizeType i = 0; i < v_phreatic_line.Size(); i++)
    {
        double x = v_phreatic_line[i][0].GetDouble();
        double z = v_phreatic_line[i][1].GetDouble();
        points.push_back(Point{x, z});
    }
    PhreaticLine phreatic_line = PhreaticLine{points};

    // PARSE BISHOP SEARCH GRID
    Value &v_bishop_search_grid = document["bishop_search_grid"];
    float x_left = v_bishop_search_grid["x_left"].GetDouble();
    float z_bottom = v_bishop_search_grid["z_bottom"].GetDouble();
    float width = v_bishop_search_grid["width"].GetDouble();
    float height = v_bishop_search_grid["height"].GetDouble();
    float tangents_top = v_bishop_search_grid["tangents_top"].GetDouble();
    float tangents_bottom = v_bishop_search_grid["tangents_bottom"].GetDouble();
    float minimum_slip_plane_length = v_bishop_search_grid["minimum_slip_plane_length"].GetDouble();

    BishopSearchGrid bishop_search_grid = BishopSearchGrid{
        x_left,
        z_bottom,
        width,
        height,
        tangents_top,
        tangents_bottom,
        minimum_slip_plane_length,
    };

    return 0.0;
}

int main()
{
    cout << bishop(" ");
}

// namespace py = pybind11;

// PYBIND11_MODULE(pstab, m)
// {
//     m.doc() = R"pbdoc(
//         Pybind11 example plugin
//         -----------------------

//         .. currentmodule:: pstab

//         .. autosummary::
//            :toctree: _generate

//            bishop
//     )pbdoc";

//     m.def("bishop", &bishop, R"pbdoc(
//         Calculate the Bishop safety factor from the given model

//         Some other explanation about the bishop function.
//     )pbdoc");

// #ifdef VERSION_INFO
//     m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
// #else
//     m.attr("__version__") = "dev";
// #endif
// }
