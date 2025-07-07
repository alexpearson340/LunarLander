#ifndef STARFIELD_GENERATOR_H
#define STARFIELD_GENERATOR_H

#include "PerlinNoise1D.h"
#include "engine/Texture.h"
#include <cmath>
#include <random>
#include <vector>


class StarfieldGenerator
{
public:
    struct Star
    {
        int x, y;
        Uint8 brightness;
        double size;
    };

    StarfieldGenerator();
    void generateStarfield(int width, int height, size_t targetStars = 2000);
    void createStarfieldTexture(SDL_Renderer* renderer, Texture* targetTexture);

private:
    // Astronomical structure generators
    void generateGalacticPlane(size_t starCount);   // Horizontal Milky Way band
    void generateGalacticCenter(size_t starCount);  // Dense central bulge
    void generateSpiralArms(size_t starCount);      // Spiral structure hints
    void generateLocalClusters(size_t starCount);   // Star clusters and associations
    void generateBackgroundStars(size_t starCount); // Sparse background population

    // Helper functions
    double calculateGalacticPlaneDistance(int y) const; // Distance from galactic plane
    double calculateDistanceFromCenter(int x, int y) const; // Distance from galactic center
    void addStar(int x, int y, double intensity, double baseSize = 1.0);

    // Configuration (set during generation)
    int mWidth, mHeight;
    int mGalacticCenterX, mGalacticCenterY;     // Where the "galaxy center" appears
    int mGalacticPlaneY;                        // Y coordinate of galactic plane

    // Noise generators for different astronomical scales
    PerlinNoise1D mLargeScaleNoise;     // Galaxy-wide structure
    PerlinNoise1D mMediumScaleNoise;    // Spiral arms and regions
    PerlinNoise1D mSmallScaleNoise;     // Local clusters
    PerlinNoise1D mVariationNoise;      // Fine-scale variation

    // Random number generation
    std::mt19937 mGenerator;

    // Generated stars
    std::vector<Star> mStars;
};

#endif // STARFIELD_GENERATOR_H