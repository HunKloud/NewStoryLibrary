
// class CfgVehicles
// {
// 	class AnyItem
// 	{
// 		hiddenSelections[] =
// 		{
// 			***,
// 			"selection1",
// 			"selection2",
// 			"selection3",
// 			"selection4",
// 			"selection5",
// 			"selection6",
// 			***
//		};
//		// логика не меняется
// 		hologramMaterialPath = "base material folder";
//		// это значение полностью заменяет логику поведения выставления материалов у голограммы
// 		//  поэтому прописывать 'hologramMaterial' не имеет никакого смысла
// 		NSgHologramSelections[] =
// 		{
// 			"selection1",
// 			"selection2",
// 			"selection3",
// 			"selection4",
// 			"selection5",
// 			"selection6"
// 		};
// 		// Порядок должен быть таким же как и в 'NSgHologramSelections'
// 		//  итоговый путь для селекшена будет выглядеть как 'hologramMaterial + \\ + NSgHologramMaterials[n]'
// 		NSgHologramMaterials[] =
// 		{
// 			// суффикс ставится по той же самой логике, что и в оригинальном варианте
// 			"hologram\\material_selection1", 
// 			"hologram\\material_selection2",
// 			"hologram\\material_selection3",
// 			"hologram\\material_selection4",
// 			// оставляет тот материал который был выставлен в самой модели
// 			"", 
// 			"hologram\\material_selection6"
// 		};
// 	};
// };

class NSgHologram: Hologram
{
	static const string TEXTURE_NAME = "#(argb,8,8,3)color(0.5,0.5,0.5,0.75,ca)";
	
	protected ref array<ref Param2<int, string>> m_MaterialForSelection;

	override void SetAnimations()
	{
		LoadNSgHologramSelections();
		super.SetAnimations();
	}

	protected void LoadNSgHologramSelections()
	{
		string type = m_Projection.GetType();

		string selectionsConfigPath = string.Format( "CfgVehicles %1 NSgHologramSelections", type );
		string materialsConfigPath = string.Format( "CfgVehicles %1 NSgHologramMaterials", type );

		if ( !GetGame().ConfigIsExisting( selectionsConfigPath ) ) {
			return;
		}

		if ( !GetGame().ConfigIsExisting( materialsConfigPath ) ) {
			return;
		}

		array<string> selections = new array<string>();
		array<string> materials = new array<string>();

		GetGame().ConfigGetTextArray( selectionsConfigPath, selections );
		GetGame().ConfigGetTextArray( materialsConfigPath, materials );

		int hiddenSelection = -1;

		for ( int i = selections.Count() - 1; i >= 0; i-- ) {
			hiddenSelection = m_Projection.GetHiddenSelectionIndex( selections[i] );

			if ( hiddenSelection == -1 ) {
				continue;
			}

			if ( !m_MaterialForSelection ) {
				m_MaterialForSelection = new array<ref Param2<int, string>>();
			}

			m_MaterialForSelection.Insert( new Param2<int, string>( hiddenSelection, materials[i] ) );
		}
	}

	override void RefreshVisual()
	{
		if ( !m_Projection || !m_MaterialForSelection ) {
			super.RefreshVisual();
			return;
		}

		string materialFolderConfigPath = string.Format( "CfgVehicles %1 hologramMaterialPath", m_Projection.GetType() );
		string materialFolder = GetGame().ConfigGetTextOut( materialFolderConfigPath );
		string hologramMaterialPath = "";

		Param2<int, string> selectionData;

		for ( int i = 0; i < m_MaterialForSelection.Count(); i++ ) {
			selectionData = m_MaterialForSelection[i];

			m_Projection.SetObjectTexture( selectionData.param1, TEXTURE_NAME );

			if ( selectionData.param2 != "" ) {
				hologramMaterialPath = string.Format( "%1\\%2%3", materialFolder, selectionData.param2, CorrectMaterialPathName() );
				m_Projection.SetObjectMaterial( selectionData.param1, hologramMaterialPath );
			}
		}
	}
}
