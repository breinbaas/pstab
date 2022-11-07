#include <pybind11/pybind11.h>
#include "rapidjson/document.h"
#include <string>
#include <fstream>
#include <sstream>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

// using namespace std;
// using namespace rapidjson;

double bishop(const std::string &model)
{
    // for now read a test file
    std::ifstream file("./model.json");
    std::stringstream buffer{};
    buffer << file.rdbuf();

    // create a document using rapidjson
    rapidjson::Document document;
    document.Parse(buffer.str().c_str());

    return 0.0;
}

namespace py = pybind11;

PYBIND11_MODULE(pstab, m)
{
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: pstab

        .. autosummary::
           :toctree: _generate

           bishop           
    )pbdoc";

    m.def("bishop", &bishop, R"pbdoc(
        Calculate the Bishop safety factor from the given model

        Some other explanation about the bishop function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
