#ifndef __ZIPIT_H__
#define __ZIPIT_H__

#include <utility>
#include <iterator>
#include <optional>

namespace zipit {

template<typename ItFirst, typename ItSecond>
class Zip_iterator {
public:
    using elm_type = std::pair< typename std::iterator_traits<ItFirst>::value_type,  typename std::iterator_traits<ItSecond>::value_type >; 

    using difference_type = long;
    using value_type = elm_type;
    using pointer = elm_type*;
    using reference = elm_type&;
    using iterator_category = std::forward_iterator_tag;

    Zip_iterator(ItFirst first, ItSecond second) : first_(first), second_(second) {
    }

    Zip_iterator(const Zip_iterator &arg) : first_(arg.first_), second_(arg.second_) {
    }

    Zip_iterator& operator++() { 
        ++first_; 
        ++second_; 
        return *this;
    }
    
    Zip_iterator operator++(int) {
        Zip_iterator retval = *this; 
        ++(*this); 
        return retval;
    }
    
    bool operator==(Zip_iterator other) const {
        return first_ == other.first_ || second_ == other.second_;
    }

    bool operator!=(Zip_iterator other) const {
        return !(*this == other);
    }
    
    value_type operator*() { 
        return elm_type( *first_, *second_ );
    }   
    
private:    
    ItFirst first_;
    ItSecond second_;
};


template<typename ItFirst, typename ItSecond>
Zip_iterator<ItFirst,ItSecond> make(ItFirst first, ItSecond second) { 
    return Zip_iterator(first, second); 
}

template<typename InputItFirst, typename InputItSecond, typename Function>
void for_each_zip(InputItFirst begin_first, InputItFirst end_first, InputItSecond begin_sec, InputItSecond end_sec, Function func) {
  
    using OptFirstArg = typename std::iterator_traits<InputItFirst>::value_type; 
    using OptSecondArg = typename std::iterator_traits<InputItSecond>::value_type; 
    using OptArgPair = std::pair< OptFirstArg, OptSecondArg >;  

    while( begin_first !=  end_first && begin_sec != end_sec) {
            
        OptArgPair arg(*begin_first, *begin_sec);

        func( arg );

        ++begin_first;
        ++begin_sec;
    }
}

template<typename InputItFirst, typename InputItSecond, typename Function>
void for_each_zip_ref_func(InputItFirst begin_first, InputItFirst end_first, InputItSecond begin_sec, InputItSecond end_sec, Function &func) {
  
    using OptFirstArg = typename std::iterator_traits<InputItFirst>::value_type; 
    using OptSecondArg = typename std::iterator_traits<InputItSecond>::value_type; 
    using OptArgPair = std::pair< OptFirstArg, OptSecondArg >;  

    while( begin_first !=  end_first && begin_sec != end_sec) {
            
        OptArgPair arg(*begin_first, *begin_sec);

        func( arg );

        ++begin_first;
        ++begin_sec;
    }
}


template<typename InputItFirst, typename InputItSecond, typename Function>
void for_each_zip_longest(InputItFirst begin_first, InputItFirst end_first, InputItSecond begin_sec, InputItSecond end_sec, Function func) {

    using OptFirstArg = std::optional< typename std::iterator_traits<InputItFirst>::value_type >; 
    using OptSecondArg = std::optional< typename std::iterator_traits<InputItSecond>::value_type >; 
    using OptArgPair = std::pair< OptFirstArg, OptSecondArg >;  

    while( begin_first !=  end_first && begin_sec != end_sec) {
        OptFirstArg first_arg;
        first_arg.emplace( *begin_first );

        OptSecondArg second_arg;
        second_arg.emplace( *begin_sec );

        OptArgPair val( first_arg, second_arg );   

        func( val );

        ++begin_first;
        ++begin_sec;
    }


    while( begin_first !=  end_first ) {

        OptFirstArg first_arg;
        first_arg.emplace( *begin_first );

        OptSecondArg second_arg;

        OptArgPair val( first_arg, second_arg );   

        func( val );

        ++begin_first;
    }

    while( begin_sec != end_sec) {

        OptFirstArg first_arg;

        OptSecondArg second_arg;
        second_arg.emplace( *begin_sec );

        OptArgPair val( first_arg, second_arg );   

        func( val );

        ++begin_sec;
    }
}


template<typename InputItFirst, typename InputItSecond, typename Function>
void for_each_zip_longest_ref_func(InputItFirst begin_first, InputItFirst end_first, InputItSecond begin_sec, InputItSecond end_sec, Function &func) {

    using OptFirstArg = std::optional< typename std::iterator_traits<InputItFirst>::value_type >; 
    using OptSecondArg = std::optional< typename std::iterator_traits<InputItSecond>::value_type >; 
    using OptArgPair = std::pair< OptFirstArg, OptSecondArg >;  

    while( begin_first !=  end_first && begin_sec != end_sec) {
        OptFirstArg first_arg;
        first_arg.emplace( *begin_first );

        OptSecondArg second_arg;
        second_arg.emplace( *begin_sec );

        OptArgPair val( first_arg, second_arg );   

        func( val );

        ++begin_first;
        ++begin_sec;
    }


    while( begin_first !=  end_first ) {

        OptFirstArg first_arg;
        first_arg.emplace( *begin_first );

        OptSecondArg second_arg;

        OptArgPair val( first_arg, second_arg );   

        func( val );

        ++begin_first;
    }

    while( begin_sec != end_sec) {

        OptFirstArg first_arg;

        OptSecondArg second_arg;
        second_arg.emplace( *begin_sec );

        OptArgPair val( first_arg, second_arg );   

        func( val );

        ++begin_sec;
    }
}

} // eof namespace zipit

#endif
