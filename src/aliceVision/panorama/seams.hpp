#pragma once

#include <aliceVision/types.hpp>
#include <aliceVision/image/all.hpp>

#include "cachedImage.hpp"
#include "graphcut.hpp"
#include "boundingBoxMap.hpp"

namespace aliceVision
{

void drawBorders(aliceVision::image::Image<image::RGBAfColor>& inout, aliceVision::image::Image<unsigned char>& mask,
                 size_t offset_x, size_t offset_y);
void drawSeams(aliceVision::image::Image<image::RGBAfColor>& inout, aliceVision::image::Image<IndexT>& labels);

bool getMaskFromLabels(aliceVision::image::Image<float> & mask, CachedImage<IndexT> & labels, IndexT index, size_t offset_x, size_t offset_y);

class WTASeams
{
public:
    WTASeams(size_t outputWidth, size_t outputHeight)
        : _panoramaWidth(outputWidth)
        , _panoramaHeight(outputHeight)
    {
    }

    virtual ~WTASeams() = default;

    bool initialize(image::TileCacheManager::shared_ptr & cacheManager);

    bool append(const aliceVision::image::Image<unsigned char>& inputMask,
                        const aliceVision::image::Image<float>& inputWeights, 
                        IndexT currentIndex, size_t offset_x, size_t offset_y);

    CachedImage<IndexT> & getLabels() 
    {
        return _labels; 
    }

private:
    CachedImage<float> _weights;
    CachedImage<IndexT> _labels;

    int _panoramaWidth;
    int _panoramaHeight;
};

class HierarchicalGraphcutSeams
{
public:
    HierarchicalGraphcutSeams(image::TileCacheManager::shared_ptr cacheManager, size_t outputWidth, size_t outputHeight, size_t countLevels)
        : _cacheManager(cacheManager)
        , _outputWidth(outputWidth)
        , _outputHeight(outputHeight)
        , _countLevels(countLevels)
    {
    }

    virtual ~HierarchicalGraphcutSeams() = default;

    bool initialize();

    bool setOriginalLabels(CachedImage<IndexT>& labels);

    virtual bool append(const aliceVision::image::Image<image::RGBfColor>& input,
                        const aliceVision::image::Image<unsigned char>& inputMask, 
                        IndexT currentIndex, size_t offset_x, size_t offset_y);

    bool process();

    CachedImage<IndexT>& getLabels() 
    { 
        return _graphcuts[0].getLabels();
    }

private:
    std::vector<GraphcutSeams> _graphcuts;
    image::TileCacheManager::shared_ptr _cacheManager;

    size_t _countLevels;
    size_t _outputWidth;
    size_t _outputHeight;
};

} // namespace aliceVision