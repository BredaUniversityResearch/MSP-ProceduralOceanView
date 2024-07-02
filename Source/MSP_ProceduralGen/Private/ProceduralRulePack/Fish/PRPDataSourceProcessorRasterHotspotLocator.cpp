#include "PRPDataSourceProcessorRasterHotspotLocator.h"

#include "DataPool/DataPool.h"
#include "DataPool/DataPoolRaster.h"

void UPRPDataSourceProcessorRasterHotspotLocator::RegisterWithDataPool_Implementation(UDataPool* DataPool)
{
    Super::RegisterWithDataPool_Implementation(DataPool);

    DataPool->CreateDataSource(DataPoolTargetPointLayer, EDataPoolDataType::Points, EDataPoolDataUnitType::Other);

    bool regSuccessNoise = DataPool->TryRegisterDataUpdatedListener(DataPoolSourceRasterLayer, DataPoolChangedHandler);
    check(regSuccessNoise);

    OnDataSourceChanged(DataPoolSourceRasterLayer);
}

void UPRPDataSourceProcessorRasterHotspotLocator::OnDataSourceChanged(const FName& dataSourceName)
{
    IDataPoolRasterReadOnly* sourceData = nullptr;
    if (TargetDataPool->TryGetData(DataPoolSourceRasterLayer, sourceData))
    {
        FDataPoolPoints targetPoints;
        TargetDataPool->TryGetData(DataPoolTargetPointLayer, targetPoints);

        RecalculateOutputLayer(TargetDataPool->GetSimulationBounds(), *sourceData, targetPoints);

        TargetDataPool->TrySetData(DataPoolTargetPointLayer, targetPoints);
    }
}

void UPRPDataSourceProcessorRasterHotspotLocator::RecalculateOutputLayer(const FBox2D& SimulationBounds, const IDataPoolRasterReadOnly& SourceLayer, FDataPoolPoints& TargetPoints) const
{
    struct HotSpotCellEntry
    {
        FIntPoint CellCoordinate;
        double CellMaxValue;
        FIntPoint CellMaxValueRasterPoint;
    };

    FVector2D simulationBoundsDelta = SimulationBounds.GetSize();
    FVector2D cellSize = simulationBoundsDelta / static_cast<double>(GridAxisDivisionCount);

    TArray<HotSpotCellEntry> results;
    results.Reserve(GridAxisDivisionCount * GridAxisDivisionCount);

    for (int y = 0; y < GridAxisDivisionCount; ++y)
    {
        for (int x = 0; x < GridAxisDivisionCount; ++x)
        {
            FVector2D cellMin = SimulationBounds.Min + FVector2D(cellSize.X * x, cellSize.Y * y);
            FVector2D cellMax = cellMin + cellSize;
            FIntPoint cellRasterMin = SourceLayer.GetProjection().WorldToRaster(cellMin);
            FIntPoint cellRasterMax = SourceLayer.GetProjection().WorldToRaster(cellMax);

            double maxValue = {};
            FIntPoint maxValueRasterLocation = {};
            SampleRasterCell(SourceLayer, FIntRect(cellRasterMin, cellRasterMax), maxValue, maxValueRasterLocation);

            results.Emplace(HotSpotCellEntry{.CellCoordinate = FIntPoint(x, y), .CellMaxValue = maxValue, .CellMaxValueRasterPoint = maxValueRasterLocation});
        }
    }

    Algo::SortBy(results, &HotSpotCellEntry::CellMaxValue, TGreater{});

    TArray<FIntPoint> usedHotSpotCells;
    int hotspotsCreated = 0;
    for (const HotSpotCellEntry& entry : results)
    {
        bool isNeighbouringUsedCell = false;
        for (const FIntPoint& usedCell : usedHotSpotCells)
        {
            FIntPoint delta = usedCell - entry.CellCoordinate;
            if (FMath::Abs(delta.X) <= 1 && FMath::Abs(delta.Y) <= 1)
            {
                isNeighbouringUsedCell = true;
                break;
            }
        }

        if (!isNeighbouringUsedCell)
        {
            FVector2D worldHotSpotLocation = SourceLayer.GetProjection().RasterSubPixelToWorld(entry.CellMaxValueRasterPoint);
            TargetPoints.m_points.Emplace(FDataPoolPoint{FVector(worldHotSpotLocation, 0.0f)});
            ++hotspotsCreated;
            usedHotSpotCells.Emplace(entry.CellCoordinate);

            if (hotspotsCreated == HotSpotsToTrack)
            {
                break;
            }
        }
    }
}

void UPRPDataSourceProcessorRasterHotspotLocator::SampleRasterCell(const IDataPoolRasterReadOnly& SourceLayer, const FIntRect& RasterBounds, double& MaxValue, FIntPoint& MaxValuePixelLocation) const
{
    ensure(RasterBounds.Size().X > 0 && RasterBounds.Size().Y > 0);

    MaxValue = 0.0f;
    for (int y = RasterBounds.Min.Y; y < RasterBounds.Max.Y; ++y)
    {
        for (int x = RasterBounds.Min.X; x < RasterBounds.Max.X; ++x)
        {
            double value = SourceLayer.GetNormalizedValueAtRasterPosition(FIntPoint(x, y));
            if (value > MaxValue)
            {
                MaxValue = value;
                MaxValuePixelLocation = FIntPoint(x, y);
            }
        }
    }
}
