typedef NewStoryGroupStringFunction NewStoryGroupStringFunctuon;

class NewStoryGroupStringFunction
{
	static int IndexOf(string text, string sample, int start = 0)
	{
		int textLength		= text.LengthUtf8();
		int sampleLength	= sample.LengthUtf8();

		if ( textLength == 0 || sampleLength == 0 ) {
			return -1;
		}

		int index = Math.Max( start, 0 );
		string check = "";

		while ( index < textLength ) {
			check += text.SubstringUtf8( index++, 1 );

			if ( check.LengthUtf8() == sampleLength ) {
				if ( check == sample ) {
					return index - sampleLength;
				}

				check = "";
			}
		}

		return -1;
	}

	static string CreateSymbolLength( string symbol, int length)
	{
		string text = "";
		for ( int i = 0; i < length; i++ ) {
			text += symbol;
		}
		return text;
	}

	static string SubstringInvertedUtf8( string string_to_split, int position_start, int position_end )
	{
		string first_half = string_to_split.SubstringUtf8(0, position_start);
		string second_half = string_to_split.SubstringUtf8( position_end, string_to_split.LengthUtf8() - position_end );
		string result = first_half + second_half;
		return result;
	}
	
	// Проверка текста на допустимые численные значения
	static bool ValidValueCheck(inout string potential_value, bool ignore_sign = false)
	{
		bool was_text_changed = false;
		
		for ( int i = potential_value.LengthUtf8() - 1 ; i >= 0; --i ) {
			int asciiCode = potential_value.Get(i).ToAscii();
			if ( ignore_sign && i == 0 && asciiCode == 45 ) {
				continue;
			}

			if ( asciiCode < 48 || asciiCode > 57 ) {
				potential_value = potential_value.SubstringInverted( potential_value, i, i + 1 );
				was_text_changed = true;
			}
		}
		
		return was_text_changed;
	}
	
	// Ограничение на длину текста
	static bool LimitTextBoxCharacterCount(inout string text, int max_length)
	{
		if ( max_length > 0 && text.LengthUtf8() > max_length ) {
			text = text.SubstringUtf8(0, max_length);
			return true;
		}
		
		return false;
	}
	
	static bool LimitTextBoxValue(inout string text, int min_value, int max_value)
	{
		bool changed	= ValidValueCheck( text, true );
		int value		= text.ToInt();

		if ( value < min_value ) {
			text = min_value.ToString();
			return true;
		}

		if ( value > max_value ) {
			text = max_value.ToString();
			return true;
		}
		
		return changed;
	}
	
	static bool CheckEmptyText(string text)
	{
		text.Replace(" ", "");
		return text == "";
	}

	static void ReplaceKeyWords(inout string text)
	{
		if ( GetGame() ) {
			int start_index = text.IndexOf( "%" );
			int stop_index;

			while ( start_index != -1 ) {
				text.Set( start_index, "" );
				stop_index = text.IndexOf( "%" );

				if ( stop_index == -1 ) {
					break;
				}

				text.Set( stop_index, "" );

				string item_type = text.SubstringUtf8( start_index, stop_index - start_index );
				string cfg_path = NewStoryGroupObjectFunctuon.GetItemPath( item_type );
				string replace_text = "";

				if ( cfg_path != "" ) {
					replace_text = GetGame().ConfigGetTextOut( cfg_path + " displayName" );
				}

				text.Replace( String( "%" + item_type + "%" ), replace_text );
				text.Replace( item_type, replace_text );

				start_index = text.IndexOf( "%" );
			}
		}
	}

	static void ReplaceKeyWords(EntityAI source, inout string text)
	{
		ReplaceKeyWords( source.GetType(), text );
	}

	static void ReplaceKeyWords(string item_type, inout string text)
	{
		if ( !GetGame() ) {
			return;
		}

		string cfg_path = NewStoryGroupObjectFunctuon.GetItemPath( item_type );

		string new_text = "";
		string check_text = "";
		int length = text.LengthUtf8();

		bool replace = false;

		for ( int i = 0; i < length; i++ ) {
			string symbol = text.SubstringUtf8( i, 1 );

			if ( symbol == "%" ) {
				if ( replace ) {
					new_text += HelpReplaceKeyWord( cfg_path, check_text );
					check_text = "";
				}

				replace = !replace;
				continue;
			}

			if ( !replace ) {
				new_text += symbol;
			}
			else {
				check_text += symbol;
			}
		}

		text = new_text;
	}

