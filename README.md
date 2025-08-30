# ShootThemUp

This is a game maded by me with Unreal Engine 5.6. After 5 years of unity developing I have decided to start learning Unreal engine. And watch a course on Udemy about Unreal Engine with C++ developing that I completed.

I was really scared of unknown things, but I understand now that Unreal Engine it's a really powerful tool for game developing. And C++ is not that bad how I was thinking at first. After this course I know how the engine work, and all the basics stuff.

Because of my Unity knowledge I was learning very fast, everything it's just the same, in some ways easier.

---

# What I learned by the end of this project:

## Basic stuff
Very basics staff like what is `AActor`, `UPROPERTY`, `UE_LOG`, `USTRUCT`, `UENUM`, `materials`, timers `GetWorld()->GetTimerManager`, spawn actors, how to declare delegates, how to subscribe to them, how to broadcast them. Then I learned `APawn`, `APlayerController`, `ACharacter` understand a little garbage collector.
## Character movement, animations
Then I started to add character. Set up movement, input bindings, camera movement, spring component. I then continued with adding animations, I have learned `Animation Blueprint`, `State Machine`, added walking forward, then walking left, right, left, using blending animations.
<img width="882" height="534" alt="{5FE0D2DE-97A5-409B-BAB4-4F1FE69A8623}" src="https://github.com/user-attachments/assets/d622205e-31b9-4e20-bbc8-f6dca9ccd0fe" />
## Health, damage
I started to experement with `Take Damage`, `Apply damage`, I created `actor component class` for character `UHealthComponent`, this component was subscribing to owner `OnTakeAnyDamage`, and making some arifmetical operations to add, substract health, I then also added `TakeHeal`, this component also have own delegates `OnDamaged`, `OnHealthChanged`.
I then added `ASpectatorPawn`, and fall damage.
<img width="1259" height="705" alt="{A9C34B26-86A6-4AC3-A4AF-E1286C820B26}" src="https://github.com/user-attachments/assets/bf39cbc1-e9f1-41e7-b066-853ef526ffd2" />

## Weapons
With weapons I started by creating `ABaseWeapon` class. I was implementing my first hud, I was drawing crosshair by hand, with c++ in `AHUD` class. Also I created new `actor component` called `UWeaponComponent` for character, that controls everything related to weapons, store them, switch them, calling shoot on them, etc. 
I learned how to use `Line trace`, I have added shooting ability, and `take damage` was called on `HitResult` of line trace. I also added new animations, `animation offset`, using this, I've implemented correct aim animations. Also I used `F.A.B.R.I.K` to attach left hand to bone position. I then added separated classes `RifleWeapon`, `LauncherWeapon`.
`RifleWeapon` was still using `ABaseWeapon`, when `LauncherWeapon` was a child class to `BaseWeapon`, and was overriding function like shoot, reload etc. I programmed rocket spawning, rocket moving, and applying radial damage. I learned `anim notify` using this I created functions CanEquip CanFire, when animations was playing, I was notifying
WeaponComponent that he can't shoot or equip right now because of reload, or equip animations is already playing. I learned `anim montage` and implemented death animation, reload animation.
<img width="491" height="549" alt="{5A6103A0-33FD-4C40-9E10-F313A85CA23E}" src="https://github.com/user-attachments/assets/fe0e3299-cc27-44e4-8b26-f9e7cb34883a" />
<img width="685" height="625" alt="{E5711DD5-ECE7-4571-9EF2-3400ECDDF35E}" src="https://github.com/user-attachments/assets/b0fb0d5d-64d0-4c10-a6b9-209a0c8323ca" />

## UMG
In this section, I learned `Blueprint Widget` or `UUserWidget`, how to use it, how to create it, how to create and control it from code. I added HealthBar using Progress bar, added new crosshair, I was also switching crosshair based on weapon that is character holding right now. I implemented different `UserWidget` for spectator and I was
switching visibility of this when character is dead.
<img width="1441" height="763" alt="{7C10404A-7E91-4888-892C-37BF708F5F0D}" src="https://github.com/user-attachments/assets/35351858-b7ca-4391-9a58-e7b58930c54b" />

## Pickups
Here I created in game items like `HealthPickup` that was adding health when equiped, and `AmmoPickup` that was adding ammo when picked up. For this I created `BasePickup` class that was a parent for this pickups, there is OnPickup function, that is overriding by other pickups class, and there they implementing they functions.
## VFX, Niagara
In this section, I learned `Niagara` and `Cascade` system. For cascade system I just read about this, to first know, what system was before, and then I started to learn and practice with `Niagara`. I added `UWeaponFXComponent` for BaseWeapon class, this class controlling everything related to spawning fx on muzzle when shooting, spawning trace.
I also learned `Decals`, and used them to make holes from bullets when shooting. I also removed my death anim to use more advanced tech called `Ragdoll`. I added shake to the camera when player taking damage, and UI blood effect, using anim and blueprint yet. I also added `Postprocessing` to spectator camera for grain, and grey color effects.

