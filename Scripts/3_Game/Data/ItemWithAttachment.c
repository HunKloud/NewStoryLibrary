class NewStoryGroupItemWithAttachment
{
	string Name;
	ref array<float> Health01 = new array<float>();
	ref array<float> Quantity01 = new array<float>();

	ref array<ref NewStoryGroupItemWithAttachment> Inventory = new array<ref NewStoryGroupItemWithAttachment>();
	ref array<ref array<string>> Args;

	void Init()
	{
		array<string> customSettings = new array<string>();
		Name.Split( "|", customSettings );

		int customSettingsArgsCount = customSettings.Count();

		if ( customSettingsArgsCount == 0 ) {
			return;
		}

		Name = customSettings[0];

		string cfg_path = NewStoryGroupObjectFunctuon.GetItemPath( Name );

		if ( cfg_path == "" || GetGame().ConfigGetInt( cfg_path + " scope" ) != 2 ) {
			Name = "";
			return;
		}

		Name.ToLower();

		SetMinMax( Health01 );
		SetMinMax( Quantity01 );

		if ( customSettingsArgsCount > 1 ) {
			LoadArgs( customSettings );
		}

		NewStoryGroupItemWithAttachment att;

		for ( int i = Inventory.Count() - 1; i >= 0; i-- )
		{
			att = Inventory[i];
			att.Init();

			if ( !att.IsCorrect() ) {
				Inventory.Remove(i);
			}
		}
	}

	private void SetMinMax(array<float> values)
	{
		if ( !values || values.Count() == 0 ) {
			values = null;
			return;
		}

		array<float> setVarible = new array<float>();

		if ( values.Count() == 1 ) {
			setVarible.Insert( Math.Clamp( values[0], 0, 1 ) );
		}
		else {
			float min, max;

			max = Math.Clamp( values[1], 0, 1 );
			min = Math.Clamp( values[0], 0, values[1] );

			setVarible.Insert( min );

			if ( min != max ) {
				setVarible.Insert( max );
			}
		}

		values = setVarible;
	}

	private void LoadArgs(array<string> list)
	{
		Args = new array<ref array<string>>();

		string buffer;

		for ( int i = 1; i < list.Count(); i++ ) {
			buffer = list[i];
			buffer.ToLower();

			array<string> arg = new array<string>();
			buffer.Split( " ", arg );
			Args.Insert( arg );
		}
	}

	bool IsCorrect()
	{
		return Name != "";
	}

	float GetRandomHealth01()
	{
		if ( !Health01 || Health01.Count() == 0 ) {
			return 1;
		}

		if ( Health01.Count() == 1 ) {
			return Health01[0];
		}

		float min = Health01[0];
		float max = Health01[1];

		if ( min == max ) {
			return max;
		}

		return Math.RandomFloatInclusive( min, max );
	}

	float GetRandomQuantity()
	{
		if ( !Quantity01 || Quantity01.Count() == 0 ) {
			return -1;
		}

		if ( Quantity01.Count() == 1 ) {
			return Quantity01[0];
		}

		return Math.RandomFloatInclusive( Quantity01[0], Quantity01[1] );
	}

	float GetQuantityMin()
	{
		if ( !Quantity01 || Quantity01.Count() == 0 ) {
			return -1;
		}

		return Quantity01[0];
	}

	float GetQuantityMax()
	{
		if ( !Quantity01 || Quantity01.Count() == 0 ) {
			return -1;
		}

		if ( Quantity01.Count() == 1 ) {
			return Quantity01[0];
		}

		return Quantity01[1];
	}

	bool HasArgument(string name)
	{
		if ( !Args ) {
			return false;
		}

		for ( int i = 0; i < Args.Count(); i++ ) {
			if ( Args[i][0] == name ) {
				return true;
			}
		}

		return false;
	}

	bool GetArgumentValue(string name, out string value)
	{
		if ( !Args ) {
			return false;
		}

		array<string> arg;

		for ( int i = 0; i < Args.Count(); i++ ) {
			arg = Args[i];

			if ( arg[0] == name ) {
				value = "";

				if ( arg.Count() > 1 ) {
					value = arg[1];
				}
				
				return true;
			}
		}

		return false;
	}
}
