# ShootThemUp

Full multiplayer and soccer mode here! Check it out with you friends, download 1.2.1 release!

A third person shooter built in Unreal Engine 5.6, focused on Embark Studios Dynamism genre. Demonstrates core gameplay systems, like AI, UI, VFX, and clean component-based architecture that was implemented using C++.

## Summary
ShootThemUp - is a project made by me while learning Unreal Engine 5.6 after 5 years of Unity developing. In this project I have implemented a full gameplay loop (movement, weapons, health, AI, pickups, UI, VFX, sound, steam multiplayer) and is designed to show practical engineering decisions and reusable systems.

## Videos of Gameplay
### [STU 1.2.1 Soccer Gameplay](https://youtu.be/-2R0pDLVn30)
Older videos:

[STU 1.2 Multiplayer Gameplay youtube link](https://youtu.be/EuI6q9YkiPk)

[ STU 1.0 Gameplay youtube link](https://youtu.be/bS7BoY_a3ME)

## Highlights

- Fully working multiplayer through Steam relay, with working steam invites (`Replicated` variables, `RPC` methods, Fully working Lobby)

![Steam](https://github.com/user-attachments/assets/8c9938ab-ed5f-49e1-834c-bb1f36f89a58)

- This is the lobby, you can see every player here. Change your weapon, change team color, and team name. Also the host can change the level to play on:

![Lobby](https://github.com/user-attachments/assets/2bd03367-5b88-4f6b-a379-200f52528f3f)

- Full movement, animations, weapons shooting, equiping, replication between clients, and host.

https://github.com/user-attachments/assets/08498f1e-a5b4-4016-a251-f1b7824aec4c

![Multiplayer](https://github.com/user-attachments/assets/15a20c3f-78b4-4851-a782-56db97237731)

- Complete third-person character with animations (`blendspaces`, `montages`, `notifies`, `ragdoll`, `aim offsets`, `F.A.B.R.I.K`).

![Animations](https://github.com/user-attachments/assets/cd30227c-395b-4a5e-8e58-e2052bb77982)

<img width="1450" height="357" alt="{A0E66C4F-9895-48FA-B7A3-459D8FB684FA}" src="https://github.com/user-attachments/assets/3d9c327b-a629-4d49-85e6-d44c67022a93" />

- Weapon system using self created Actor component `WeaponComponent` that is controlling weapons that character have (shooting, equipping, reloading), and `ABaseWeapon` class that implementing all of this methods shoot, reload, and inherited classes `RifleWeapon` and `LauncherWeapon` with overrided methods, for their use.
`LineTrace` for rifle weapon, and `UGameplayStatics::ApplyRadialDamage` for Launcher radial damage.

![WeaponComponent](https://github.com/user-attachments/assets/f154198a-d45e-44f7-9a4e-614368fa509b)

- Useable component, and PlayerUseComponent, to interact with objects.

![Soccer(2)](https://github.com/user-attachments/assets/bb677721-b998-456b-bcf1-315d52bac8ee)


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

<img width="2560" height="1363" alt="LobbyUI" src="https://github.com/user-attachments/assets/cace92ca-9116-4c98-80e7-6c8b52ba1e66" />

<img width="2560" height="1363" alt="GameOverUI" src="https://github.com/user-attachments/assets/cc252c38-cb07-462a-af97-c33680556bca" />

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

## Next steps
- ~~Multiplayer (`Replication`, steam integration).~~ Done.
- Add abilities to players that they can choose.
- Add more weapons to players that they can choose.
- Add random spawned weapons that players can pick up.
- ~~Add main soccer like gamemode.~~ Done.
- Add objects that players can "Hack" making them control environment.
- Add simple destruction.

## How to run
### Game
- You can simply download recent Release in Release section, this is for windows only but you can try proton on linux which I not tested.
### Project
- Install Unreal Engine 5.6 from Epic Games launcher, or compile from Source.
- Clone this repository `git clone github.com/1Laggy1/ShootThemUp`.
- If you are using Visual Studio generate project files (right-click .uproject -> Generate Visual Studio project)
- Start up .uproject file or Unreal Engine -> Open Project and project will be open.

## Notes
With this project I fully transition my engineering skills from Unity C# to Unreal Engine C++. I prioritized creating reusable systems, so I can now extend them.

## License

Copyright (c) 2025 Andrew Shchigol.  
All rights reserved.

This repository is for **viewing and educational purposes only**.  
You are **not allowed to copy, modify, distribute, or use the code** in your own projects, whether commercial or non-commercial, without explicit permission from the author.
