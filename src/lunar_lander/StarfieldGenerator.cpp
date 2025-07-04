#include "lunar_lander/StarfieldGenerator.h"
#include <iostream>

StarfieldGenerator::StarfieldGenerator()
    : mLargeScaleNoise(std::random_device{}())
    , mMediumScaleNoise(std::random_device{}())
    , mSmallScaleNoise(std::random_device{}())
    , mVariationNoise(std::random_device{}())
    , mGenerator(std::random_device{}())
{
}

void StarfieldGenerator::generateStarfield(int width, int height, size_t targetStars)
{
    // Reinitialize noise generators with new seed each time
    unsigned int seed = std::random_device{}();
    mLargeScaleNoise = PerlinNoise1D(seed);
    mMediumScaleNoise = PerlinNoise1D(seed + 1);
    mSmallScaleNoise = PerlinNoise1D(seed + 2);
    mVariationNoise = PerlinNoise1D(seed + 3);
    mGenerator.seed(seed);
    
    // Set dimensions and derived values
    mWidth = width;
    mHeight = height;
    mGalacticCenterX = static_cast<int>(width * 0.3); // Off-center for realism
    mGalacticCenterY = static_cast<int>(height * 0.35); // Lower third of screen
    mGalacticPlaneY = static_cast<int>(height * 0.45); // Slightly below center
    
    mStars.clear();
    mStars.reserve(4000); // Pre-allocate for performance

    // Distribute stars across different astronomical components
    // These percentages roughly match real galaxy structure
    size_t galacticPlaneStars = static_cast<size_t>(targetStars * 0.25);
    size_t galacticCenterStars = static_cast<size_t>(targetStars * 0.15);
    size_t spiralArmStars = static_cast<size_t>(targetStars * 0.25);
    size_t clusterStars = static_cast<size_t>(targetStars * 0.20);
    size_t backgroundStars = targetStars - (galacticPlaneStars + galacticCenterStars + spiralArmStars + clusterStars);

    std::cout << "Generating realistic starfield with " << targetStars << " stars:" << std::endl;
    std::cout << "  Galactic plane: " << galacticPlaneStars << " stars" << std::endl;
    std::cout << "  Galactic center: " << galacticCenterStars << " stars" << std::endl;
    std::cout << "  Spiral arms: " << spiralArmStars << " stars" << std::endl;
    std::cout << "  Star clusters: " << clusterStars << " stars" << std::endl;
    std::cout << "  Background: " << backgroundStars << " stars" << std::endl;

    generateGalacticPlane(galacticPlaneStars);
    generateGalacticCenter(galacticCenterStars);
    generateSpiralArms(spiralArmStars);
    generateLocalClusters(clusterStars);
    generateBackgroundStars(backgroundStars);

    std::cout << "Generated " << mStars.size() << " total stars" << std::endl;
}

void StarfieldGenerator::generateGalacticPlane(size_t starCount)
{
    // The galactic plane appears as a horizontal band with varying density
    std::uniform_int_distribution<int> xDist(0, mWidth - 1);
    std::uniform_real_distribution<double> chanceDist(0.0, 1.0);

    size_t starsPlaced = 0;
    const size_t maxAttempts = starCount * 3; // Prevent infinite loops
    size_t attempts = 0;

    while (starsPlaced < starCount && attempts < maxAttempts)
    {
        int x = xDist(mGenerator);

        // Use 1D noise along X-axis to vary the galactic plane density
        double xNoise = mLargeScaleNoise.octaveNoise(x * 0.003, 3, 0.5, 1.0); // Large-scale structure
        double localVariation = mVariationNoise.noise(x * 0.02); // Local variation

        // Combine noise sources for complex density variation
        double densityFactor = (xNoise * 0.7 + localVariation * 0.3 + 1.0) * 0.5; // Normalize to [0,1]

        // Generate Y coordinate with preference for galactic plane
        std::normal_distribution<double> yDist(mGalacticPlaneY, mHeight * 0.08); // 8% std deviation
        int y = static_cast<int>(yDist(mGenerator));
        y = std::clamp(y, 0, mHeight - 1);

        // Distance from galactic plane affects probability
        double planeDistance = calculateGalacticPlaneDistance(y);
        double planeProbability = std::exp(-planeDistance * planeDistance * 8.0); // Exponential falloff

        // Final probability combines density variation with distance from plane
        double finalProbability = densityFactor * planeProbability * 0.8;

        if (chanceDist(mGenerator) < finalProbability)
        {
            addStar(x, y, static_cast<double>(densityFactor), 1.0);
            starsPlaced++;
        }
        attempts++;
    }
}

