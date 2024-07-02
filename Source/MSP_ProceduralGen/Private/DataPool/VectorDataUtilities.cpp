#include "VectorDataUtilities.h"

TArray<FVector2D> FVectorDataUtilities::GeneratePointsIn2DPolygon(const TArray<FVector2D>& Points, const TArray<TArray<FVector2D>>& Gaps, const int32 Amount, const float Spacing)
{
	FVector2D longestEdgePointA, longestEdgePointB;
	float longestDistance;
	FindLongestEdge(Points, longestDistance, longestEdgePointA, longestEdgePointB);

	FVector2D yAxis = (longestEdgePointB - longestEdgePointA).GetSafeNormal();
	FVector2D xAxis(-yAxis.Y, yAxis.X); // trick to rotate 90 degrees for the matrix frame

	FMatrix2x2 inverseRotationMatrix(xAxis.X, xAxis.Y, yAxis.X, yAxis.Y);

	TArray<FVector2D> transformedPoints;
	for (const FVector2D& point : Points)
	{
		FVector2D centeredPoint = point - longestEdgePointA;
		transformedPoints.Add(inverseRotationMatrix.TransformPoint(centeredPoint));
	}

	FBox2D localAlignedBoundingBox(transformedPoints[0], transformedPoints[0]);
	for (const FVector2D& transformedPoint : transformedPoints)
	{
		localAlignedBoundingBox += transformedPoint;
	}

	//Determine spacing based on amount of objects and surface area
	float surfaceArea = GetPolygonArea(Points, Gaps);
	int32 numberObjects = Amount;
	float objectSpacing = Spacing;
	if (objectSpacing < FLT_EPSILON)
	{
		if (numberObjects > 0)
		{
			objectSpacing = FMath::Sqrt((surfaceArea / (numberObjects * 0.97f)));
		}
		else
		{
			objectSpacing = FMath::Sqrt(surfaceArea / 100.0f);
		}
	}
	if (numberObjects <= 0)
		numberObjects = INT32_MAX;

	float cellWidth = objectSpacing;
	float cellHeight = objectSpacing;

	int32 numCols = FMath::CeilToInt(localAlignedBoundingBox.GetSize().X / cellWidth);
	int32 numRows = FMath::CeilToInt(localAlignedBoundingBox.GetSize().Y / cellHeight);
	verifyf(numRows * numCols < 10000, TEXT("Generating over 10000 points inside a polygon, this might not be what we are looking for."));

	float localOffsetX = (localAlignedBoundingBox.GetSize().X - cellWidth * (numCols - 1)) / 2;
	float localOffsetY = (localAlignedBoundingBox.GetSize().Y - cellHeight * (numRows - 1)) / 2;

	// Add the points and check if they are inside the area and not inside a gap
	int32 objectsPlaced = 0;
	TArray<FVector2D> instancePoints;
	for (int32 row = 0; row < numRows && objectsPlaced < numberObjects; ++row)
	{
		for (int32 col = 0; col < numCols && objectsPlaced < numberObjects; ++col)
		{
			FVector2D localCenter = localAlignedBoundingBox.Min + FVector2D(cellWidth * col + localOffsetX, cellHeight * row + localOffsetY);
			FVector2D worldPoint = inverseRotationMatrix.Inverse().TransformPoint(localCenter) + longestEdgePointA;
			if (IsPointInsidePolygonWithHoles(worldPoint, Points, Gaps))
			{
				instancePoints.Add(worldPoint);
				objectsPlaced++;
			}
		}
	}
	return instancePoints;
}

float FVectorDataUtilities::GetPolygonArea(const TArray<FVector2D>& a_points, const TArray<TArray<FVector2D>>& a_gaps)
{
	float area = 0;
	for (int i = 0; i < a_points.Num(); ++i)
	{
		int j = (i + 1) % a_points.Num();
		area += a_points[i].X * a_points[j].Y - a_points[i].Y * a_points[j].X;
	}
	area = FMath::Abs(area * 0.5f);
	for (auto gap : a_gaps)
	{
		TArray<TArray<FVector2D>> noGaps;
		area -= GetPolygonArea(gap, noGaps);
	}
	return area;
}

FVector2D FVectorDataUtilities::GetPolygonCenter(const TArray<FVector2D>& a_points)
{
	FVector2D accumulator = FVector2D::Zero();
	for (const FVector2D& point : a_points)
	{
		accumulator += point;
	}
	FVector2D center = accumulator / a_points.Num();
	return center;
}

void FVectorDataUtilities::FindLongestEdge(const TArray<FVector2D>& a_points, float& a_outDistance, FVector2D& a_outStartPoint, FVector2D& a_outEndPoint)
{
	a_outDistance = 0.0f;
	for (int32 i = 0; i < a_points.Num(); i++)
	{
		const int32 j = (i + 1) % a_points.Num();

		const float distance = FVector2D::Distance(a_points[i], a_points[j]);
		if (distance > a_outDistance)
		{
			a_outDistance = distance;
			a_outStartPoint = a_points[i];
			a_outEndPoint = a_points[j];
		}
	}
};

bool FVectorDataUtilities::IsPointInsidePolygon(const FVector2D& a_point, const TArray<FVector2D>& a_polygon)
{
	int windingNumber = 0;

	const int numVertices = a_polygon.Num();
	for (int i = 0; i < numVertices; ++i)
	{
		int j = (i + 1) % numVertices;

		if (a_polygon[i].Y <= a_point.Y)
		{
			if (a_polygon[j].Y > a_point.Y)
			{
				if ((a_polygon[j].X - a_polygon[i].X) * (a_point.Y - a_polygon[i].Y) - (a_point.X - a_polygon[i].X) * (a_polygon[j].Y - a_polygon[i].Y) > 0)
				{
					++windingNumber;
				}
			}
		}
		else
		{
			if (a_polygon[j].Y <= a_point.Y)
			{
				if ((a_polygon[j].X - a_polygon[i].X) * (a_point.Y - a_polygon[i].Y) - (a_point.X - a_polygon[i].X) * (a_polygon[j].Y - a_polygon[i].Y) < 0)
				{
					--windingNumber;
				}
			}
		}
	}

	return windingNumber != 0;
}

bool FVectorDataUtilities::IsPointInsidePolygonWithHoles(const FVector2D& a_point, const TArray<FVector2D>& a_polygons, const TArray<TArray<FVector2D>>& a_gapsArray)
{
	if (a_polygons.Num() == 0)
	{
		return false;
	}

	// Check if the point is inside the outer boundary
	if (!IsPointInsidePolygon(a_point, a_polygons))
	{
		return false;
	}

	// Check if the point is inside any of the hole polygons
	for (int32 i = 0; i < a_gapsArray.Num(); ++i)
	{
		if (IsPointInsidePolygon(a_point, a_gapsArray[i]))
		{
			return false;
		}
	}

	return true;
}
