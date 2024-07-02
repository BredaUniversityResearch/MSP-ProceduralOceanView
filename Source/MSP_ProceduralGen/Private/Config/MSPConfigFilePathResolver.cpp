#include "MSPConfigFilePathResolver.h"

#include "Config/Helpers/MSPConfigFileLoadHelper.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"

FMSPConfigFilePathResolver::FMSPConfigFilePathResolver(const FString& a_basePath)
	: BasePath(FPaths::GetPath(a_basePath))
{
	// // test code
	// FString a(FPaths::GetPath("dira\\file.ext")); //dira
	// FString b(FPaths::GetPath("dira\\dirb\\file.ext")); //dira\dirb
	// FString c(FPaths::GetPath("dira\\")); // dira
	// FString d(FPaths::GetPath("dira")); // empty
	// FString e(FPaths::GetPath("file.ext")); // empty
	// FString f(FPaths::GetPathLeaf("dira")); // dira
	// FString g(FPaths::GetPathLeaf("file.ext")); // file.ext
	// FString h(FPaths::GetExtension("dira")); // empty
	// FString i(FPaths::GetExtension("file.ext")); // ext
	// FString j(FPaths::GetExtension("file.ext\\")); // empty

	// If the result is empty, it means a_basePath is a directory without a file,
	//   or just a filename without a directory.
	if (BasePath.IsEmpty()) {
		// if there is an extension assume it is a file, then don't use it as a base path.
		//   user should add a \\ to force a directory with an extension in it
		BasePath = FPaths::GetExtension(a_basePath) == "" ? a_basePath : "";
	}
}

FString FMSPConfigFilePathResolver::ResolvePath(const FString& InPath) const
{
	const FString path = InPath.TrimChar('/'); // removes at max one instance in the beginning and end of the string
	if (!FPaths::IsRelative(path)) {
		// if the path contains a drive, show an warning, we should not ever use a full path, those are machine dependent.
		UE_LOG(LogTemp, Warning, TEXT("Encountered a full path, which should not be used: %s. Make it relative to the project content folder or loaded json path."), *InPath);
		return InPath;
	}

	if (!BasePath.IsEmpty())
	{
		FString result = FPaths::ProjectContentDir() + FMSPConfigFileLoadHelper::BASE_PATH_FROM_CONTENT_DIR + BasePath + "/" + path;
		if (FPaths::FileExists(result))
		{
			return result;
		}
	}

	// check content path
	FString projectPath = FPaths::ProjectContentDir() + "/" + path;
	if (FPaths::FileExists(projectPath))
	{
		return projectPath;
	}

	UE_LOG(LogMspProceduralGen, Warning, TEXT("Could not resolve file path %s to something relative to the config file."), *InPath);
	return InPath;
}