<img width="172" height="201" alt="{6C358CCD-36AF-4C9E-9FB8-E3E6040EFD14}" src="https://github.com/user-attachments/assets/da3dc90d-ea48-4438-afdf-ce615c609eae" />

## AI
In this section I was learning AI. First what I was looking about is `NavMesh volume` to add navigation to scene, `Behavior tree` and `Blackboard`. I first made ai to move in to random scene position. I learned about `Task` and how to create my own in code. Then I learned `UAIPerceptionComponent`, I added `SightSense` for bot, and in `Behavior tree`
I made him look at closest enemy, by learning and implementing `Decorator`, and `Service`, I learned how to own in code, and made him shoot, when `Blackboard` object value enemy was not null, that is checked by Decorator. I added new `WeaponComponent` class for AI, to override functions like shoot, so when ammo is zero, he have to switch to the next
weapon.
<img width="1770" height="720" alt="{FD345CF1-E31E-4C81-A06B-41AB1D4E49C7}" src="https://github.com/user-attachments/assets/09fb19bd-1e6f-4053-a1f9-e44f798ca3d6" />

### EQS
Here I learned about `Environment Query System`. I learned how to use context, how to make random locations, based on `EQS` components, and `Tests`. And with this knowledge I implemented search for health and ammo for bot. I used `find by class`, and `Trace`. I also implemented random position for AI, like when he was looking at enemy, he was choosing
random position, based on EQS, here I was also using my own created `Querier Context` in code, to return enemy that is writed in `Blackboard` object value Enemy. I also created my own `Test` in code, `PickupCouldBeTaken`, it was iterating for each pickup it's found, and set fail to pickups that can't be taken right now, if they respawning for example.
<img width="628" height="311" alt="{CA9EF74D-D731-487F-800D-AA5AA2ABEB0E}" src="https://github.com/user-attachments/assets/b56cd90d-76c6-46da-a778-3b40f4a86eba" />
<img width="756" height="534" alt="{73423E4F-B303-492D-8539-35F14C51E821}" src="https://github.com/user-attachments/assets/24ab2108-9522-47bf-9e8d-6f892e25c10d" />

## Game mode
In this section I learned about `Gamemode` class. I was adding functionality to my own `GamemodeBase` class. I added AI spawns, game time, round number using struct. I learned about `PlayerState` class, I added there information about players kills, deaths, teamid, teamcolor, using struct too. I changed AI to shoot and aim only for
player from enemy team. I added `PlayerCharacter` based on `ABaseCharacter` because previosly, player was just using `ABaseCharacter` class.
<img width="808" height="484" alt="{E4E595AA-C85E-47C3-BBFB-044F93A05275}" src="https://github.com/user-attachments/assets/b8a87b5a-fa81-40e8-8401-52d516c2dde4" />

## UI
Here I learned about `BindWidget`. Added round number, round time, kills number to PlayerHUD widget. Added main menu level. Learned about `UGameInstance` and added there struct of information about levels. I maded UI statistics on game over, I was creating widgets from code, and by overriding `UserWidget` functionality I was using `BindWidget` 
changing values of PlayerName, his kills, his deaths, his team color. I also learned about `UWidgetComponent`, I added HealthBars above AI characters. Then I made UI more beatiful, changed fonts, colors, and other stuff. Learned about `BindWidgetAnim` and how to start it from `UserWidget` code. Made show animation for my own `UserWidgetBase`,
that every other widgets was inhereted from.

<img width="1409" height="748" alt="{7FF958E2-6144-4624-8A6B-421EA06ACBE6}" src="https://github.com/user-attachments/assets/f0689383-1d96-414d-99b6-62ad51b55b2c" />
<img width="260" height="255" alt="{D90E333A-F22A-471F-85E6-233C2BBFEA71}" src="https://github.com/user-attachments/assets/bfc733c1-8f0b-4f5d-a7d0-99d7288faaf6" />

## Sound
Here I learned about `USoundCue`, `USoundWave`. Added sounds for buttons. Learned how to add sound to animations using on notify. I added walk, run, jump, land, reload, equip sounds. I learned about `USoundClass` used it for structure sounds to types, character sounds, weapon sounds etc. I learned about `Sound Attenuation`, and implement it to
my sounds. I then learned how to using `UGameplayStatics` spawn sounds in code, so I added shoot, death, respawn, no ammo, pick up, impact sounds. I also learned about `BlueprintFunctionLibrary`, and `USoundMix` but didn't used it in this game.
## Last changes
Here I was just adding and modifying more stuff to make it look, and play better. For example I added zoom for weapons, headshot, and leg damage multipliers by using bonename of mesh and physics material.

<img width="857" height="652" alt="{0662B160-C9B6-4ED9-9AC0-4774D22E7226}" src="https://github.com/user-attachments/assets/6001a8aa-062e-4588-ba7f-fbaaf34e96d5" />

