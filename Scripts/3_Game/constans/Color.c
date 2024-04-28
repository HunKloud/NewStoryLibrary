class NewStoryGroupColor
{
    static int VectorARGBToHex(vector rgb)
    {
        float r = rgb[0];
        float g = rgb[1];
        float b = rgb[2];

        return ARGBToHex( 255, (int)r, (int)g, (int)b );
    }

    static int VectorRGBToHex(vector rgb)
    {
        float r = rgb[0];
        float g = rgb[1];
        float b = rgb[2];

        return RGBToHex( (int)r, (int)g, (int)b );
    }

    static int RGBToHex(int r, int g, int b)
    {
        r = (r & 0xFF) << 16;
        g = (g & 0xFF) << 8;
        b = b & 0xFF;

        return r | g | b;
    }

    static int ARGBToHex(int a, int r, int g, int b)
    {
        a = (a & 0xFF) << 24;
        r = (r & 0xFF) << 16;
        g = (g & 0xFF) << 8;
        b = b & 0xFF;

        return a | r | g | b;
    }

    static void HexToRGB(int rgb, out int r, out int g, out int b)
    {
        r = ( rgb >> 16 ) & 0xFF;
        g = ( rgb >> 8 ) & 0xFF;
        b = rgb & 0xFF;
    }

    static void HexToARGB(int argb, out int a, out int r, out int g, out int b)
    {
        a = ( argb >> 24 ) & 0xFF;
        r = ( argb >> 16 ) & 0xFF;
        g = ( argb >> 8 ) & 0xFF;
        b = argb & 0xFF;
    }

    static vector HexToVectorRGB(int rgb)
    {
        int r = ( rgb >> 16 ) & 0xFF;
        int g = ( rgb >> 8 ) & 0xFF;
        int b = rgb & 0xFF;

        return Vector( r, g, b );
    }
}
