#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <sstream>
#include <execution>


using namespace std;


template<typename Hash>
int FindCollisions(Hash &hasher, std::istream &text) {
    std::string word;
    int collisions = 0;
    std::unordered_map<size_t, std::unordered_set<std::string, Hash>> collisions_map;
    while (text >> word) {
        collisions_map[hasher(word)].insert(std::move(word));
    }
    std::for_each(execution::par,collisions_map.begin(), collisions_map.end(), [&collisions](const auto &p) {
        const auto&[hash, collisions_n] = p;
        collisions += collisions_n.size() - 1;
    });
    return collisions;
}

struct DummyHash {
    size_t operator()(const string&) const {
        return 42;
    }
};

int main() {
    DummyHash dummy_hash;
    hash<string> good_hash;

    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(dummy_hash, stream) << endl;
    }
    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(good_hash, stream) << endl;
    }
}