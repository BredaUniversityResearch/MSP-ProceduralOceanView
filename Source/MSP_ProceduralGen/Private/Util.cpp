#include "Util.h"

FName Util::CamelToSnake(const FName& a_input) {
    FString inputStr = a_input.ToString();
    FString outputStr;

    for (const TCHAR c : inputStr) {
        if (FChar::IsUpper(c) && !outputStr.IsEmpty()) {
            outputStr += TEXT('_');
        }
        outputStr += FChar::ToLower(c);
    }

    return FName(*outputStr);
}
