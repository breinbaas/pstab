// #include <pybind11/pybind11.h> // should be added once we start working on the python part
// #include "rapidjson/document.h" // should be added once we start working on the python part
#include "include/rapidjson/document.h" // ugly temp fix (include/) to allow development in cpp folder
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include <future>

#include <chrono> // temporay to allow performance measurements

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

using namespace std;
using namespace rapidjson;

// These are the default number of circles in x direction, z direction and
// the number of tangent lines, they should have a value of at least 2
const int NUM_X = 2; // at least 2
const int NUM_Z = 2;
const int NUM_T = 2;

// Structures to save the data
// Soil information
struct Soil
{
    string code;
    string color;
    double y_dry;
    double y_sat;
    double cohesion;
    double friction_angle;
};

// Point information
struct Point
{
    double x;
    double z;
};

// The soilpolygons from the model
struct SoilPolygon
{
    vector<Point> points;
    string soilcode;
};

// The xz coordinates of the phreatic line
struct PhreaticLine
{
    vector<Point> points;
};

// The definition of the Bishop search grid
struct BishopSearchGrid
{
    double left;
    double bottom;
    double width;
    double height;
    double tangents_top;
    double tangents_bottom;
    double minimum_slip_plane_length;
};

// One model to rule them all
struct BishopModel
{
    vector<Soil> soils;
    vector<SoilPolygon> soil_polygons;
    BishopSearchGrid bishop_search_grid;
    PhreaticLine phreatic_line;
};

/*
This function will calculate the Bishop safety factor for the given
model, centerpoint of the slope circle and z coordinate of the tangent line
*/
double sf_bishop(const int i, const BishopModel &model, double mx, double mz, double z_tangent, double *sf)
{
    double r = mz - z_tangent;
    cout << "starting thread" << i << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "ending thread" << i << endl;
    *sf = 1.0;
}

/*
This function will parse a json string from the typical leveelogic calculation model
and create a BishopModel structure to pass to the actual calculation
*/
BishopModel parse_bishop_model(const string &json)
{
    // create a document using rapidjson
    Document document;
    document.Parse(json.c_str());
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
    double left = v_bishop_search_grid["x_left"].GetDouble();
    double bottom = v_bishop_search_grid["z_bottom"].GetDouble();
    double width = v_bishop_search_grid["width"].GetDouble();
    double height = v_bishop_search_grid["height"].GetDouble();
    double tangents_top = v_bishop_search_grid["tangents_top"].GetDouble();
    double tangents_bottom = v_bishop_search_grid["tangents_bottom"].GetDouble();
    double minimum_slip_plane_length = v_bishop_search_grid["minimum_slip_plane_length"].GetDouble();

    BishopSearchGrid bishop_search_grid = BishopSearchGrid{
        left,
        bottom,
        width,
        height,
        tangents_top,
        tangents_bottom,
        minimum_slip_plane_length,
    };

    return BishopModel{
        soils,
        soilpolygons,
        bishop_search_grid,
        phreatic_line,
    };
}

vector<double> calculate_bishop() // will become calculate_bishop(const string &json)
{
    // for now we skip the given string and read a test file
    ifstream file("./test/model.json");
    stringstream buffer{};
    buffer << file.rdbuf();
    string json = buffer.str();

    // get the model from the string
    BishopModel model = parse_bishop_model(json);

    double x = model.bishop_search_grid.left;
    double z = model.bishop_search_grid.bottom;
    double dx = model.bishop_search_grid.width / double(NUM_X - 1);
    double dz = model.bishop_search_grid.height / double(NUM_Z - 1);
    double dt = (model.bishop_search_grid.tangents_top - model.bishop_search_grid.tangents_bottom) / double(NUM_T - 1);

    // temporary code to measure performance
    auto start = std::chrono::high_resolution_clock::now();

    // iterate over the possible slope circle locations, multithreaded
    array<thread, NUM_T * NUM_X * NUM_Z> threads;
    array<double, NUM_T * NUM_X * NUM_Z> sfs;
    int i = 0;
    for (int nx = 0; nx < NUM_X; ++nx)
    {
        for (int nz = 0; nz < NUM_Z; ++nz)
        {
            for (int nt = 0; nt < NUM_T; ++nt)
            {
                double x = model.bishop_search_grid.left + nx * dx;
                double z = model.bishop_search_grid.bottom + nz * dz;
                double t = model.bishop_search_grid.tangents_bottom + nt * dt;

                threads[i] = thread(sf_bishop, i, model, x, z, t, &sfs[i]);

                ++i;
            }
        }
    }

    for (auto &t : threads)
    {
        t.join();
    }

    // temporary code to measure performance
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Elapsed time " << elapsed.count() << " ms\n";

    for (auto &sf : sfs)
    {
        std::cout << "sf =" << sf << endl;
    }

    return {};
}

int main()
{
    vector<double> sfs = calculate_bishop(); // will become calculate_bishop("jsonstring");
}

// THE NEXT CODE IS COMMENTED SINCE I USE THIS AS A CPP PROJECT FIRST
// ONCE THIS IS DONE IT WILL BE PART OF THE PYTHON LIBRARY CODE
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
