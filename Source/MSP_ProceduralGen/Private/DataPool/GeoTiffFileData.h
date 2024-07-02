#pragma once

static_assert(WITH_LIBTIFF, "GeoTiff relies on LibTiff being compiled in.");

//From http://geotiff.maptools.org/spec/geotiff6.html Section 6.2
enum class EGeoTiffGeoKey: short
{
	Invalid = 0,
	GTModelTypeGeoKey			   = 1024, /*  Section 6.3.1.1 Codes       */
	GTRasterTypeGeoKey			   = 1025, /*  Section 6.3.1.2 Codes       */
	GTCitationGeoKey			   = 1026, /* documentation */

	GeographicTypeGeoKey           = 2048, /*  Section 6.3.2.1 Codes     */
	GeogCitationGeoKey             = 2049, /* documentation             */
	GeogGeodeticDatumGeoKey        = 2050, /*  Section 6.3.2.2 Codes     */
	GeogPrimeMeridianGeoKey        = 2051, /*  Section 6.3.2.4 codes     */
	GeogLinearUnitsGeoKey          = 2052, /*  Section 6.3.1.3 Codes     */
	GeogLinearUnitSizeGeoKey       = 2053, /* meters                    */
	GeogAngularUnitsGeoKey         = 2054, /*  Section 6.3.1.4 Codes     */
	GeogAngularUnitSizeGeoKey      = 2055, /* radians                   */
	GeogEllipsoidGeoKey            = 2056, /*  Section 6.3.2.3 Codes     */
	GeogSemiMajorAxisGeoKey        = 2057, /* GeogLinearUnits           */
	GeogSemiMinorAxisGeoKey        = 2058, /* GeogLinearUnits           */
	GeogInvFlatteningGeoKey        = 2059, /* ratio                     */
	GeogAzimuthUnitsGeoKey         = 2060, /*  Section 6.3.1.4 Codes     */
	GeogPrimeMeridianLongGeoKey    = 2061, /* GeogAngularUnit           */

	ProjectedCSTypeGeoKey          = 3072,  /*  Section 6.3.3.1 codes   */
	PCSCitationGeoKey              = 3073,  /* documentation           */
	ProjectionGeoKey               = 3074,  /*  Section 6.3.3.2 codes   */
	ProjCoordTransGeoKey           = 3075,  /*  Section 6.3.3.3 codes   */
	ProjLinearUnitsGeoKey          = 3076,  /*  Section 6.3.1.3 codes   */
	ProjLinearUnitSizeGeoKey       = 3077,  /* meters                  */
	ProjStdParallel1GeoKey         = 3078,  /* GeogAngularUnit */
	ProjStdParallel2GeoKey         = 3079,  /* GeogAngularUnit */
	ProjNatOriginLongGeoKey        = 3080,  /* GeogAngularUnit */
	ProjNatOriginLatGeoKey         = 3081,  /* GeogAngularUnit */
	ProjFalseEastingGeoKey         = 3082,  /* ProjLinearUnits */
	ProjFalseNorthingGeoKey        = 3083,  /* ProjLinearUnits */
	ProjFalseOriginLongGeoKey      = 3084,  /* GeogAngularUnit */
	ProjFalseOriginLatGeoKey       = 3085,  /* GeogAngularUnit */
	ProjFalseOriginEastingGeoKey   = 3086,  /* ProjLinearUnits */
	ProjFalseOriginNorthingGeoKey  = 3087,  /* ProjLinearUnits */
	ProjCenterLongGeoKey           = 3088,  /* GeogAngularUnit */
	ProjCenterLatGeoKey            = 3089,  /* GeogAngularUnit */
	ProjCenterEastingGeoKey        = 3090,  /* ProjLinearUnits */
	ProjCenterNorthingGeoKey       = 3091,  /* ProjLinearUnits */
	ProjScaleAtNatOriginGeoKey     = 3092,  /* ratio   */
	ProjScaleAtCenterGeoKey        = 3093,  /* ratio   */
	ProjAzimuthAngleGeoKey         = 3094,  /* GeogAzimuthUnit */
	ProjStraightVertPoleLongGeoKey = 3095,  /* GeogAngularUnit */