	static void ReplaceKeyItems(inout string text)
	{
		if ( !GetGame() ) {
			return;
		}

		string cfg_path = "";
		string new_text = "";
		string check_text = "";
		int length = text.LengthUtf8();
		array<string> buffer = new array<string>();

		bool replace = false;

		for ( int i = 0; i < length; i++ ) {
			string symbol = text.SubstringUtf8( i, 1 );

			if ( symbol == "%" ) {
				if ( replace ) {
					buffer.Clear();
					check_text.Split( ".", buffer );

					if ( buffer.Count() == 2 ) {
						cfg_path = NewStoryGroupObjectFunctuon.GetItemPath( buffer[0] );

						if ( cfg_path != "" ) {
							new_text += HelpReplaceKeyWord( cfg_path, buffer[1] );
						}
					}

					check_text = "";
				}

				replace = !replace;
				continue;
			}

			if ( !replace ) {
				new_text += symbol;
			}
			else {
				check_text += symbol;
			}
		}

		text = new_text;
	}

	static string HelpReplaceKeyWord(string cfg_path, string key)
	{
		if ( cfg_path == "" ) {
			return ""; 
		}

		switch ( key ) {
			case "ItemDisplayName":			return GetGame().ConfigGetTextOut( cfg_path + " displayName" );
			case "ItemDescriptionShort":	return GetGame().ConfigGetTextOut( cfg_path + " descriptionShort" );
			case "ItemHealth":				return GetGame().ConfigGetInt( cfg_path + " DamageSystem GlobalHealth Health" ).ToString();
			case "ItemSize": {
				array<int> item_size_config = new array<int>();
				GetGame().ConfigGetIntArray( cfg_path + " itemSize", item_size_config);

				if ( item_size_config.Count() >= 2 ) {
					return string.Format( "%1x%2", item_size_config[0], item_size_config[1] );
				}

				return "UNKNOW";
			}
		}

		return "UNKNOW";
	}

	static void ReplaceTextWithKey(inout string text, notnull map<string, string> keys)
	{
		if ( !GetGame() ) {
			return;
		}

		string new_text = "";
		string check_text = "";
		int length = text.LengthUtf8();

		bool replace = false;

		for ( int i = 0; i < length; i++ ) {
			string symbol = text.SubstringUtf8( i, 1 );

			if ( symbol == "%" ) {
				if ( replace ) {
					new_text += keys[check_text];
					check_text = "";
				}

				replace = !replace;
				continue;
			}

			if ( !replace ) {
				new_text += symbol;
			}
			else {
				check_text += symbol;
			}
		}

		text = new_text;
	}

	static string SubstringComment(string text)
	{
		int indx = text.IndexOf("//");

		if ( indx != -1 ) {
			return text.SubstringUtf8( 0, indx );
		}
		
		return text;
	}

	// Return corrected corrected bool value
	// "true" or "false"	-> "1" or "0"	true
	// " true " or " false"	-> "1" or "0"	true
	// "1" or "0"			-> "1" or "0"	true
	// "3"					-> "3"			false
	static bool IsBool(inout string value)
	{
		string copy = value.Trim();

		if ( copy == "true" || copy == "1") {
			value = "1";
			return true;
		}

		if ( copy == "false" || copy == "0") {
			value = "0";
			return true;
		}

		return false;
	}

	// Not supported HEX
	// Return corrected corrected int value
	//  "0"		-> "0"		true
	//  " 0"	-> "0"		true
	//  " 0 "	-> "0"		true
	//  " 0 0 " -> " 0 0 " 	false
	static bool IsInt(inout string value)
	{
		string copy = value.Trim();

		int length = copy.LengthUtf8();

		if ( length == 0 ) {
			return false;
		}

		string outValue = "";
		string symbol = "";
		int asciiCode = 0;

		for ( int i = 0; i < length; i++ ) {
			symbol		= copy[i];
			asciiCode	= symbol.ToAscii();

			if ( asciiCode < 48 || asciiCode > 57 ) {
				if ( i != 0 || asciiCode != 45 ) {
					return false;
				}
			}

			outValue += symbol;
		}

		value = outValue;
		return true;
	}
	
