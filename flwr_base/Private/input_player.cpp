
#include "input_player.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

Ainput_player::Ainput_player()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	fp_Camera = CreateDefaultSubobject<UCameraComponent>("fp_Camera");
	Camera->SetupAttachment(SpringArm);
	//TODO: attach to socket/adjust the clipping
	fp_Camera->SetupAttachment(RootComponent);
	fp_Camera->bUsePawnControlRotation = true;
}
void Ainput_player::BeginPlay()
{
	Super::BeginPlay();
	
}
void Ainput_player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void Ainput_player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(input_mapping, 0);
		}
	}

	if (UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		input->BindAction(move_action, ETriggerEvent::Triggered, this, &Ainput_player::Move);
		input->BindAction(look_action, ETriggerEvent::Triggered, this, &Ainput_player::Look);
		input->BindAction(jump_action, ETriggerEvent::Triggered, this, &Ainput_player::Jump);
		input->BindAction(toggle_cam_action, ETriggerEvent::Triggered, this, &Ainput_player::ToggleCam);

	}
}
void Ainput_player::Move(const FInputActionValue& input_value)
{
	FVector2D input_vector = input_value.Get<FVector2D>(); 
	if (IsValid(Controller)) {
		//get direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, input_vector.Y);
		AddMovementInput(RightDirection, input_vector.X);

	}
}

void Ainput_player::Look(const FInputActionValue& input_value)
{
	FVector2D input_vector = input_value.Get<FVector2D>();
	if (IsValid(Controller)) {

		AddControllerYawInput(input_vector.X);
		AddControllerPitchInput(input_vector.Y);
	}
}
void Ainput_player::ToggleCam()
{
	if (IsValid(Controller)) {
		//if clipping can use GetMesh->SetOwnerNoSee
		is_first_person = !is_first_person;
		if (is_first_person) {
			fp_Camera->SetActive(true);
			Camera->SetActive(false);
			// GetMesh()->SetOwnerNoSee(true);
		}
		else {
			fp_Camera->SetActive(false);
			Camera->SetActive(true);
			// GetMesh()->SetOwnerNoSee(false);
		}
	}
}

void Ainput_player::Jump()
{
	ACharacter::Jump();
}


