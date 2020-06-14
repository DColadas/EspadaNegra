#include "Random.hpp"

#include <random>

int Random::randInt(int min, int max) {
    //TODO panic if min < max
    std::uniform_int_distribution<> dist(min, max);
    return dist(rng);
}