	// Return corrected corrected float value
	//  "0.01"  -> "0.01"  true
	//  "-0.01" -> "-0.01" true
	//  ".01"   -> "0.01"  true
	//  " .01"  -> "0.01"  true
	//  " .01 " -> "0.01"  true
	//  "0,01"  -> "0.01"  true
	//  " ,01"  -> "0.01"  true
	//	"0.0.1" -> "0.0.1" false
	static bool IsFloat(inout string value)
	{
		string copy = value.Trim();
		int length = copy.LengthUtf8();

		if ( length == 0 ) {
			return false;
		}

		string outValue = "";
		bool hasDelimiter	= false;
		bool startZero		= false;
		string symbol = "";
		int asciiCode = 0;

		for ( int i = 0; i < length; i++ ) {
			symbol		= copy[i];
			asciiCode	= symbol.ToAscii();

			// if ( !hasDelimiter && !startZero && asciiCode == 48 ) {
				
			// }

			// Check " "
			if ( asciiCode == 32 ) {
				return false;
			}

			// Check "," or "."
			if ( asciiCode == 44 || asciiCode == 46 ) {
				if ( !hasDelimiter ) {
					if ( i == 0 ) {
						outValue += "0";
					}
					
					if ( i != length - 1 ) {
						outValue += ".";
					}

					hasDelimiter = true;
				}
				else {
					return false;
				}
			}
			// is not number
			else if ( asciiCode < 48 || asciiCode > 57 ) {
				if ( i != 0 || asciiCode != 45 ) {
					return false;
				}

				outValue += symbol;
			}
			else {
				outValue += symbol;
			}
		}

		value = outValue;
		return true;
	}

	// Возвращает в 'value' первое вхождение (до пробела) значения заданного в 'type'
	//  не поддерживает ENewStoryGroupVariblesType.VECTOR
	//   для этого используйте 'ParserVector'
	//  'text' = 'остаток от текста'
	static bool ParserVarible(inout string text, out string value, int type = ENewStoryGroupVariblesType.STRING)
	{
		string outValue = "";
		string copy 	= text.Trim();
		string symbol	= "";

		int index		= 0;
		int length		= copy.LengthUtf8();

		while ( index < length ) {
			symbol = copy[index];
			index++;

			if ( symbol == " " ) {
				break;
			}

			outValue += symbol;
		}

		bool corrected = false;

		switch ( type )
		{
			case ENewStoryGroupVariblesType.BOOL:
			{
				if ( IsBool( outValue ) ) {
					corrected = true;
				}
				break;
			}
			case ENewStoryGroupVariblesType.INT:
			{
				if ( IsInt( outValue ) ) {
					corrected = true;
				}
				break;
			}
			case ENewStoryGroupVariblesType.FLOAT:
			{
				if ( IsFloat( outValue ) ) {
					corrected = true;
				}
				break;
			}
			case ENewStoryGroupVariblesType.STRING:
			{
				if ( outValue.LengthUtf8() == 0 ) {
					return false;
				}
				corrected = true;
				break;
			}
		}

		if ( corrected ) {
			text	= copy.SubstringUtf8( index, length - index );
			value	= outValue;
			return true;
		}

		return false;
	}
	
	// Return corrected corrected vector
	//  "0 0 0"		-> "0 0 0"		true
	//  "-0 -0 0"   -> "-0 -0 0"	true
	//  " 0 0 0 "	-> "0 0 0"		true
	//  "<0 0 0>"	-> "0 0 0"		true
	//  "<0,0,0>"	-> "0 0 0"		true
	//  "<0 0 0>"	-> "0 0 0"		true
	//  "<0, 0, 0>"	-> "0 0 0"		true
	//  "[0,0,0]"	-> "0 0 0"		true
	//  "[0 0 0]"	-> "0 0 0"		true
	//  "[0, 0, 0]"	-> "0 0 0"		true
	//  "[0 0,0>"	-> "0 0 0"		true =D
	//  "[0 0,0,0>"	-> "0 0 0"		true Okey ...
	//  "0 0"		-> "0 0"		false
	//  "0 0 ad"	-> "0 0 ad"		false
	static bool IsVector(inout string value)
	{
		string copy = value.Trim();
		int length	= copy.LengthUtf8();

		if ( length < 5 ) {
			return false;
		}

		int valueCount		= 0;
		string outValue 	= "";
		string checkFloat 	= "";
		string symbol		= "";
		int asciiCode		= 0;

		bool endLine		= false;
		bool delimiter		= false;

		for ( int i = 0; i < length; i++ ) {
			symbol		= copy[i];
			asciiCode	= symbol.ToAscii();

			delimiter	= false;
			endLine		= i == length - 1;

			// "<" or ">" or "[" or "]"
			if ( asciiCode == 60 || asciiCode == 62 || asciiCode == 91 || asciiCode == 93 ) {
				symbol = "";
			}
			// "," or " "
			else if ( asciiCode == 44 || asciiCode == 32 ) {
				delimiter = true;
			}

			if ( delimiter || endLine ) {
				if ( endLine && !delimiter ) {
					checkFloat += symbol;
				}

				if ( checkFloat == "" ) {
					continue;
				}
				
				if ( !IsFloat( checkFloat ) ) {
					return false;
				}

				outValue += checkFloat;

				checkFloat = "";
				valueCount += 1;

				if ( valueCount == 3 ) {
					break;
				}

				outValue += " ";
			}
			else {
				checkFloat += symbol;
			}
		}

		if ( valueCount == 3 ) {
			value = outValue;
			return true;
		}

		return false;
	}

