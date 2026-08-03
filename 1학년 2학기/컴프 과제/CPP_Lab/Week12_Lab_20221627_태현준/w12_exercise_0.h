
#include <iostream>
#include <vector>
#include <random>

template <typename T>
void printElementsLikeArray(std::vector<T> &bucket){

    int len = bucket.size();

    for(int i=0;i<len;i++)
        std::cout<<bucket[i]<<" ";
    std::cout<<"\n";
    

}

template <typename T>
void printElementsUsingIterator(std::vector<T> &bucket){

    typename std::vector<T>::iterator it;

    //TODO
    for(typename std::vector<T>::iterator it = bucket.begin(); it != bucket.end(); it++) {
        std::cout<<*it<<" ";
    }
    std::cout<<"\n";

}

