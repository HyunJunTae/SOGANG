
// for details, please refer to https://cplusplus.com/reference/random/.
#include "w12_exercise_0.h"

template <typename T>
void run (){

    const int sampleN = 10;
    
    std::vector<T> bucket;
 
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0,9);

    //exercise: add each element at the end of the vector.
    for ( int i = 0; i < sampleN ; i++ ){
        
        //randomly sample a integer.
        T sampledNumber = (T)distribution(generator); 

        //TODO: attach the element
        // typename std::vector<T>::iterator it = bucket.end();
        // bucket.insert(it++, sampledNumber);
        bucket.push_back(sampledNumber);

        //TODO: implement function printElementsUsingIterator<int> which must have the same result of function printElementsLikeArray.
        printElementsUsingIterator<T>(bucket);
        //printElementsLikeArray<int>(bucket);

    }

    //exercise: erase the minimum in the list until all elements are erased.
    for (int i=0;i<sampleN;i++){

        // find the minimum element and delete it. Please use vector<int>::iterator.
        typename std::vector<T>::iterator min_it = bucket.begin();
        typename std::vector<T>::iterator it;

        // TODO: find the minimum and erase it.
        for(typename std::vector<T>::iterator it = bucket.begin(); it != bucket.end(); it++) {
            if (*min_it > *it) {
                min_it = it;
            }
        }

        bucket.erase(min_it);

        printElementsLikeArray<T>(bucket);
        
    }

}

int main(){
    run<int>();
    return 0;
}