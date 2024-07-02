#pragma once 

class FVectorDataUtilities
{
public:
	static TArray<FVector2D> GeneratePointsIn2DPolygon(const TArray<FVector2D>& Points, const TArray<TArray<FVector2D>>& Gaps, const int32 Amount, const float Spacing);
	static float GetPolygonArea(const TArray<FVector2D>& a_points, const TArray<TArray<FVector2D>>& a_gaps);
	static FVector2D GetPolygonCenter(const TArray<FVector2D>& a_points);
	static void FindLongestEdge(const TArray<FVector2D>& a_points, float& a_outDistance, FVector2D& a_outStartPoint, FVector2D& a_outEndPoint);
	static bool IsPointInsidePolygon(const FVector2D& a_point, const TArray<FVector2D>& a_polygon);
	static bool IsPointInsidePolygonWithHoles(const FVector2D& a_point, const TArray<FVector2D>& a_polygons ,const TArray<TArray<FVector2D>>& a_gapsArray);
};