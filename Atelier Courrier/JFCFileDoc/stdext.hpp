#ifndef __STD_EXT_LIB_H__
#define __STD_EXT_LIB_H__

/*
 * fichier : stdext.hpp
 *  
 * Extensions de la librairie standard du C++ sous la forme :
 * 
 *  -  d'algorithmes C++ STL fonctionnant avec toues les formes de containers.
 * 
 *  -  de function objects not in <functional> permettant de faire des
 *		opérations logiques ou bit à bit.
 * 
 *  -  de function adapters complètement compatibles avec ceux définis dans
 *      le header <functional>. Ces adapters permettent de composer des
 *      function objects pour réaliser par exemple les opération suivantes : 
 *      
 *      # compose_f_gx      : process: op1(op2(x))
 *      # compose_f_gx_hx   : process: op1 (op2(x), op3(x))
 *      # compose_f_gx_hy   : process: op1 (op2(x) ,op3(y))
 *
 *  Aurélien RAINONE  
 */

//=============================================================================	
//                Helper functions pour la libraire standard 
//=============================================================================	

#include <iostream>

/* PRINT_ELEMENTS()
* - prints optional C-string optcstr followed by
* - all elements of the collection coll
* - separated by spaces
*/
template <class T>
inline void PRINT_ELEMENTS (const T& coll, const char* optcstr="")
{
   typename T::const_iterator pos;
   std::cout << optcstr;
   for (pos=coll.begin(); pos!=coll.end(); ++pos) {
       std::cout << *pos << ' ';
   }
   std::cout << std::endl;
}


//=============================================================================	
//=============================================================================	
//									Algorithms
//=============================================================================	
//=============================================================================	


//=============================================================================	
template<class InputIterator,
		class OutputIterator, 
		class Predicate> inline
	OutputIterator 
copy_if(InputIterator first, InputIterator last, OutputIterator	result, Predicate pred)
//=============================================================================	
{
	for (; first != last; ++first)
	{
		if( pred(*first))
			*result++ = *first;
	}
	return result;
}

//=============================================================================	
//Performs f() on any element elem in [first, last) where pred(elem) is true
template<class InputIterator,
		class Predicate,
		class UnaryFunction> inline
	UnaryFunction
for_each_if(InputIterator first, InputIterator last, Predicate pred, UnaryFunction f)
//=============================================================================	
{
	for(;first != last; ++first)
	{
		if (pred(*first))
			f(*first);
	}
	return f;
}


//=============================================================================	
//=============================================================================	
//					Logical and Bitwise function objects
//=============================================================================	
//	taken from : http://www.boost.org/doc/libs/1_36_0/boost/mpi/operations.hpp
//=============================================================================	


///============================================================================	
///  @brief Compute the maximum of two values.
///
/// This binary function object computes the maximum of the two values
/// it is given.
///
///============================================================================	
template<typename T>
struct maximum : public std::binary_function<T, T, T>
{
	/** @returns the maximum of x and y. */
	const T& operator()(const T& x, const T& y) const
	{
		return x < y? y : x;
	}
};

///============================================================================	
///  @brief Compute the minimum of two values.
///
/// This binary function object computes the minimum of the two values
/// it is given.
///
///============================================================================	
template<typename T>
struct minimum : public std::binary_function<T, T, T>
{
	/** @returns the minimum of x and y. */
	const T& operator()(const T& x, const T& y) const
	{
		return x < y? x : y;
	}
};

///============================================================================	
///  @brief Compute the bitwise AND of two integral values.
///
/// This binary function object computes the bitwise AND of the two
/// the two values it is given.
///
///============================================================================	
template<typename T>
struct bitwise_and : public std::binary_function<T, T, T>
{
	/** @returns @c x & y. */
	T operator()(const T& x, const T& y) const
	{
		return x & y;
	}
};

///============================================================================	
///  @brief Compute the bitwise OR of two integral values.
///
/// This binary function object computes the bitwise OR of the two
/// the two values it is given.
///
///============================================================================	
template<typename T>
struct bitwise_or : public std::binary_function<T, T, T>
{
	/** @returns the @c x | y. */
	T operator()(const T& x, const T& y) const
	{
		return x | y;
	}
};

///============================================================================	
///  @brief Compute the logical exclusive OR of two integral values.
///
/// This binary function object computes the logical exclusive of the
/// the two values it is given.
///
///============================================================================	
template<typename T>
struct logical_xor : public std::binary_function<T, T, T>
{
	/** @returns the logical exclusive OR of x and y. */
	T operator()(const T& x, const T& y) const
	{
		return (x || y) && !(x && y);
	}
};

///============================================================================	
///  @brief Compute the bitwise exclusive OR of two integral values.
///
/// This binary function object computes the bitwise exclusive OR of
/// the two values it is given.
///
///============================================================================	
template<typename T>
struct bitwise_xor : public std::binary_function<T, T, T>
{
	/** @returns @c x ^ y. */
	T operator()(const T& x, const T& y) const
	{
		return x ^ y;
	}
};


//=============================================================================	
//=============================================================================	
//                           Function Adapters
//=============================================================================	
//=============================================================================	

//=============================================================================	
//                                      Unary Compose Function Object Adapters
//=============================================================================	

#include <functional>

//=============================================================================	
// class for the compose_f_gx adapter,
// used for nested computations, ie : op1(op2(x))
//=============================================================================	
template <class OP1,
        class OP2>
    class compose_f_gx_t
    : public std::unary_function<typename OP2::argument_type,
                                typename OP1::result_type>
{
    private:
        OP1 op1;
        OP2 op2;
        
    public:
        // constructor
        compose_f_gx_t(const OP1& o1, const OP2& o2)
        : op1(o1), op2(o2) {}
        
        // function call
        typename OP1::result_type
        operator() (const typename OP2::argument_type& x) const {
            return op1 (op2(x));
        }
};


