// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Cog/InputBuffer/FCogInputBufferWindow.h"
#include "Game/Character/Player/GASCoursePlayerCharacter.h"


#if ENABLE_COG

#include "CogImguiHelper.h"
#include "CogWindowHelper.h"
#include "CogWindowWidgets.h"

//--------------------------------------------------------------------------------------------------------------------------
void FCogInputBufferWindow::Initialize()
{
	Super::Initialize();
	bHasMenu = true;
	InputConfig = GetInputConfig();

	
	if(AGASCoursePlayerCharacter* PlayerCharacter = Cast<AGASCoursePlayerCharacter>(GetLocalPlayerPawn()))
	{
		//PlayerCharacter->OnInputBufferOpenedEvent.AddDynamic(this, &FCogInputBufferWindow::OnInputBufferOpened);
		//PlayerCharacter->OnInputBufferClosedEvent.AddDynamic(this, &FCogInputBufferWindow::OnInputBufferClosed);
	}
	
}

UGASCourseInputConfig* FCogInputBufferWindow::GetInputConfig()
{
	// specify the path to the asset you want to load
	FString AssetPath = TEXT("/Game/GASCourse/Game/Character/Input/Config/IC_GASCourseCharacter_NativeActions.IC_GASCourseCharacter_NativeActions");
  
	// load the asset; here null is passed for WorldContextObject
	UObject* Asset = StaticLoadObject(UObject::StaticClass(), nullptr, *AssetPath);
	
	return Cast<UGASCourseInputConfig>(Asset);
}

//--------------------------------------------------------------------------------------------------------------------------
void FCogInputBufferWindow::RenderHelp()
{
	ImGui::Text(
	"This window displays the input buffering state. "
	"It show when the input buffer opens, closes, and which input action tags are added to the buffer"
	"When the input buffer closes, it will show the input tag that is executed, if one is. "
	);
}

//--------------------------------------------------------------------------------------------------------------------------
void FCogInputBufferWindow::RenderContent()
{
	Super::RenderContent();
	ImGui::Text("Input Buffer");

	if(InputConfig == nullptr)
	{
		ImGui::TextDisabled("Invalid Input Configuration");
		return;
	}

	if(AGASCoursePlayerCharacter* PlayerCharacter = Cast<AGASCoursePlayerCharacter>(GetLocalPlayerPawn()))
	{
		if(PlayerCharacter->IsInputBufferOpen())
		{
			ImGui::Text("Input Buffer Is Open!");
		}
		TArray<FGameplayTag> BufferedTags = PlayerCharacter->GetBufferedTags();
		if(BufferedTags.IsEmpty())
		{
			return;
		}
		for(FGameplayTag BufferedTag : BufferedTags)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Buffered Input Tag: %s"), *BufferedTag.ToString());
			//TODO: Make sure that the text fits!
			//ImGui::Text(TEXT("Buffered Input Tag: %s", *BufferedTag.ToString()));
		}
	}
}

void FCogInputBufferWindow::OnInputBufferOpened()
{
	bInputBufferOpened = true;
}

void FCogInputBufferWindow::OnInputBufferClosed()
{
	bInputBufferOpened = false;
}

#endif //ENABLE_COG

