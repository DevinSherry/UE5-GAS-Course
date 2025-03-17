// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "GASC_TimeDilation_Subsystem.generated.h"

/**
 * @enum EGASC_TimeDilation_Priority
 * @brief Defines the priority levels for time dilation effects within the game.
 *
 * This enumeration is used to specify the importance or precedence of time dilation effects,
 * allowing systems to determine which effects to prioritize or override in scenarios where
 * multiple time dilation requests are active. It includes predefined levels such as Low,
 * Normal, High, and Critical.
 */

UENUM(BlueprintType)
enum class EGASC_TimeDilation_Priority : uint8
{
	Low,
	Normal,
	High,
	Critical,
};

/**
 * @struct FGASC_TimeDilation_Subsystem_Data
 * @brief Represents the data structure used for configuring and managing time dilation effects in a game subsystem.
 *
 * This structure provides configurable parameters for applying time dilation to gameplay scenarios. It supports
 * both standard time dilation settings and advanced configurations using customizable curves. Additionally, priority
 * levels and affected actors can be specified for precise control over the behavior and impact of the time dilation effect.
 *
 * Key features include:
 * - Basic time dilation with customizable intensity and duration.
 * - Support for smooth transitions using blend modes and custom curves.
 * - Hierarchical prioritization for resolving conflicts between multiple effects.
 * - Options for globally or locally affecting specific actors.
 */
USTRUCT(BlueprintType)
struct FGASC_TimeDilation_Subsystem_Data
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeDilation", meta = (ClampMin = "0.01"))
	float TimeDilation = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeDilation")
	float TimeDilationDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeDilation")
	bool bUseTimeDilationCurve = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeDilation", meta = (EditCondition = "bUseTimeDilationCurve", EditConditionHides))
	EAlphaBlendOption BlendMode = EAlphaBlendOption::Custom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeDilation",  meta = (EditCondition = "BlendMode == EAlphaBlendOption::Custom",EditConditionHides))
	UCurveFloat* CustomTimeDilationCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeDilation")
	EGASC_TimeDilation_Priority Priority = EGASC_TimeDilation_Priority::Normal;

	float ElapsedTime;
	TArray<AActor*> AffectedActors;

	FGASC_TimeDilation_Subsystem_Data() : TimeDilationDuration(0.0f), ElapsedTime(0.0f) {}
	FGASC_TimeDilation_Subsystem_Data(float InTimeDilationDuration) : TimeDilationDuration(InTimeDilationDuration), ElapsedTime(0.0f) {}

};

/**
 * @class UGASC_TimeDilation_Subsystem
 * @brief Manages and applies time dilation effects within the game.
 *
 * This class provides functionality for controlling the flow of time at a subsystem level,
 * allowing for dynamic adjustments such as slowing down or speeding up time. It includes methods
 * to configure, start, update, and stop time dilation effects based on predefined parameters.
 *
 * The subsystem integrates seamlessly with other gameplay mechanics, enabling scenarios like
 * dramatic slow-motion moments or fast-paced action sequences.
 */
