#ifndef LUNARLANDERENGINE_H
#define LUNARLANDERENGINE_H

#include "engine/BaseEngine.h"
#include "lunar_lander/Constants.h"

class LunarLanderEngine : public BaseEngine
{
public:
    LunarLanderEngine();

private:
    bool loadMedia() override;
    bool create() override;
    bool update() override;
    bool render() override;
};

#endif // LUNARLANDERENGINE_H