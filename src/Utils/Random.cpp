#include "Random.hpp"

#include <random>
#include <algorithm>

namespace Random {
namespace {
std::random_device rd;  //Seed
std::mt19937 rng{rd()};
}  // namespace

int randInt(int min, int max) {
    //TODO panic if min < max
    std::uniform_int_distribution<> dist(min, max);
    return dist(rng);
}

template <class RandomIt>
void shuffle(RandomIt first, RandomIt last) {
    std::shuffle(first, last, rng);
}

}  // namespace Random
