#pragma once

#include <iterator>

// Utilities to generate random numbers
namespace Random {

// Generate random integer between two given ones (included)
int randInt(int min, int max);

// Shuffle a collection of elements
template <class RandomIt>
void shuffle(RandomIt first, RandomIt last);

}  // namespace Random
