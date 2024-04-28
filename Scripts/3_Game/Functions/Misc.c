typedef NewStoryGroupMiscFunction NewStoryGroupMiscFunctuon;

class NewStoryGroupMiscFunction
{
	private static const string SPECIAL_CHARACTERS = "!#$%&()+,-.:;<=>?@[]^_{|}~";

	static float NormalizeRadAngle(float angle)
	{
		int c = angle/Math.PI2;
		angle = angle - Math.PI2 * c;

		if (angle < 0) {
			return Math.PI2 - angle;
		}
		
		if (angle > Math.PI2) {
			return angle - Math.PI2;
		}

		return angle;
	}

	static void CorrectionCountMinMax(int other_min, int other_max, inout int min, inout int max)
	{
		if ( other_max <= other_min ) {
			min = other_min;
			max = other_min;
		}
		else {
			max = Math.Min( max, other_max );
			min = Math.Clamp( min, other_min, max - 1 );
		}
	}

	static int GetRandomIndex(int current, int min, int max)
	{
		int indx = Math.RandomIntInclusive( min, max );

		if ( indx == current ) {
			indx += 1;
			if ( indx > max ) {
				return min;
			}
		}

		return indx;
	}

	// Предполагаем что 'current' больше или равен 'min'
	static int GetNextIndex(int current, int min, int max)
	{
		int indx = current + 1;
		
		if ( indx > max ) {
			return min;
		}

		return indx;

		// Что блять ?! Это под чем я был...
		// тут было то, что сейчас в 'GetRandomIndex'
	}

	static int GetPreviousIndex(int current, int min, int max)
	{
		int indx = current - 1;
		
		if ( indx < min ) {
			return max;
		}

		return indx;
	}

	// Преобразует входящую величину
	//  т.к. под 01 у меня может быть как прогресс, так и кол-во или пресет
	//  если входящее значение от -1 до 1, то вернет тоже значение с типом 2
	//  если значение больше 1 или меньше -1, то возвращается значение quantity01 +- 1 с типом 1
	//   т.е. в настройки мы должны вводить значение на 1 больше
	//    т.е. если нам надо что бы вывелось значение 1, то вводить должны 2
	//     да это не удобно, но таким образом не сломается то, что уже работает
	//  если входящее значение меньше 0, то возвращается тоже значение, но с типом 0
	static float ConvertValue(float value, out int type)
	{
		if ( value == 0 ) {
			type = 0;
			return 0;
		}

		if ( value >= -1 && value <= 1 ) {
			type = 2;
			return value;
		}

		type = 1;
		
		if ( value > 1 ) {
			return value - 1;
		}
		
		return value + 1;
	}

	static float ConvertValueEx(float value, float valueMax)
	{
		int valueType;
		float newValue = ConvertValue( value, valueType );

		if ( valueType == 2 ) {
			newValue *= valueMax;
		}
		
		return newValue;
	}

	static Man GetPlayerByIdentity(notnull PlayerIdentity playerIdentity)
	{
		if ( GetGame() ) {
			int	networkIdLowBits    = 0;
			int networkIdHightBits  = 0;
			GetGame().GetPlayerNetworkIDByIdentityID( playerIdentity.GetPlayerId(), networkIdLowBits, networkIdHightBits );
			return Man.Cast( GetGame().GetObjectByNetworkId( networkIdLowBits, networkIdHightBits ) );
		}

		return null;
	}

	// рассчет 2х значение по входящему оператору
	// v1 - текущее значение
	// v2 - текущее значение
	static float CalculationOperator(float v1, float v2, ENewStoryGroupOperators operator)
	{
		switch ( operator )
		{
			case ENewStoryGroupOperators.LOWEST:           return Math.Min( v1, v2 );
			case ENewStoryGroupOperators.HIGHEST:          return Math.Max( v1, v2 );
			case ENewStoryGroupOperators.ADD:              return v1 + v2;
			case ENewStoryGroupOperators.SUBSTRACT:        return v1 - v2;
			case ENewStoryGroupOperators.MULTIPLICATIVE:   return v1 * v2;

			case ENewStoryGroupOperators.DIVISION:
			{
				if ( v2 == 0 )
				{
					ErrorEx( "Деление на 0 запрещено" );
					return 0;
				}

				return v1 / v2;
			}

			case ENewStoryGroupOperators.SET: return v2;
		}

		ErrorEx( "Попытка рассчета с неизвестным оператором" );
		return 0;
	}

	static string GetSteamProfileURL(string steamID)
	{
		return "https://steamcommunity.com/profiles/" + steamID;
	}

	static array<ref array<string>> ParsedConditions(string conditions)
	{
		array<ref array<string>> conditionsList;

		autoptr array<string> andList = new array<string>();

		conditions.Replace( "'And'", "&" );
		conditions.Replace( "'Or'", "|" );
		conditions.Replace( " ", "" );
		conditions.Replace( "{", "" );
		conditions.Replace( "}", "" );
		
		conditions.Split( "&", andList );
		
		foreach ( string and: andList )
		{
			array<string> orList = new array<string>();

			and.Split( "|", orList );

			if ( !conditionsList )
				conditionsList = new array<ref array<string>>();
			
			conditionsList.Insert( orList );
		}

		return conditionsList;
	}

	static bool CheckConditions(Class instance, string function, Man player, string conditions)
	{
		if ( conditions == "" ) return true;
		return CheckConditions( instance, function, player, ParsedConditions(conditions) );
	}

	// (Man man, string conditionName, array<string> values)
	static bool CheckConditions(Class instance, string function, Man player, array<ref array<string>> conditionsList)
	{
		if ( !conditionsList ) return true;

		autoptr array<string> buffer = new array<string>();

		string conditionName;
		string valuesString;
		
		foreach ( array<string> list: conditionsList )
		{
			bool result = false;

			foreach ( string condition: list )
			{
				buffer.Clear();
				condition.Split( ":", buffer );

				conditionName = buffer[0];

				if ( buffer.Count() > 1 )
				{
					valuesString = buffer[1];
					buffer.Clear();
					valuesString.Split( ",", buffer );
				}
				else
				{
					buffer.Clear();
				}

				bool check;

				Param params = new Param3<Man, string, array<string>>( player, conditionName, buffer );
				bool callResult = GetGame().GameScript.CallFunctionParams( instance, function, check, params );

				if ( !callResult )
				{
					if ( instance ) {
						ErrorEx( string.Format( "Failed to call function \'%1\' on \'%2\'", function, instance.GetDebugName() ) );
					} else {
						ErrorEx( string.Format( "Failed to call global function \'%1\'", function ) );
					}
					
					return false;
				}

				if ( check )
				{
					result = true;
					break;
				}
			}

			if ( !result )
				return false;
		}

		return true;
	}

	static string GetVaribleType(ENewStoryGroupVariblesType variable)
	{
		switch ( variable )
		{
			case ENewStoryGroupVariblesType.BOOL:   return "bool";
			case ENewStoryGroupVariblesType.INT:    return "int";
			case ENewStoryGroupVariblesType.FLOAT:  return "float";
			case ENewStoryGroupVariblesType.STRING: return "string";
			case ENewStoryGroupVariblesType.VECTOR: return "vector";
		}

		return "";
	}

	static bool IsSpecialCharacter(string character)
	{
		return SPECIAL_CHARACTERS.Contains( character );
	}

	static string GetRandomSpecialCharacter()
	{
		return SPECIAL_CHARACTERS.Get( Math.RandomInt( 0, SPECIAL_CHARACTERS.Length() ) );
	}
}
