// Shoot THem Up Game. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUpTarget : TargetRules
{
	public ShootThemUpTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
        bUsesSteam = true;
		bUsePCHFiles = false;
		bUseSharedPCHs = false;
        ExtraModuleNames.AddRange( new string[] { "ShootThemUp" } );
	}
}