UCLASS()
class GASCOURSE_API UGASC_TimeDilation_Subsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	/**
	 * @brief Adds a global hit stop effect with the specified data.
	 *
	 * This method allows adding a global time dilation effect to the system using
	 * the provided data structure. If there is an existing active global time dilation
	 * and the new data has a higher priority, it will replace the current one.
	 * Otherwise, the new data will be added to the queue to be processed later.
	 *
	 * @param HitStopData The data defining the global hit stop, including time dilation
	 * scale, duration, and optional curve for progression.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|TimeDilation")
	void AddGlobalHitStopWithData(FGASC_TimeDilation_Subsystem_Data HitStopData);

	/**
	 * @brief Adds a local hit stop effect to specific actors using the provided data.
	 *
	 * This method applies a time dilation effect to a set of actors defined in the AffectedActors array.
	 * If there is an existing active local time dilation effect and the new data has a higher priority,
	 * it will replace the current effect. Otherwise, the new data is enqueued for later processing.
	 *
	 * @param HitStopData A structure containing the time dilation scale, duration, progression curve, and
	 * priority for the local hit stop effect.
	 * @param AffectedActors An array of actors that the local hit stop effect will be applied to.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|TimeDilation")
	void AddLocalHitStopWithData(FGASC_TimeDilation_Subsystem_Data HitStopData, TArray<AActor*> AffectedActors);

	/**
	 * @brief Adds a global time dilation effect with customizable parameters including a progression curve.
	 *
	 * This method applies a global time dilation effect to the system using the provided parameters.
	 * If an active global time dilation exists and the new effect has a higher priority,
	 * the new effect replaces the existing one. Otherwise, it is added to the queue to be processed later.
	 *
	 * @param TimeDilationDuration The duration, in seconds, for which the time dilation effect will be applied.
	 * @param TimeDilationMinValue The minimum time dilation value to apply. A value less than 1 slows time, greater than 1 speeds it up.
	 * @param AlphaBlendOption Specifies the blending mode to transition into the time dilation effect. Example: Linear or Cubic.
	 * @param Curve A float curve defining the progression of time dilation over the specified duration. If null, a constant time dilation is applied.
	 * @param Priority The priority of this time dilation effect. Higher priority effects override lower priority ones.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|TimeDilation")
	void AddGlobalHitStopWithCurve(float TimeDilationDuration = 1.0f, float TimeDilationMinValue = 1.0f, EAlphaBlendOption AlphaBlendOption = EAlphaBlendOption::Linear,
		UCurveFloat* Curve = nullptr, EGASC_TimeDilation_Priority Priority = EGASC_TimeDilation_Priority::Normal);

	/**
	 * @brief Adds a local hit stop effect with a specified time dilation curve and properties.
	 *
	 * This method allows customizing a local hit stop effect by specifying the duration, minimum time dilation,
	 * blending options, priority and optionally affected actors. If an active local time dilation is already present,
	 * this method will override it if the priority of the new configuration is higher.
	 *
	 * @param TimeDilationDuration The duration, in seconds, for which the time dilation effect should be applied.
	 * @param TimeDilationMinValue The minimum time dilation value for the effect. Lower values slow time down further.
	 * @param AlphaBlendOption The blending mode used to interpolate the time dilation effect over its duration.
	 * @param Curve An optional float curve that defines how the time dilation progresses over its duration.
	 * If null, the effect will use the specified blending mode.
	 * @param Priority The priority level of the time dilation effect. Effects with higher priority override lower ones.
	 * @param AffectedActors A list of actors that should be specifically affected by this time dilation effect.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|TimeDilation")
	void AddLocalHitStopWithCurve(TArray<AActor*> AffectedActors, float TimeDilationDuration = 1.0f, float TimeDilationMinValue = 1.0f, EAlphaBlendOption AlphaBlendOption = EAlphaBlendOption::Linear, UCurveFloat* Curve = nullptr,
		EGASC_TimeDilation_Priority Priority = EGASC_TimeDilation_Priority::Normal);

	/**
	 * @brief Applies a global hit stop effect with specified parameters.
	 *
	 * This method adds a global hit stop to the subsystem by temporarily adjusting time dilation
	 * with a given duration and minimum time dilation value. If another global hit stop is already
	 * active, this method compares their priorities and replaces the current hit stop if the new one
	 * has a higher priority.
	 *
	 * @param TimeDilationDuration The duration of the hit stop effect in seconds.
	 * @param TimeDilationMinValue The minimum value of time dilation to be applied during the hit stop effect.
	 *                             A value less than 1 slows time, while a value of 0 freezes time completely.
	 * @param Priority The priority level for this hit stop effect. Higher priority effects override
	 *                 lower priority ones. Defaults to `EGASC_TimeDilation_Priority::Normal`.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|TimeDilation")
	void AddGlobalHitStop(float TimeDilationDuration = 1.0f, float TimeDilationMinValue = 1.0f, EGASC_TimeDilation_Priority Priority = EGASC_TimeDilation_Priority::Normal);

	/**
	 * @brief Adds a local hit stop effect with a specified time dilation for a defined duration, priority, and affected actors.
	 *
	 * This method applies a time dilation effect locally to the subsystem, affecting specific actors.
	 * It replaces the currently active local hit stop if the new effect has a higher priority.
	 * Otherwise, the new effect is enqueued and will be applied when the current one finishes.
	 *
	 * @param TimeDilationDuration The duration, in seconds, for which the time dilation effect is applied.
	 * @param TimeDilationMinValue The minimum value of the time dilation to apply, determining how slow time becomes.
	 * A value closer to 0 results in more significant slowing of time.
	 * @param Priority The priority level of the hit stop effect. Higher priority effects override lower priority ones.
	 * Defaults to EGASC_TimeDilation_Priority::Normal.
	 * @param AffectedActors The array of actors that will be affected by the time dilation effect. Only these actors
	 * will experience the time dilation specified by this method.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|TimeDilation")
	void AddLocalHitStop(TArray<AActor*> AffectedActors, float TimeDilationDuration = 1.0f, float TimeDilationMinValue = 1.0f,
		EGASC_TimeDilation_Priority Priority = EGASC_TimeDilation_Priority::Normal);

	/**
	 * @brief Stops all active time dilation requests in the subsystem.
	 *
	 * This method terminates both global and local time dilation requests, ensuring that any
	 * ongoing time modifications are reset to their default state. It is useful for restoring
	 * the normal flow of time within the subsystem. No specific conditions are required to
	 * invoke this method.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|TimeDilation")
	void StopAllTimeDilationRequests();

	/**
	 * @brief Stops and clears all active local time dilation requests managed by this subsystem.
	 *
	 * This method restores the time dilation of all currently affected actors to their default value
	 * and cleans up the associated active time dilation request data.
	 *
	 * It ensures that no local time dilation effects remain active in the subsystem after execution.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|TimeDilation")
	void StopAllLocalTimeDilationRequests();

	/**
	 * @brief Stops and clears all active global time dilation requests within the subsystem.
	 *
	 * Resets any active global time dilation effect to the default value (1.0) and releases associated resources.
	 * This method ensures that no leftover time dilation effects remain in the subsystem.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|TimeDilation")
	void StopAllGlobalTimeDilationRequests();

	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}

private:
	
	TQueue<FGASC_TimeDilation_Subsystem_Data> GlobalTimeDilationQueue;
	TQueue<FGASC_TimeDilation_Subsystem_Data> LocalTimeDilationQueue;
	
	FGASC_TimeDilation_Subsystem_Data* ActiveGlobalTimeDilation;
	FGASC_TimeDilation_Subsystem_Data* ActiveLocalTimeDilation;

	void ProcessGlobalHitStops(float DeltaTime);
	void ProcessLocalHitStops(float DeltaTime);

	bool bWorldBeginPlay = false;
	
};