//=============================================================================	
// convenience functions for the compose_f_gx adapter
//=============================================================================	
template<class OP1,
        class OP2> inline
    compose_f_gx_t<OP1,OP2>
compose_f_gx (const OP1& op1, const OP2& op2)
{
   return compose_f_gx_t<OP1,OP2>(op1, op2);
}

//=============================================================================	
// example for the compose_f_gx adapter
//=============================================================================	
/*
   #include <iostream>
   #include <vector>
   #include <algorithm>
   #include <functional>
   #include <stdext.hpp>
   
   using namespace std;

   int main()
   {
       vector<int> coll;

       // insert elements from 1 to 9
       for (int i=1; i<=9; ++i) {
           coll.push_back(i);
       }
       PRINT_ELEMENTS(coll);

       //for each element add 10 and multiply by 5
       transform (coll.begin(),coll.end(),
                  ostream_iterator<int>(cout," "),
                  compose_f_gx (bind2nd (multiplies<int>(),5),
                                bind2nd (plus<int>(),10)));
       cout << endl;
   }
*/


//=============================================================================	
// class for the compose_f_gx_hx adapter,
// used to combine two criterias, ie : op1(op2(x),op3(x))
//=============================================================================	
template<class OP1,
        class OP2,
        class OP3>
    class compose_f_gx_hx_t
    : public std::unary_function<typename OP2::argument_type,
                                typename OP1::result_type>
{
    private:
        OP1 op1;
        OP2 op2;
        OP3 op3;
        
    public:
        // constructor
        compose_f_gx_hx_t (const OP1& o1, const OP2& o2, const OP3& o3)
        : op1(o1), op2(o2), op3(o3) {}
        
        // function call
        typename OP1::result_type
        operator()(const typename OP2::argument_type& x) const {
            return op1(op2(x),op3(x));
        }
};


//=============================================================================	
// convenience functions for the compose_f_gx_hx adapter
//=============================================================================	
template <class OP1,
        class OP2,
        class OP3> inline
    compose_f_gx_hx_t<OP1, OP2, OP3>
compose_f_gx_hx (const OP1& op1, const OP2& op2, const OP3& op3)
{
    return compose_f_gx_hx_t<OP1,OP2,OP3>(op1,op2,op3);
}


//=============================================================================	
// example for the compose_f_gx_hx adapter
//=============================================================================	
/*
   #include <iostream>
   #include <vector>
   #include <algorithm>
   #include <functional>
   #include <stdext.hpp>

   using namespace std;

   int main()
   {
       vector<int> coll;

	   // insert elements from 1 to 9
	   for (int i=1; i<=9; ++i)
	   {
		coll.push_back(i);
	   }
	   PRINT_ELEMENTS(coll);

	   // remove all elements that are greater than four and less than seven
	   // - retain new end
	   vector<int>::iterator pos;
	   pos = remove_if (coll.begin(), coll.end(), compose_f_gx_hx(logical_and<bool>(),
													   bind2nd(greater<int>(),4),
													   bind2nd(less<int>(),7)));
	   // remove "removed" elements in coll
	   coll.erase(pos,coll.end());

	   PRINT_ELEMENTS(coll);
   }
*/

//=============================================================================	
//                                      Binary Compose Function Object Adapters
//=============================================================================	

//=============================================================================	
// class for the compose_f_gx_hy adapter,
// used to compose adapters that process the result of two different unary
// operations , ie : op1(op2(x),op3(y))
//=============================================================================	
template<class OP1,
        class OP2,
        class OP3>
    class compose_f_gx_hy_t
    : public std::binary_function<typename OP2::argument_type,
                                typename OP3::argument_type,
                                typename OP1::result_type>
{
    private:
        OP1 op1;
        OP2 op2;
        OP3 op3;
    
    public:
        // constructor
        compose_f_gx_hy_t (const OP1& o1, const OP2& o2, const OP3& o3)
        : op1(o1), op2(o2), op3(o3) {}

        // function call
        typename OP1::result_type
            operator()(const typename OP2::argument_type& x,
                        const typename OP3::argument_type& y) const {
            return op1(op2(x),op3(y));
        }
};


//=============================================================================	
// convenience functions for the compose_f_gx_hy adapter
//=============================================================================	
template<class OP1,
        class OP2,
        class OP3> inline
    compose_f_gx_hy_t <OP1, OP2, OP3>
compose_f_gx_hy (const OP1& op1, const OP2& op2, const OP3& op3)
{
    return compose_f_gx_hy_t<OP1, OP2, OP3>(op1, op2, op3);
}


//=============================================================================	
// example for the compose_f_gx_hy adapter
//=============================================================================	
/*
    #include <iostream>
    #include <algorithm>
    #include <functional>
    #include <string>
    #include <stdext.hpp>

    using namespace std;

   int main()
   {
       string s("Internationalization");
       string sub("Nation");

       //search substring case insensitive
       string::iterator pos;
       pos = search (s.begin(), s.end(),         //string to search in
                     sub.begin() , sub.end() ,      //substring to search
                     compose_f_gx_hy(equal_to<int>(), //compar. criterion
                                      ptr_fun(toupper),
                                      ptr_fun(toupper)));

       if (pos != s.end()) {
           cout << "\"" << sub << "\" is part of \"" << s << "\""
                << end1;
       }
   }
*/


#endif // !__STD_EXT_LIB_H__
