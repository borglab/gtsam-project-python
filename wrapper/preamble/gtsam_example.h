/**
  * This is a type caster for `boost::optional` so we can use optionals in Python.
  * 
  * Example:
  * In C++: void foo(boost::optional<Bar> &opt);
  * In Python:
  *   - foo(None)
  *   - foo(bar_instance)
*/
#include <pybind11/stl.h>

namespace pybind11 { namespace detail {
    template <typename T>
    struct type_caster<boost::optional<T>> : optional_caster<boost::optional<T>> {};
}}