void StarfieldGenerator::generateGalacticCenter(size_t starCount)
{
    // Dense central bulge with exponential falloff from center
    std::uniform_real_distribution<double> chanceDist(0.0, 1.0);

    size_t starsPlaced = 0;
    const size_t maxAttempts = starCount * 4;
    size_t attempts = 0;

    while (starsPlaced < starCount && attempts < maxAttempts)
    {
        // Generate positions with bias toward galactic center
        std::normal_distribution<double> xDist(mGalacticCenterX, mWidth * 0.15);
        std::normal_distribution<double> yDist(mGalacticCenterY, mHeight * 0.12);

        int x = static_cast<int>(xDist(mGenerator));
        int y = static_cast<int>(yDist(mGenerator));

        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
        {
            attempts++;
            continue;
        }

        double distanceFromCenter = calculateDistanceFromCenter(x, y);

        // Use distance as input to 1D noise for central structure
        double centralNoise = mMediumScaleNoise.octaveNoise(distanceFromCenter * 0.01, 2, 0.6, 1.0);

        // Probability decreases exponentially with distance from center
        double distanceFactor = std::exp(-distanceFromCenter * distanceFromCenter * 0.00001);
        double finalProbability = (centralNoise + 1.0) * 0.5 * distanceFactor * 1.2;

        if (chanceDist(mGenerator) < finalProbability)
        {
            // Central stars tend to be brighter
            addStar(x, y, static_cast<double>(finalProbability), 1.2);
            starsPlaced++;
        }
        attempts++;
    }
}

void StarfieldGenerator::generateSpiralArms(size_t starCount)
{
    // Create hints of spiral structure using 1D noise along calculated spiral paths
    std::uniform_real_distribution<double> chanceDist(0.0, 1.0);

    const int numArms = 2; // Two main spiral arms
    size_t starsPerArm = starCount / numArms;

    for (int arm = 0; arm < numArms; ++arm)
    {
        size_t starsPlaced = 0;
        const size_t maxAttempts = starsPerArm * 3;
        size_t attempts = 0;

        double armOffset = arm * M_PI; // 180 degrees apart

        while (starsPlaced < starsPerArm && attempts < maxAttempts)
        {
            // Generate radius from galactic center
            std::uniform_real_distribution<double> radiusDist(mWidth * 0.1, mWidth * 0.4);
            double radius = radiusDist(mGenerator);

            // Calculate spiral angle (galaxies have logarithmic spirals)
            double angle = armOffset + radius * 0.008; // Spiral tightness factor

            // Convert to screen coordinates
            int x = mGalacticCenterX + static_cast<int>(radius * std::cos(angle));
            int y = mGalacticCenterY + static_cast<int>(radius * std::sin(angle) * 0.7); // Flattened

            if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
            {
                attempts++;
                continue;
            }

            // Use radius as input to 1D noise for spiral arm density
            double spiralNoise = mMediumScaleNoise.octaveNoise(radius * 0.005, 2, 0.5, 1.0);
            double localNoise = mSmallScaleNoise.noise(radius * 0.02);

            double armDensity = (spiralNoise * 0.8 + localNoise * 0.2 + 1.0) * 0.5;

            // Add some width to spiral arms
            std::normal_distribution<double> armWidth(0.0, mHeight * 0.03);
            y += static_cast<int>(armWidth(mGenerator));
            y = std::clamp(y, 0, mHeight - 1);

            if (chanceDist(mGenerator) < armDensity * 0.6)
            {
                addStar(x, y, static_cast<double>(armDensity), 1.0);
                starsPlaced++;
            }
            attempts++;
        }
    }
}

