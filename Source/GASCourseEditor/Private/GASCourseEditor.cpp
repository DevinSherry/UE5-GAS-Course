#include "GASCourseEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_GAME_MODULE(FGASCourseEditorModule, GASCourseEditor);

DEFINE_LOG_CATEGORY(GASCourseEditor)

#define LOCTEXT_NAMESPACE "GASCourseEditor"

void FGASCourseEditorModule::StartupModule()
{
	UE_LOG(GASCourseEditor, Warning, TEXT("GASCourseEditor: Log Started"));
}

void FGASCourseEditorModule::ShutdownModule()
{
	UE_LOG(GASCourseEditor, Warning, TEXT("GASCourseEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE