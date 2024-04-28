typedef NewStoryGroupIntFunction NewStoryGroupIntFunctuon;

class NewStoryGroupIntFunction
{
	static bool CheckMaskBit(int mask, int bit)
	{
		return ( mask & bit ) == bit;
	}
}
