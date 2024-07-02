#include "ArrayFunctions.h"

TArray<int32> UArrayFunctions::InvertArray(const TArray<int32>& InputArray, int32 MaxIndex)
{
    int32 InvertedArraySize = MaxIndex - InputArray.Num();
    TArray<int32> InvertedArray;
    InvertedArray.SetNumUninitialized(InvertedArraySize);

    int32 InputIndex = InputArray.Num() - 1; 
    int32 InvertedIndex = 0; 

    for (int32 i = MaxIndex - 1; i >= 0; --i)
    {
        if (InputIndex >= 0 && InputArray[InputIndex] == i)
        {
            InputIndex--;
        }
        else
        {
            InvertedArray[InvertedIndex++] = i;
        }
    }

    return InvertedArray;
}
