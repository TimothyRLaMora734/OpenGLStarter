
#ifndef ReferenceCounter__H
#define ReferenceCounter__H

#include <map>

template <typename T>
class ReferenceCounter {
    std::map<T,int> map;
public:
    T add(T c){
        if (map.find(c) == map.end())
            map[c] = 0;
        map[c]++;
        return c;
    }
    void remove(T c){
        if (map.find(c) != map.end()){
            map[c]--;
            if (map[c] <= 0){
                map.erase(c);
                setNullAndDelete(c);
            }
        }
    }
    ~ReferenceCounter() {
        typename std::map<T,int>::iterator it = map.begin();
        while (it != map.end()){
            T c = it->first;
            setNullAndDelete(c);
            it++;
        }
        map.clear();
    }
};



template <typename T>
class ReferenceCounterArray {
    std::map<T,int> map;
public:
    T add(T c){
        if (map.find(c) == map.end())
            map[c] = 0;
        map[c]++;
        return c;
    }
    void remove(T c){
        if (map.find(c) != map.end()){
            map[c]--;
            if (map[c] <= 0){
                map.erase(c);
                setNullAndDeleteArray(c);
            }
        }
    }
    ~ReferenceCounterArray() {
        typename std::map<T,int>::iterator it = map.begin();
        while (it != map.end()){
            T c = it->first;
            setNullAndDeleteArray(c);
            it++;
        }
        map.clear();
    }
};


#endif
