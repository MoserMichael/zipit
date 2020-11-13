#ifndef __ZIPIT_H__
#define __ZIPIT_H__

#include <utility>
#include <iterator>
#include <optional>
#include <tuple>

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
    
    bool operator==(const Zip_iterator other) const {
        return first_ == other.first_ || second_ == other.second_;
    }

    bool operator!=(const Zip_iterator other) const {
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


template<typename ...Type>
class Zip_iterator_tuple {
public:
    using self_type = Zip_iterator_tuple<Type...>;

    using elm_type = std::tuple< typename std::iterator_traits<Type>::value_type... >; 

    using difference_type = long;
    using value_type = elm_type;
    using pointer = elm_type*;
    using reference = elm_type&;
    using iterator_category = std::forward_iterator_tag;

    Zip_iterator_tuple(Type... it) : iterators_(it...) {
    }

    Zip_iterator_tuple& operator++() { 
        inc_helper<0>();
        return *this;
    }

   
    Zip_iterator_tuple operator++(int) {
        Zip_iterator_tuple retval = *this; 
        ++(*this); 
        return retval;
    }
    
    bool operator==(const self_type other) const {
        return cmp_helper<0>(other);
    }


    bool operator!=(const self_type other) const {
        return !(*this == other);
    }
    
    value_type operator*() { 

        value_type rval;

        make_rval<0>(rval);

        return rval;
    }

   
private:    
    std::tuple<Type...> iterators_;

    template<size_t FieldIndex>
    void inc_helper() {
       
        ++std::get<FieldIndex>(iterators_);

        if constexpr (FieldIndex < std::tuple_size<elm_type>::value - 1 ) { 
            inc_helper<FieldIndex + 1>();
        }   
    }

    template<size_t FieldIndex>
    bool cmp_helper(const self_type &other) const {
        
        if (std::get<FieldIndex>(iterators_) == std::get<FieldIndex>(other.iterators_)) {
            return true;
        }

        if constexpr (FieldIndex < std::tuple_size<elm_type>::value - 1) {
            return cmp_helper<FieldIndex + 1>(other); 
        }   
        return false;
    }

    template<size_t FieldIndex>
    void make_rval(value_type &rval) {
        std::get<FieldIndex>(rval) = *std::get<FieldIndex>(iterators_);

        if constexpr (FieldIndex < std::tuple_size<elm_type>::value - 1) {
            make_rval<FieldIndex + 1>(rval); 
        }
    }
};

template<typename ...Ty>
Zip_iterator_tuple<Ty...> maken(Ty ...it) { 
    return Zip_iterator_tuple(it...);
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
