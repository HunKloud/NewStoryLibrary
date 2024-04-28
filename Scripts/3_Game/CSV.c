class NewStoryGroupCSV
{
	// callback (int lineNumber, array<string> parameters)
	static void Parse(string filename, string delimiter, ScriptCaller callback)
	{
		FileHandle file = OpenFile( filename, FileMode.READ );

		if ( file == 0 ) {
			ErrorEx( string.Format( "Can't open file: %1", filename ) );
			return;
		}

		int lineNumber = 1;
		string lineContent;
		array<string> parameters = new array<string>();

		while ( FGets( file,  lineContent ) > 0 ) {
			lineContent = NewStoryGroupStringFunction.SubstringComment( lineContent );
			parameters.Clear();

			lineContent.Split( delimiter, parameters );
			callback.Invoke( lineNumber, parameters );
			
			lineNumber++;
		}

		CloseFile( file );
	}
}
