#ifndef Stl_Helper_Fuctions
#define Stl_Helper_Fuctions
#include <iostream>

namespace mystd
{
  //
  // Nice print tool for arbitrary stl templated containers
  // - cf
  //   * http://www.java2s.com/Tutorial/Cpp/0580__STL-Algorithms-Min-Max/Usemaxelementwithcriteriafunction.htm
  //   * http://pages.cs.wisc.edu/~driscoll/typename.html
  //

  template <class T>
    inline void print_elements (const T& coll, const char* optcstr="")
    {
      typename T::const_iterator pos;

      std::cout << optcstr;
      for (pos=coll.begin(); pos!=coll.end(); ++pos) {
	std::cout << *pos << ' ' << std::endl;
      }
    }

} // mystl

#endif //Stl_Helper_Fuctions
