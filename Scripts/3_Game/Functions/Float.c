class NewStoryGroupFloatFunction
{
    static string ConvectToString(float value, int scaleOfFloatDouble = -1)
    {
        string text = value.ToString();

        if ( scaleOfFloatDouble < 0 || !GetGame() ) {
            return text;
        }

        int length = text.Length();
        string newText = "";
        int pointIndex = -1;

        for ( int i = 0; i < length; i++ ) {
            string symbol = text[i];
            
            if ( symbol == "." ) {
                pointIndex = i;
                continue;
            }

            if ( pointIndex == -1 ) {
                newText += symbol;
                continue;
            }

            int afterLength = i - pointIndex;

            if ( afterLength == 1 ) {
                newText += ".";
            }

            newText += symbol;

            if ( afterLength == scaleOfFloatDouble ) {
                return newText;
            }
        }

        return newText;
    }
}
