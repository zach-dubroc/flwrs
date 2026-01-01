
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "player_camera_component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLWR_BASE_API Uplayer_camera_component : public UActorComponent
{
	GENERATED_BODY()

public:	
	Uplayer_camera_component();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleCam();
	void ZoomIn();
	void ZoomOut();

protected:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* spring_arm;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* fp_camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	FName fp_camera_socket;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float min_arm_length;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float max_arm_length;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float zoom_step;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float zoom_interp_speed;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float target_arm_length;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float current_arm_length;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	bool is_first_person = false;
		
};
