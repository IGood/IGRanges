// Copyright Ian Good

using UnrealBuildTool;

public class IGRanges : ModuleRules
{
	public IGRanges(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
		});
	}
}