	static bool ParserVector(inout string text, out string value)
	{
		string copy 	= text.Trim();
		int length		= copy.LengthUtf8();

		if ( length < 5 ) {
			return false;
		}

		string outValue 	= "";
		
		string symbol		= "";
		int asciiCode		= 0;

		int index			= 0;
		int lastIndex		= 0;

		string checkFloat 	= "";
		int valueCount		= 0;
		bool endLine		= false;
		bool delimiter		= false;

		bool requestFindEnd	= false;

		while ( index < length ) {
			symbol		= copy[index];
			asciiCode	= symbol.ToAscii();

			index++;

			if ( requestFindEnd ) {
				// ">" or "]"
				if ( asciiCode == 62 || asciiCode == 93 ) {
					lastIndex = index;
					break;
				}
				
				continue;
			}

			delimiter	= false;
			endLine		= index == length - 1;


			// "<" or "["
			if ( asciiCode == 60 || asciiCode == 91 ) {
				symbol = "";
			}
			// "," or " "
			else if ( asciiCode == 44 || asciiCode == 32 ) {
				delimiter = true;
			}

			if ( delimiter || endLine ) {
				if ( endLine && !delimiter ) {
					checkFloat += symbol;
				}

				if ( checkFloat == "" ) {
					continue;
				}
				
				if ( !IsFloat( checkFloat ) ) {
					return false;
				}

				outValue += checkFloat;

				checkFloat = "";
				valueCount += 1;

				if ( valueCount == 3 ) {
					requestFindEnd	= true;
					lastIndex		= index;
				}
				else {
					outValue += " ";
				}
			}
		}

		if ( requestFindEnd ) {
			text	= copy.SubstringUtf8( lastIndex, length - lastIndex );
			value	= outValue;
			return true;
		}

		return false;
	}
	
	static bool IsArray(inout string value, int type = ENewStoryGroupVariblesType.STRING)
	{
		string copy = value.Trim();

		int length = copy.LengthUtf8();

		if ( length < 3 || copy[0] != "{" || copy[length - 1] != "}" ) {
			return false;
		}

		string checkValue	= "";
		string outValue		= "";
		copy = copy.SubstringUtf8( 1, length - 2 );

		array<string> values = new array<string>();
		copy.Split( ",", values );

		int count = values.Count();

		for ( int i = 0; i < count; i++ ) {
			checkValue = values[i];
			
			switch( type )
			{
				case ENewStoryGroupVariblesType.INT:
				{
					if ( !IsInt( checkValue ) ) {
						return false;
					}
					break;
				}

				case ENewStoryGroupVariblesType.FLOAT:
				{
					if ( !IsFloat( checkValue ) ) {
						return false;
					}
					break;
				}
				
				case ENewStoryGroupVariblesType.BOOL:
				{
					if ( !IsBool( checkValue ) ) {
						return false;
					}
					break;
				}
				
				case ENewStoryGroupVariblesType.VECTOR:
				{
					if ( !IsVector( checkValue ) ) {
						return false;
					}
					break;
				}
			}

			outValue += checkValue;

			if ( i != count - 1 ) {
				outValue += ",";
			}
		}

		value = "{" + outValue + "}";
		return true;
	}

	static bool ParserArray(inout string text, out string value, int type = ENewStoryGroupVariblesType.STRING)
	{
		string outValue = "";
		string copy 	= value.Trim();
		string symbol	= "";

		int index		= 0;
		int length		= copy.LengthUtf8();

		int startIndex	= -1;
		int endIndex	= -1;

		for ( ; index < length; index++ ) {
			symbol = copy[index];

			if ( startIndex == -1 ) {
				if ( symbol == "{" ) {
					startIndex = index;
				}

				continue;
			}

			if ( symbol == "}" ) {
				endIndex = index;
				break;
			}
		}

		if ( startIndex != -1 && endIndex != -1 ) {
			outValue = copy.SubstringUtf8( startIndex, endIndex - startIndex + 1 );

			if ( IsArray( outValue, type ) ) {
				text = copy.SubstringUtf8( endIndex + 1, length - endIndex - 1 );
				value = outValue;
				return true;
			}
		}

		return false;
	}
}
