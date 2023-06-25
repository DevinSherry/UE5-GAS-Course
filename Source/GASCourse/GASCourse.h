// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//Current Class Name + Function Name where this is called!
#define GASCOURSE_CUR_CLASS_FUNC (FString(__FUNCTION__))

//Current Class where this is called!
#define GASCOURSE_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Function Name where this is called!
#define GASCOURSE_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))
  
//Current Line Number in the code where this is called!
#define GASCOURSE_CUR_LINE  (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called!
#define GASCOURSE_CUR_CLASS_LINE (GASCOURSE_CUR_CLASS + "(" + GASCOURSE_CUR_LINE + ")")
  
//Current Function Signature where this is called!
#define GASCOURSE_CUR_FUNCSIG (FString(__FUNCSIG__))

#include "CoreMinimal.h"
