#ifndef PERLIN_NOISE_1D_H
#define PERLIN_NOISE_1D_H

#include <array>
#include <cmath>
#include <random>
#include <iostream>

class PerlinNoise1D
{
public:
    // Constructor with optional seed for reproducible results
    inline explicit PerlinNoise1D(unsigned int seed);
    
    // Generate noise value at position x
    // Returns value in range approximately [-1, 1]
    inline double noise(double x) const;
    
    // Generate noise with octaves for more complex patterns
    // octaves: number of noise layers to combine
    // persistence: amplitude multiplier for each octave (typically 0.5)
    // frequency: frequency multiplier for input (higher = more compressed)
    inline double octaveNoise(double x, int octaves, double persistence = 0.5, double frequency = 1.0) const;

private:
    static constexpr int PERMUTATION_SIZE = 256;
    std::array<int, PERMUTATION_SIZE * 2> mPermutation;
    
    // Initialize permutation table with given seed
    inline void initializePermutation(unsigned int seed);
    
    // Fade function for smooth interpolation (6t^5 - 15t^4 + 10t^3)
    inline static double fade(double t);
    
    // Linear interpolation
    inline static double lerp(double a, double b, double t);
    
    // Gradient function for 1D noise
    inline static double grad(int hash, double x);
};

PerlinNoise1D::PerlinNoise1D(unsigned int seed)
{
    std::cout << "Initializing Perlin Noise 1D generation with seed: " << seed << std::endl;
    initializePermutation(seed);
}

void PerlinNoise1D::initializePermutation(unsigned int seed)
{
    // Initialize with sequential values
    std::array<int, PERMUTATION_SIZE> p;
    std::iota(p.begin(), p.end(), 0);
    
    // Shuffle using the provided seed
    std::mt19937 generator(seed);
    std::shuffle(p.begin(), p.end(), generator);
    
    // Duplicate the permutation to avoid buffer overflow
    for (size_t i = 0; i < PERMUTATION_SIZE; ++i)
    {
        mPermutation[i] = p[i];
        mPermutation[i + PERMUTATION_SIZE] = p[i];
    }
}

double PerlinNoise1D::fade(double t)
{
    // Smooth step function: 6t^5 - 15t^4 + 10t^3
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

double PerlinNoise1D::lerp(double a, double b, double t)
{
    return a + t * (b - a);
}

double PerlinNoise1D::grad(int hash, double x)
{
    // Simple gradient for 1D: return x or -x based on hash
    return (hash & 1) ? x : -x;
}

double PerlinNoise1D::noise(double x) const
{
    // Find unit grid cell containing point
    size_t X = static_cast<size_t>(std::floor(x)) & 255;
    
    // Get relative position within cell
    x -= std::floor(x);
    
    // Compute fade curve for x
    double u = fade(x);
    
    // Hash coordinates of grid cell corners
    int A = mPermutation[X];
    int B = mPermutation[X + 1];
    
    // Blend the results from the two corners
    return lerp(grad(A, x), grad(B, x - 1.0), u);
}

double PerlinNoise1D::octaveNoise(double x, int octaves, double persistence, double frequency) const
{
    double value = 0.0;
    double amplitude = 1.0;
    double maxValue = 0.0; // Used for normalizing result to [-1, 1]
    
    for (int i = 0; i < octaves; ++i)
    {
        value += noise(x * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0;
    }
    
    return value / maxValue;
}

#endif // PERLIN_NOISE_1D_H