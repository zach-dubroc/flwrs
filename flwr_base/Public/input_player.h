
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "input_player.generated.h"

UCLASS()
class FLWR_BASE_API Ainput_player : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* spring_arm;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* fp_camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta = (AllowPrivateAccess = "true"))
	FName fp_camera_socket = "cam_socket";

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector fp_cam_offset = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector tp_cam_offset = FVector(-160.f, 110.f, 0.f);

	FVector current_cam_offset;
	FVector target_cam_offset;

	UPROPERTY(EditAnywhere, Category="Camera")
	float min_arm_length = 0.f;
	UPROPERTY(EditAnywhere, Category="Camera")
	float max_arm_length = 300.f;
	UPROPERTY(EditAnywhere, Category="Camera")
	float zoom_step = 80.f;
	UPROPERTY(EditAnywhere, Category="Camera")
	float zoom_interp_speed = 20.f;
	UPROPERTY(EditAnywhere, Category="Camera")
	float target_arm_length;
	UPROPERTY(EditAnywhere, Category="Camera")
	float current_arm_length;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float tp_arm_length = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float fp_arm_length = 0.f;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	bool is_first_person = false;

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
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* zoom_in_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* zoom_out_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* fire_action;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool is_sprinting = false;
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float walk_speed = FMath::Clamp(600.f, 150.f, 1200.f);
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float sprint_speed = FMath::Clamp(1200.f, walk_speed + 1.f, 2400.f);



	/////WEAPONS 



	FTimerHandle fire_timer_handle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool is_firing = false;

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
	void ZoomIn(const FInputActionValue& input_value);
	void ZoomOut(const FInputActionValue& input_value);

};
