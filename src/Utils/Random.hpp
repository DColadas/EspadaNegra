#pragma once

#include <algorithm>
#include <iterator>
#include <random>

// Utilities to generate random numbers
namespace Random {
namespace {
//TODO I think this is incorrect in a header file
std::random_device rd;  //Seed
std::mt19937 rng{rd()};
}  // namespace

// Generate random integer between two given ones (included)
int randInt(int min, int max);

// Shuffle a collection of elements
template <class RandomIt>
void shuffle(RandomIt first, RandomIt last) {
    std::shuffle(first, last, rng);
}

}  // namespace Random
