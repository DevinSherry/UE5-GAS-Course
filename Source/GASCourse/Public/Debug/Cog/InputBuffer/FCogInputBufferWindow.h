// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CogCommon.h"
#include "Game/Input/GASCourseInputConfig.h"

#if ENABLE_COG

#include "CogWindow.h"

/**
 * @class FCogInputBufferWindow
 *
 * @brief Class representing a window for input buffer in a cognitive system.
 */

class FCogInputBufferWindow : public FCogWindow
{
	typedef FCogWindow Super;

public:

	virtual void Initialize() override;

	UGASCourseInputConfig* GetInputConfig();

protected:

	virtual void RenderHelp() override;

	virtual void RenderContent() override;

	UFUNCTION()
	void OnInputBufferOpened();
	
	UFUNCTION()
	void OnInputBufferClosed();

private:

	TObjectPtr<UGASCourseInputConfig> InputConfig = nullptr;

	bool bInputBufferOpened = false;
	
};
#endif //ENABLE_COG