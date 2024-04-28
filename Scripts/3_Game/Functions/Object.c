typedef NewStoryGroupObjectFunction NewStoryGroupObjectFunctuon;

class NewStoryGroupObjectFunction
{
	static void GetSideCenters(notnull Object obj, vector centers[6])
	{
		vector minmax[2];
		obj.GetCollisionBox( minmax );

		vector min = minmax[0];
		vector max = minmax[1];

		vector c = ( max + min ) * 0.5;

		centers[0] = Vector( c[0], 		max[1], 	c[2] );
		centers[1] = Vector( c[0], 		min[1], 	c[2] );
		centers[2] = Vector( max[0], 	c[1], 		c[2] );
		centers[3] = Vector( min[0], 	c[1], 		c[2] );
		centers[4] = Vector( c[0], 		c[1], 		max[2] );
		centers[5] = Vector( c[0], 		c[1], 		min[2] );
	}

	static vector GetCenter(notnull Object obj)
	{
		vector minmax[2];
		obj.GetCollisionBox( minmax );
		return ( minmax[1] + minmax[0] ) * 0.5;
	}

	static float GetCollisionHeight(notnull Object obj)
	{
		vector minmax[2];
		obj.GetCollisionBox( minmax );
		return minmax[1][1] - minmax[0][1];
	}

	static vector GetCollisionBoxSize(notnull Object obj)
	{
		vector minmax[2];
		obj.GetCollisionBox( minmax );		
		return Vector( minmax[1][0] - minmax[0][0], minmax[1][1] - minmax[0][1], minmax[1][2] - minmax[0][2] );
	}

	static float GetDiagonalBox(notnull Object obj, bool ignore_height = false)
	{
		vector size = GetCollisionBoxSize(obj);

		if ( ignore_height ) {
			return Math.Sqrt( Math.SqrFloat( size[0] ) + Math.SqrFloat( size[2] ) );
		}
		
		return Math.Sqrt( Math.SqrFloat( size[0] ) + Math.SqrFloat( size[1] ) + Math.SqrFloat( size[2] ) );
	}

	static float GetRadiusBox(notnull Object obj, bool ignore_height = false)
	{
		return GetDiagonalBox( obj, ignore_height ) / 2;
	}

	static int GetItemSize(notnull Object obj)
	{
		array<int> configSize = new array<int>();
		obj.ConfigGetIntArray( "itemSize", configSize );

		int itemSize = 1;

		foreach (int size: configSize) {
			itemSize *= size;
		}

		return itemSize;
	}

	static int GetItemSize(string type_name)
	{
		string itemPath = GetItemPath(type_name) + " itemSize";
		array<int> configSize = new array<int>();

		GetGame().ConfigGetIntArray( itemPath, configSize );

		int itemSize = 1;

		foreach (int size: configSize) {
			itemSize *= size;
		}

		return itemSize;
	}

	static string GetItemPath(string type_name)
	{
		if ( GetGame() && type_name != "" ) {
			if ( GetGame().ConfigIsExisting( "CfgVehicles " + type_name ) ) {
				return "CfgVehicles " + type_name;
			}
			else if ( GetGame().ConfigIsExisting( "CfgWeapons " + type_name ) ) {
				return "CfgWeapons " + type_name;
			}
			else if ( GetGame().ConfigIsExisting( "CfgMagazines " + type_name ) ) {
				return "CfgMagazines " + type_name;
			}
		}

		return "";
	}

	// Аналог 'KindOf' в 'Object'
	static bool ItemKindOf(string type_name, string tag, string cfg_path = "")
	{
		if ( cfg_path == "" ) {
			cfg_path = GetItemPath( type_name );
		}

		if ( cfg_path == "" ) {
			return false;
		}

		array<string> tags = new array<string>();
		GetGame().ConfigGetTextArray( cfg_path + " itemInfo", tags );

		for (int i = 0; i < tags.Count(); i++) {
			if ( tags[i] == tag ) {
				return true;
			}
		}

		return false;
	}

	// 'type_name' не обязателен к заполнению, если имеется 'cfg_path'
	static bool ItemIsKindOf(string type_name, string parent_type_name, string cfg_path = "")
	{
		if ( cfg_path == "" ) {
			cfg_path = GetItemPath( type_name );
		}

		if ( cfg_path == "" ) {
			return false;
		}

		array<string> parents = new array<string>();
		GetGame().ConfigGetFullPath( cfg_path, parents );

		string parent = "";

		parent_type_name.ToLower();

		for ( int i = 0; i < parents.Count(); i++ ) {
			parent = parents[i];
			parent.ToLower();

			if ( parent_type_name == parent ) {
				return true;
			}
		}

		return false;
	}

	// не берем в расчет патроны!!!
	static bool ItemIsMagazine(string type_name, string cfg_path = "")
	{
		if ( cfg_path == "" ) {
			cfg_path = GetItemPath( type_name );
		}

		if ( cfg_path == "" ) {
			return false;
		}

		array<string> parents = new array<string>();
		GetGame().ConfigGetFullPath( cfg_path, parents );

		string parent = "";

		for ( int i = parents.Count() - 1; i >= 0; i-- ) {
			parent = parents[i];
			parent.ToLower();

			if ( parent == "ammunition_base" ) {
				return false;
			}

			if ( parent == "magazine_base" ) {
				return true;
			}
		}

		return false;
	}

