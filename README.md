# ShootThemUp
First Steam multiplayer build here! Check it out with you friends, download 1.1 release!

Currently working on teams implementation on [Teams branch](https://github.com/1Laggy1/ShootThemUp/tree/Teams).

A third person shooter built in Unreal Engine 5.6. Demonstrates core gameplay systems, like AI, UI, VFX, and clean component-based architecture that was implemented using C++.
## Summary
ShootThemUp - is a project made by me while learning Unreal Engine 5.6 after 5 years of Unity developing. In this project I have implemented a full gameplay loop (movement, weapons, health, AI, pickups, UI, VFX, sound, and game flow) and is designed to show practical engineering decisions and reusable systems.
## Highlights
- Fully working multiplayer (`Replicated`, `RPC`, `Lobby`)

<img width="1978" height="1237" alt="{C8F44DBB-BF43-42FE-AC2C-A6A3E115541D}" src="https://github.com/user-attachments/assets/48c0b6e3-e41d-4501-9a7c-9cd781da7731" />

- Full movement, animations, weapons shooting, equiping, replication between clients, and host.

![Multiplayer](https://github.com/user-attachments/assets/15a20c3f-78b4-4851-a782-56db97237731)

- Complete third-person character with animations (`blendspaces`, `montages`, `notifies`, `ragdoll`, `aim offsets`, `F.A.B.R.I.K`).

![Animations](https://github.com/user-attachments/assets/cd30227c-395b-4a5e-8e58-e2052bb77982)

<img width="1450" height="357" alt="{A0E66C4F-9895-48FA-B7A3-459D8FB684FA}" src="https://github.com/user-attachments/assets/3d9c327b-a629-4d49-85e6-d44c67022a93" />

- Weapon system using self created Actor component `WeaponComponent` that is controlling weapons that character have (shooting, equipping, reloading), and `ABaseWeapon` class that implementing all of this methods shoot, reload, and inherited classes `RifleWeapon` and `LauncherWeapon` with overrided methods, for their use.
`LineTrace` for rifle weapon, and `UGameplayStatics::ApplyRadialDamage` for Launcher radial damage.

![WeaponComponent](https://github.com/user-attachments/assets/f154198a-d45e-44f7-9a4e-614368fa509b)

- Health system using self created Actor component `HealthActorComponent`. Subscribed to GetOwner delegates OnTakeDamage it uses his own methods to determine whether the health will be changed, and broadcasting the OnHealthChanged delegate to notify other systems of health changes.

![HealthComponent](https://github.com/user-attachments/assets/921f0de3-4106-486b-827e-4874a3f55836)

- AI using `NavMesh`, `UAIPerception`, `Behavior Tree` + `Blackboard`, built in and self created `EQS tests`, `Tasks`, `Services`.

EQS:

![EQS](https://github.com/user-attachments/assets/22168e59-3486-486c-8c11-9b615b741a67)

![EQSSecond](https://github.com/user-attachments/assets/3357ee4b-e7bd-4604-b2ab-96a5d9e4031d)

AI finding Healing pickup:

![EQSHealing](https://github.com/user-attachments/assets/6767bfcb-07d8-4739-a069-bae9e1923d75)

AI going to last enemy position:

![AILastEnemyPosition](https://github.com/user-attachments/assets/3c74d9ec-166c-4a10-af95-34aff7276082)

AI Damage sense:

![SenseDamage](https://github.com/user-attachments/assets/125b15e9-763f-4b99-8e15-9e79fa72c1d5)

<img width="1738" height="659" alt="{0FA8BD63-E554-4B34-8E84-C73549C9F766}" src="https://github.com/user-attachments/assets/f30f95c3-c44f-49cd-a2d1-013b5067cad9" />

- UMG HUD and self created `UserWidgets` driven from c++ with data binding using `BindWidget` and `BindWidgetAnim` (Health, ammo, scoreboard, spectator UI, levels).

<img width="2560" height="1440" alt="{E21245B3-9AF3-4645-84A9-E1F4CB3851C5}" src="https://github.com/user-attachments/assets/2427c077-c97f-4568-8ef4-cd4c827a7ee2" />

<img width="2560" height="1440" alt="{F7E8ED97-B0DE-4327-A53B-949572A10E15}" src="https://github.com/user-attachments/assets/b5e7c98b-1f16-4dbf-9d8b-2a38fcd92783" />


- VFX `Niagara`, rifle line traces, muzzle effects using `sockets` on meshes also bullet decals, camera shaking, post-process effects.

![VFXLauncher](https://github.com/user-attachments/assets/908930e7-41a4-4fec-9e93-74385d1ebff5)

- Full sound integrations (`Sound cues`, `Sound Classes`, `Attenuations`, `Animation notifies`)

[Sound test youtube link](https://youtu.be/W4pmJsOnAKk)

- Clean code organization: Separated gameplay components such as `HealthActorComponent` `WeaponComponent` `WeaponFXComponent`, all code is writed in places where it should be, `Player stat` have everything related to stats, and settings of player like TeamID, Kills, Deaths. `GamemodeBase` driven class have everything related to gamemode
(respawning players, score count and show), etc.
## Technical details
- `Actors`, `Pawns`, `Characters`, `Components`, `Gamemode`/`GameState`/`PlayerState`
- Animation systems Blueprint + C++ and OnNotify hooks.
- AI systems: `Behavior Tree`, `EQS`, `Perception`, custom C++ `Tasks`, `Services`, `Decorators`, `EQS tests`.
- UMG + C++ widget bindings and widget animation control
- `Niagara` and `Decal` usage, `Mesh Sockets`, `Physic Materials`, working with `Bone` names.
- Creating, subscribing and broadcasting `Delegates`, OnTakeDamage, OnHealthChanged, OnAnimEnd etc., working with timers (WorldTimerManager).
- Full sound creation `Sound Cue`, `Sound wave`, `Attenuation`, `UAudioComponent`, `USoundClass`, `USoundMix`
## [Gameplay youtube link](https://youtu.be/bS7BoY_a3ME)

## How to run
### Project
- Install Unreal Engine 5.6 from Epic Games launcher, or compile from Source.
- Clone this repository `git clone github.com/1Laggy1/ShootThemUp`.
- If tou are using Visual Studio generate project files (right-click .uproject -> Generate Visual Studio project)
- Start up .uproject file or Unreal Engine -> Open Project and project will be open.
### Game
- You can simply download recent Release in Release section, this is for windows only.
## Next steps
- Multiplayer (`Replication`, steam integration).
- Add abilities to players that they can choose.
- Add more weapons to players that they can choose.
- Add more gamemodes (Flag capture, King of the hill).
## Notes
With this project I almost fully transition my engineering skills from Unity C# to Unreal Engine C++. I prioritized creating reusable systems, and now they can be extended for larger prototypes.

## License

Copyright (c) 2025 Andrew Shchigol.  
All rights reserved.

This repository is for **viewing and educational purposes only**.  
You are **not allowed to copy, modify, distribute, or use the code** in your own projects, whether commercial or non-commercial, without explicit permission from the author.
