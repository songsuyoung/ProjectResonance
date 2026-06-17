
#pragma once

#include "CoreMinimal.h"
#include "Character/RBaseCharacter.h"
#include "RCameraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class RESONANCE_API ARCameraCharacter : public ARBaseCharacter
{
	GENERATED_BODY()
	
public:
	ARCameraCharacter();
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

};
