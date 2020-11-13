#include "zipit.h"
#include "gtest/gtest.h"
#include <vector>

namespace {


using namespace zipit;    

TEST(TestZip,testIteratorMulti) {
    std::vector<int> vecA = { 1, 2, 3};
    std::vector<char> vecB = { 'a', 'b', 'c', 'd', 'e', 'f' };
    std::vector<float> vecC = { 0.57721, 1.1, 2.2, 3.1415 };

    size_t len = 0;
    for(auto it = maken(vecA.begin(),vecB.begin(),vecC.begin()); it != maken(vecA.end(), vecB.end(), vecC.end()); ++it) {
        printf("itern: first %d second %c third %f\n", std::get<0>(*it), std::get<1>(*it), std::get<2>(*it) );
        ++len;
    }
	EXPECT_TRUE(len == vecA.size());
}

TEST(TestZip,testIteratorEqualLength) {
    std::vector<int> vecA = { 1, 2, 3};
    std::vector<char> vecB = { 'a', 'b', 'c' };

    size_t len = 0;
    for(auto it=make(vecA.begin(),vecB.begin()); it != make(vecA.end(), vecB.end()); ++it) {\
        printf("iter: first %d second %c\n", (*it).first, (*it).second);
        ++len;
    }
	EXPECT_TRUE(len == vecA.size());

    auto it=make(vecA.begin(),vecB.begin());
    EXPECT_TRUE( (*it).first == 1 && (*it).second == 'a');
    ++it;
    EXPECT_TRUE( (*it).first == 2 && (*it).second == 'b');
    ++it;
    EXPECT_TRUE( (*it).first == 3 && (*it).second == 'c');
}


TEST(TestZip,testIteratorNotEqualLength) {
    std::vector<int> vecA = { 1, 2, 3};
    std::vector<char> vecB = { 'a', 'b', 'c', 'd', 'e', 'f' };

    size_t len = 0;
    for(auto it=make(vecA.begin(),vecB.begin()); it != make(vecA.end(), vecB.end()); ++it) {\
        printf("iter: first %d second %c\n", (*it).first, (*it).second);
        ++len;
    }
	EXPECT_TRUE(len == vecA.size());

    auto it=make(vecA.begin(),vecB.begin());
    EXPECT_TRUE( (*it).first == 1 && (*it).second == 'a');
    ++it;
    EXPECT_TRUE( (*it).first == 2 && (*it).second == 'b');
    ++it;
    EXPECT_TRUE( (*it).first == 3 && (*it).second == 'c');
}


TEST(TestZip,testForeachSimple) {

    std::vector<int> vecA = { 1, 2, 3};
    std::vector<char> vecB = { 'a', 'b', 'c', 'd', 'e' };

    struct func {

        func() : count_(0) {
        }    
        
        void operator() (std::pair<int,char> &arg) const {
            printf("foreach: first %d second %c\n", arg.first, arg.second);
            count_ += 1;
            first_list_.push_back(arg.first);
            second_list_.push_back(arg.second);
        }

        mutable size_t count_;
        mutable std::vector<int> first_list_;
        mutable std::vector<char> second_list_;    
    };

    func showIt;


    for_each_zip( vecA.begin(), vecA.end(), vecB.begin(), vecB.end(), showIt );

    EXPECT_TRUE(showIt.count_ == 0);

    for_each_zip_ref_func( vecA.begin(), vecA.end(), vecB.begin(), vecB.end(), showIt );

    //printf("%zu %zu\n", showIt.count_, showIt.first_list_.size());	

    EXPECT_TRUE(showIt.count_ == vecA.size());

    EXPECT_TRUE(std::equal( vecA.begin(), vecA.end(), showIt.first_list_.begin(), showIt.first_list_.end() ) );

    EXPECT_TRUE(std::equal( vecB.begin(), vecB.begin() + vecA.size(), showIt.second_list_.begin(), showIt.second_list_.end() ) );


}
 

TEST(TestZip,testForeachLongest) {

    std::vector<int> vecA = { 1, 2, 3};
    std::vector<char> vecB = { 'a', 'b', 'c', 'd', 'e' };

    struct func {

        func() : count_(0) {
        }    
        
        void operator() (std::pair< std::optional<int>, std::optional<char> > &arg) const {

            if (arg.first.has_value()) {
                printf("first %d ", arg.first.value());
                first_list_.push_back(arg.first.value());
             }
            if (arg.second.has_value()) {
                printf("second %c", arg.second.value());
                second_list_.push_back(arg.second.value());
            }
            printf("\n");

            count_ += 1;
        }

        mutable size_t count_;
        mutable std::vector<int> first_list_;
        mutable std::vector<char> second_list_;    
    };

    func showIt;


    for_each_zip_longest( vecA.begin(), vecA.end(), vecB.begin(), vecB.end(), showIt );

    EXPECT_TRUE(showIt.count_ == 0);

    for_each_zip_longest_ref_func( vecA.begin(), vecA.end(), vecB.begin(), vecB.end(), showIt );

    //printf("%zu %zu\n", showIt.count_, showIt.first_list_.size());	

    EXPECT_TRUE(showIt.count_ == vecB.size());

    EXPECT_TRUE(std::equal( vecA.begin(), vecA.end(), showIt.first_list_.begin(), showIt.first_list_.end() ) );

    EXPECT_TRUE(std::equal( vecB.begin(), vecB.end(), showIt.second_list_.begin(), showIt.second_list_.end() ) );

}
 
} // namespace



