#ifndef HW3_ASSET_H
#define HW3_ASSET_H

#include "global.h"

class Asset {
private:
    static AAssetManager* manager;

public:
    static void setManager(AAssetManager* aAssetManager);
    static AAssetManager* getManager(void);
    static const char* load(const char* fileName);
};

#endif //HW3_ASSET_H
