
# The curious case of the zip in the night-time

Python has a [zip function](https://www.w3schools.com/python/ref_func_zip.asp)  that takes two lists and produces an iterator for walking over the list of pairs from both lists.

In the python interpreter that looks as follows:. 

```
>>> a=[1,2,3]
>>> b=['a','b','c','d']
>>> c=zip(a,b)
>>> print(type(c))
<class 'zip'>
>>> for elm in c:
...   print(elm)
...
(1, 'a')
(2, 'b')
(3, 'c')
```

Actually I think this goodie comes from [Haskell](https://hoogle.haskell.org/?hoogle=zip),  and Haskell was inspired by [Lisp](https://jtra.cz/stuff/lisp/sclr/mapcar.html). 

The python solution returns a kind of iterator. In Haskell the zip function does not produce an iterator, it returns a list. This is not as wasteful as it seems: the resulting list is computed lazily (only when values are read from it). In Lisp they have mapcar, and that is similar to a C++ algorithm: you need to pass it a function that combines a pair taken from both of the list into a value in the result list.

The interesting question is: why isn't there an equivalent function in the C++ standard library? To find out I made my own library that does this functionality.

## The zipit library

The library is a header only library, it lives in header file [zipit.h](https://github.com/MoserMichael/zipit/blob/master/inc/zipit.h) ; and in it's own namespace zipit. You can use it with c++17, as it uses std::optional in one case.

You can make a forward iterator to walk over a combined pair from two lists; the make function in namespace zipit takes two iterators and produces one for returning pairs whose elements come from both of the iterators.

```
    #include <zipit.h>

    ...

    std::vector<int> vecA = { 1, 2, 3};
    std::vector<char> vecB = { 'a', 'b', 'c' };

    for(auto it=zipit::make(vecA.begin(),vecB.begin()); it != zipit::make(vecA.end(), vecB.end()); ++it) {\
        printf("iter: first %d second %c\n", (*it).first, (*it).second);
    }

```
The output is:
```
iter: first 1 second a
iter: first 2 second b
iter: first 3 second c
```

There is a more amazing iterator that accepts an arbitrary number of iterators and returns a tuple of values (it makes use of template meta programming)

```
    #include <zipit.h>

    ...
    std::vector<int> vecA = { 1, 2, 3};
    std::vector<char> vecB = { 'a', 'b', 'c', 'd', 'e', 'f' };
    std::vector<float> vecC = { 0.57721, 1.1, 2.2, 3.1415 };

    size_t len = 0;
    for(auto it = zipit::maken(vecA.begin(),vecB.begin(),vecC.begin()); it != zipit::maken(vecA.end(), vecB.end(), vecC.end()); ++it) {
        printf("itern: first %d second %c third %f\n", std::get<0>(*it), std::get<1>(*it), std::get<2>(*it) );
        ++len;
    }

```

The output is:

```
itern: first 1 second a third 0.577210
itern: first 2 second b third 1.100000
itern: first 3 second c third 2.200000
```



You have an algorithm similar to std::for_each that does the same as python zip:

```
    #include <zipit.h>

    ...

    std::vector<int> vecA = { 1, 2, 3};
    std::vector<char> vecB = { 'a', 'b', 'c' };

    struct func {

        void operator() (std::pair<int,char> arg) const {
            printf("foreach: first %d second %c\n", arg.first, arg.second);
        }
    };

    func showIt;


    zipit::for_each_zip( vecA.begin(), vecA.end(), vecB.begin(), vecB.end(), showIt );
```

You have the same algorithm - but with the variant to pass the function object by reference, (by default std::algorithm is passing all argument objects by value, so you can't accumulate any values in your callback object)

```
    #include <zipit.h>

    ...

    std::vector<int> vecA = { 1, 2, 3};
    std::vector<char> vecB = { 'a', 'b', 'c' };

    struct func {

        func() : count_(0) {
        }    

        void operator() (std::pair<int,char> arg) const {
            printf("foreach: first %d second %c\n", arg.first, arg.second);
            count_ += 1;
        }
        mutable size_t count_;
    };

    zipit::for_each_zip_ref_func( vecA.begin(), vecA.end(), vecB.begin(), vecB.end(), showIt );
```

You can have an algorithm that does the equivalent of python's [ziplongest](https://www.geeksforgeeks.org/python-itertools-zip_longest/) . Please note that the function object is receiving a pair of optional objects, the optional is empty if it's iterator type has reached the end of iteration.

```

    #include <zipit.h>

    ...

    std::vector<int> vecA = { 1, 2, 3};
    std::vector<char> vecB = { 'a', 'b', 'c' };

    struct func {

        void operator() (std::pair< std::optional<int>, std::optional<char> > &arg) const {

            if (arg.first.has_value()) {
                printf("first %d ", arg.first.value());
             }
            if (arg.second.has_value()) {
                printf("second %c", arg.second.value());
            }
            printf("\n");
        }

    };

    func showIt;

    for_each_zip_longest( vecA.begin(), vecA.end(), vecB.begin(), vecB.end(), showIt );

```

You have the variant for_each_zip_longest_ref_func that again, does the same but by passing the function by reference.


## Bugs and limitations.

1. The iterator has the operator* member - meaning that this operator returns a copy of a std::pair object, it can't return a pointer or a reference. That's because it returns a compound object, it can't return a pointer or a reference to a pair that sits on the stack, as this object is no longer valid once we return from the operator. One could of course keep a member pair and in the iterator and return a pointer/reference to it, now the problem with that is that this object gets overwritten when the iterator advances to a different position; that would be inconvenient if someone still holds the pointer returned by a previous call.
2. You can't have an iterator for an equivalent of ziplongest - it is impossible to test if the iteration has finished; that's because the interface of iterator doesn't know if it has reached the end of it's sequence.. 
3. Actually all this is old news, c++20 has ranges, so there is a new way of iterating over stuff, a whole new world of mind boggling innovations is coming, and it's just around the corner...