void StarfieldGenerator::generateLocalClusters(size_t starCount)
{
    // Create small, dense star clusters scattered throughout
    const int numClusters = 8;
    size_t starsPerCluster = starCount / numClusters;

    for (int cluster = 0; cluster < numClusters; ++cluster)
    {
        // Random cluster center
        std::uniform_int_distribution<int> xCenterDist(mWidth / 6, 5 * mWidth / 6);
        std::uniform_int_distribution<int> yCenterDist(mHeight / 6, 5 * mHeight / 6);

        int clusterX = xCenterDist(mGenerator);
        int clusterY = yCenterDist(mGenerator);

        // Use cluster position as seed for 1D noise
        double clusterSeed = clusterX * 0.001 + clusterY * 0.001;
        double clusterDensity = (mSmallScaleNoise.noise(clusterSeed) + 1.0) * 0.5;

        // Skip very low-density clusters
        if (clusterDensity < 0.3)
            continue;

        size_t actualStarsThisCluster = static_cast<size_t>(starsPerCluster * clusterDensity);

        for (size_t i = 0; i < actualStarsThisCluster; ++i)
        {
            // Stars clustered around center with Gaussian distribution
            std::normal_distribution<double> xDist(clusterX, mWidth * 0.025); // 2.5% spread
            std::normal_distribution<double> yDist(clusterY, mHeight * 0.025);

            int x = static_cast<int>(xDist(mGenerator));
            int y = static_cast<int>(yDist(mGenerator));

            if (x >= 0 && x < mWidth && y >= 0 && y < mHeight)
            {
                addStar(x, y, static_cast<double>(clusterDensity), 0.9);
            }
        }
    }
}

void StarfieldGenerator::generateBackgroundStars(size_t starCount)
{
    // Sparse, uniform background population
    std::uniform_int_distribution<int> xDist(0, mWidth - 1);
    std::uniform_int_distribution<int> yDist(0, mHeight - 1);

    for (size_t i = 0; i < starCount; ++i)
    {
        int x = xDist(mGenerator);
        int y = yDist(mGenerator);

        // Very slight density variation using 1D noise
        double variation = mVariationNoise.noise((x + y) * 0.001);
        double intensity = (variation + 1.0) * 0.25 + 0.3; // Range [0.3, 0.8]

        addStar(x, y, static_cast<double>(intensity), 0.8);
    }
}

double StarfieldGenerator::calculateGalacticPlaneDistance(int y) const
{
    return std::abs(y - mGalacticPlaneY) / static_cast<double>(mHeight);
}

double StarfieldGenerator::calculateDistanceFromCenter(int x, int y) const
{
    double dx = x - mGalacticCenterX;
    double dy = y - mGalacticCenterY;
    return static_cast<double>(std::sqrt(dx * dx + dy * dy));
}

void StarfieldGenerator::addStar(int x, int y, double intensity, double baseSize)
{
    // Clamp coordinates
    x = std::clamp(x, 0, mWidth - 1);
    y = std::clamp(y, 0, mHeight - 1);

    // Convert intensity to brightness (100-255 range)
    int brightness = static_cast<int>(100 + intensity * 155);
    brightness = std::clamp(brightness, 100, 255);

    // Size variation for future enhancement
    double size = baseSize * (0.8 + intensity * 0.4);

    mStars.emplace_back(Star { x, y, static_cast<Uint8>(brightness), size });
}

void StarfieldGenerator::createStarfieldTexture(SDL_Renderer* renderer, Texture& outTexture)
{
    SDL_Texture* renderTarget = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        mWidth, mHeight
    );

    if (!renderTarget) {
        printf("Failed to create realistic starfield texture: %s\n", SDL_GetError());
        return;
    }

    SDL_SetRenderTarget(renderer, renderTarget);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black space
    SDL_RenderClear(renderer);

    // Render stars with brightness variation
    for (const auto& star : mStars) {
        SDL_SetRenderDrawColor(renderer, star.brightness, star.brightness, star.brightness, 255);
        SDL_RenderDrawPoint(renderer, star.x, star.y);

        // Optional: Add slight bloom for brighter stars
        if (star.brightness > 200) {
            SDL_SetRenderDrawColor(renderer, star.brightness / 3, star.brightness / 3, star.brightness / 3, 255);
            SDL_RenderDrawPoint(renderer, star.x + 1, star.y);
            SDL_RenderDrawPoint(renderer, star.x - 1, star.y);
            SDL_RenderDrawPoint(renderer, star.x, star.y + 1);
            SDL_RenderDrawPoint(renderer, star.x, star.y - 1);
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);

    // Assuming your Texture class has a method to wrap existing SDL_Texture
    outTexture.setTexture(renderTarget, mWidth, mHeight);
}