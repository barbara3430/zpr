#include <boost/python.hpp>
#include <vector>
#include <iostream>
#include <Python.h>

class World {
public:
    void test(boost::python::list& ns) {
        for (int i = 0; i < len(ns); ++i)
            std::cout << boost::python::extract<double>(ns[i]) << std::endl;
    }
};

BOOST_PYTHON_MODULE(world) {
    boost::python::class_<World>("World")
     .def("test", &World::test);
}

