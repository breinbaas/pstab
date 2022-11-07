#include <pybind11/pybind11.h>
#include <clipper2/clipper.core.h>
#include <rapidjson/document.h>
#include <string>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

using namespace std;

double bishop(const string &model)
{
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