	VerticalCSTypeGeoKey           = 4096,  /*  Section 6.3.4.1 codes   */
	VerticalCitationGeoKey         = 4097,  /* documentation */
	VerticalDatumGeoKey            = 4098,  /*  Section 6.3.4.2 codes   */
	VerticalUnitsGeoKey            = 4099,  /*  Section 6.3.1.3 codes   */
};

//http://geotiff.maptools.org/spec/geotiff6.html Section 6.3.1.1
enum class EGeoTiffModelType: short
{
	Undefined = 0,
	ModelTypeProjected   = 1,   /* Projection Coordinate System         */
	ModelTypeGeographic  = 2,   /* Geographic latitude-longitude System */
	ModelTypeGeocentric  = 3,   /* Geocentric (X,Y,Z) Coordinate System */
};

//http://geotiff.maptools.org/spec/geotiff6.html Section 6.3.1.2
enum class EGeoTiffRasterType: short
{
	Undefined = 0,
	RasterPixelIsArea  = 1,
	RasterPixelIsPoint = 2,
};

//http://geotiff.maptools.org/spec/geotiff6.html Section 6.3.1.3 and 6.3.1.4
enum class EGeoTiffUnits: short
{
	Undefined = 0,
	Linear_Meter =	9001,
	Linear_Foot =	9002,
	Linear_Foot_US_Survey =	9003,
	Linear_Foot_Modified_American =	9004,
	Linear_Foot_Clarke =	9005,
	Linear_Foot_Indian =	9006,
	Linear_Link =	9007,
	Linear_Link_Benoit =	9008,
	Linear_Link_Sears =	9009,
	Linear_Chain_Benoit =	9010,
	Linear_Chain_Sears =	9011,
	Linear_Yard_Sears =	9012,
	Linear_Yard_Indian =	9013,
	Linear_Fathom =	9014,
	Linear_Mile_International_Nautical =	9015,

	Angular_Radian =	9101,
	Angular_Degree =	9102,
	Angular_Arc_Minute =	9103,
	Angular_Arc_Second =	9104,
	Angular_Grad =	9105,
	Angular_Gon =	9106,
	Angular_DMS =	9107,
	Angular_DMS_Hemisphere =	9108,
};

enum class ETiffSampleFormat: uint16
{
	Uint = 1,	//Unsigned integer data
	Int = 2,	//two's complement signed integer data
	IEEEFP = 3,	//IEEE Floating point data
	Void = 4,	//Undefined

	ComplexInt = 5,
	ComplexIEEEFP = 6
};

class FGeoTiffFileData
{
public:
	FGeoTiffFileData(const FGeoTiffFileData&) = delete;
	FGeoTiffFileData(FGeoTiffFileData&&) noexcept = delete;

	FGeoTiffFileData& operator=(const FGeoTiffFileData&) = delete;
	FGeoTiffFileData& operator=(FGeoTiffFileData&&) noexcept = delete;

	static TUniquePtr<const FGeoTiffFileData> LoadFromFile(const FString& ResolvedFilePath);
	static TUniquePtr<const FGeoTiffFileData> LoadFromFileBytes(TArrayView<const uint8> FileBytes);

	double GetGeoUnitsToUnrealMultiplier() const;
	FVector2D GetPixelScaleAsUnrealUnits() const;
	FVector2D GetProjectedTiepointAsUnrealUnits() const;

	uint16 TiffSamplesPerPixel;
	uint16 TiffBitsPerSample;
	ETiffSampleFormat TiffSampleFormat;

	EGeoTiffModelType ModelType;
	EGeoTiffRasterType RasterType;
	EGeoTiffUnits LinearUnits;
	short CoordinateSystemType; //See http://geotiff.maptools.org/spec/geotiff6.html#6.3.3.1
	FString CoordinateSystemCitation;

	//FMatrix44d ModelTransformation{};
	double ModelPixelScale[3]{};
	FVector ModelTiepointRasterOrigin; //GeoTiff field ModelTiepoints [0-2]
	FVector ModelTiepointProjectedOrigin; //GeoTiff field ModelTiepoints [3-5]

private:
	FGeoTiffFileData() = default;

	void SetGeoTIFFKey(EGeoTiffGeoKey Key, TArrayView<const short> Values);
	void SetGeoTIFFKey(EGeoTiffGeoKey Key, TArrayView<const double> Values);
	void SetGeoTIFFKey(EGeoTiffGeoKey Key, FAnsiStringView Value);
};