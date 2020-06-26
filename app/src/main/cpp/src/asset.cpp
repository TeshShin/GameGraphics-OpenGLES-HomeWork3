#include "asset.h"

AAssetManager* Asset::manager = nullptr;

void Asset::setManager(AAssetManager *aAssetManager) {
    LOG_PRINT_DEBUG("Set asset manager");

    Asset::manager = aAssetManager;
}

AAssetManager* Asset::getManager() {
    if(Asset::manager == nullptr) {
        LOG_PRINT_ERROR("Fail to get asset manager");
    }
    return manager;
}

const char* Asset::load(const char* fileName) {
    LOG_PRINT_DEBUG("Load asset: %s", fileName);

    AAsset* aAsset = AAssetManager_open(getManager(), fileName, AASSET_MODE_UNKNOWN);

    if(aAsset == nullptr) {
        LOG_PRINT_ERROR("Fail to open asset: %s", fileName);
        return nullptr;
    }

    size_t aAssetLength = (size_t)AAsset_getLength(aAsset);
    char* const buffer = new char[aAssetLength];
    buffer[aAssetLength] = '\0';

    AAsset_read(aAsset, buffer, aAssetLength);
    AAsset_close(aAsset);

    return buffer;
}