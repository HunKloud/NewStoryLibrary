typedef NewStoryGroupTypenameFunction NewStoryGroupTypenameFunctuon;

class NewStoryGroupTypenameFunction
{
	static bool HasVariable(typename t, string name)
	{
		int count = t.GetVariableCount();

		for ( int i = 0; i < count; i++ ) {
			if ( t.GetVariableName(i) == name ) {
				return true;
			}
		}
		
		return false;
	}
}
