
#include "player_camera_component.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"

Uplayer_camera_component::Uplayer_camera_component()
{
	PrimaryComponentTick.bCanEverTick = true;

	fp_camera_socket = "cam_socket";
	min_arm_length = 0.f;
	max_arm_length = 400.f;
	zoom_step = 80.f;
	zoom_interp_speed = 10.f;
	target_arm_length = max_arm_length;
	current_arm_length = max_arm_length;

}


void Uplayer_camera_component::BeginPlay()
{
	Super::BeginPlay();
	ACharacter* owner = Cast<ACharacter>(GetOwner());

	spring_arm = NewObject<USpringArmComponent>(owner, TEXT("spring_arm"));
	spring_arm->SetupAttachment(owner->GetMesh(), fp_camera_socket);
	spring_arm->RegisterComponent();
	spring_arm->bUsePawnControlRotation = true;
	spring_arm->bEnableCameraLag = true;
	spring_arm->CameraLagSpeed = 40.f;

	fp_camera = NewObject<UCameraComponent>(owner, TEXT("fp_Camera"));
	fp_camera->SetupAttachment(spring_arm);
	fp_camera->RegisterComponent();
	fp_camera->bUsePawnControlRotation = true;
	
}


void Uplayer_camera_component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	current_arm_length = FMath::FInterpTo(current_arm_length, target_arm_length, DeltaTime, zoom_interp_speed);
	spring_arm->TargetArmLength = current_arm_length;

}

void Uplayer_camera_component::ToggleCam()
{
	is_first_person = !is_first_person;
	if (is_first_person) {
		//todo: set spring arm length
		UE_LOG(LogTemp, Warning, TEXT("first person"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("third person"));
	}
}

void Uplayer_camera_component::ZoomIn()
{
	target_arm_length = FMath::Clamp(target_arm_length - zoom_step, min_arm_length, max_arm_length);
}

void Uplayer_camera_component::ZoomOut()
{
	target_arm_length = FMath::Clamp(target_arm_length + zoom_step, min_arm_length, max_arm_length);
}

