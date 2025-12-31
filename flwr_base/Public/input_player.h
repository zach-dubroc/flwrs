
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "input_player.generated.h"

UCLASS()
class FLWR_BASE_API Ainput_player : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* fp_Camera;


protected:
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputMappingContext* input_mapping;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* move_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* look_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* jump_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* sprint_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* toggle_cam_action;
		
	UPROPERTY(VisibleAnywhere, Category="Camera")
	bool is_first_person = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	USceneComponent* fp_CameraPivot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta = (AllowPrivateAccess = "true"))
	FName fp_CameraSocket = "head";


	UPROPERTY(EditDefaultsOnly, Category="Camera")
	float camera_blend_time = 0.5f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool is_sprinting = false;
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float walk_speed = 600.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float sprint_speed = 1200.f;


public:
	Ainput_player();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	void Move(const FInputActionValue& input_value);
	void Look(const FInputActionValue& input_value);
	void ToggleCam();
	void Jump();
	void SprintStop(const FInputActionValue& input_value);
	void SprintStart(const FInputActionValue& input_value);
};
