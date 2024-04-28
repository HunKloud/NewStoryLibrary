class CfgPatches
{
	class NewStoryGroupLibrary
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = { "DZ_Scripts" };
	};
};

class CfgMods
{
	class NewStoryGroupLibrary
	{
		name = "NSG Library";
		author = "HunKloud";
		authorID = "76561197972196282";
		credits = "NEW STORY GROUP";
		version = 1.0;
		type = "mod";
		dependencies[] = { "Game", "World", "Mission" };
		class defs
		{
			class imageSets
			{
				files[] = { "NewStoryLibrary/imagesets/slots.imageset" };
			};
			class gameScriptModule
			{
				value = "";
				files[] = { "NewStoryLibrary/Scripts/3_Game" };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "NewStoryLibrary/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "NewStoryLibrary/Scripts/5_Mission" };
			};
		};
	};
};