	static bool ItemHasConfig(string type_name, string cfg_path = "")
	{
		if ( cfg_path == "" ) {
			cfg_path = GetItemPath( type_name );
		}

		if ( cfg_path == "" ) {
			return false;
		}

		return GetGame().ConfigGetInt( cfg_path + " scope" ) == 2;
	}

	static int GetItemHealthMax(string type_name, string cfg_path = "")
	{
		if ( cfg_path == "" ) {
			cfg_path = GetItemPath( type_name );
		}

		if ( cfg_path == "" ) {
			return -1;
		}
		
		return GetGame().ConfigGetInt( cfg_path + " DamageSystem GlobalHealth Health hitpoints" );
	}

	static float GetItemQuantityMax(string type_name, string cfg_path = "")
	{
		if ( cfg_path == "" ) {
			cfg_path = GetItemPath( type_name );
		}

		if ( cfg_path == "" ) {
			return -1;
		}
		
		if ( ItemIsKindOf( "", "Magazine_Base", cfg_path ) ) {
			return GetGame().ConfigGetFloat( cfg_path + " count" );
		}

		if ( GetGame().ConfigIsExisting( cfg_path + " EnergyManager" ) ) {
			return GetGame().ConfigGetFloat( cfg_path + " EnergyManager energyStorageMax" );
		}
		
		float varStackMax = GetGame().ConfigGetFloat( cfg_path + " varStackMax" );

		if ( varStackMax > 0 ) {
			return varStackMax;
		}

		return GetGame().ConfigGetFloat( cfg_path + " varQuantityMax" );
	}

	static string GetItemStackedUnitType(string type_name, string cfg_path = "")
	{
		if ( cfg_path == "" ) {
			cfg_path = GetItemPath( type_name );
		}

		if ( cfg_path == "" ) {
			return "";
		}

		string stackedUnitType = "";
		GetGame().ConfigGetText( cfg_path + " stackedUnit", stackedUnitType );

		if ( stackedUnitType == "" ) {
			if ( ItemIsKindOf( "", "Magazine_Base", cfg_path ) ) {
				return "pc.";
			}

			return "percentage";
		}

		return stackedUnitType;
	}

	static bool ItemStackedUnitIsPercentage(string type_name)
	{
		string cfg_path = "CfgVehicles " + type_name;

		if ( !GetGame().ConfigIsExisting( cfg_path ) ) {
			return false;
		}

		if ( GetGame().ConfigIsExisting( cfg_path + " EnergyManager" ) ) {
			return true;
		}

		string stackedUnitType = "";
		GetGame().ConfigGetText( cfg_path + " stackedUnit", stackedUnitType );

		if ( stackedUnitType == "percentage" || stackedUnitType == "ml" || stackedUnitType == "g" || stackedUnitType == "w" ) {
			return true;
		}

		if ( stackedUnitType == "pc." || stackedUnitType == "pills" ) {
			return false;
		}

		int showQuantityBar = GetGame().ConfigGetInt( cfg_path + " quantityBar" );
		return showQuantityBar == 1;
	}

	static EntityAI GetFirstParent(EntityAI entity)
	{
		EntityAI parent = entity.GetHierarchyParent();

		if ( !parent ) {
			return null;
		}

		while ( parent.GetHierarchyParent() != null ) {
			parent = parent.GetHierarchyParent();
		}
		
		return parent;
	}

	static bool CheckDirectionLookObject(Object root, Object target)
	{
		vector rootDirection = root.GetDirection();
		vector targetDirection = target.GetDirection();

		rootDirection[1] = 0;
		targetDirection[1] = 0;

		rootDirection.Normalize();
		targetDirection.Normalize();

		float cos_fi = vector.Dot( rootDirection, targetDirection );

		if ( cos_fi < 0 ) {
			return false;
		}

		int dir = Math.Acos(cos_fi) * Math.RAD2DEG;
		return ( dir <= 60 && dir >= -60 ) || Math.AbsFloat(dir) == 90;
	}

	static void GetAverageObjectsData(notnull array<Object> objects, out vector center, out float radius)
	{
		int count = objects.Count();
		radius = 0;

		for ( int i = 0; i < count; i++ )
		{
			vector position = objects[i].GetPosition();
			
			center += position;
			radius = Math.Max( radius, vector.Distance( position, NewStoryGroupVectorFunctuon.Division( center, i + 1 ) ) );
		}

		center = NewStoryGroupVectorFunctuon.Division( center, count );
	}

	static bool IsMapping(notnull Object obj)
	{
		// тип объекта пустой если это часть ландшавта и для него (модели) не существует конфига в 'Cfg'
		if ( obj.GetType() == "" && obj.Type() == Object ) {
			return true;
		}

		if ( obj.IsTree() || obj.IsBush() ) {
			return true;
		}

		if ( obj.IsRock() ) {
			return true;
		}

		if ( obj.IsKindOf( "House" ) ) {
			return true;
		}
		
		return false;
	}
}
