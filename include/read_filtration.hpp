#ifndef READ_FILTRATION_HPP
#define READ_FILTRATION_HPP
#include <vector>
#include <set>

namespace std {
    template<typename _Tp, typename _Alloc> class vector;
    template <typename _Key, typename _Compare, typename _Alloc> class set;
};

typedef std::set<int> vertices;


struct simplex{
  int dim;
  float val;
  vertices vert;
};


std::vector<simplex> read_filtration(std::string name)

#endif // READ_FILTRATION_HPP
;