class NewStoryGroupArraySerializer<Class T>
{
    static void Pack(array<T> list, array<ref Param> params)
    {
        params.Insert( new Param1<int>( list.Count() ) );

        for ( int i = 0; i < list.Count(); i++ ) {
            params.Insert( new Param1<T>( list[i] ) );
        }
    }

    static bool Unpack(array<T> list, ParamsReadContext ctx)
    {
        Param1<int> paramCount;

        if ( !ctx.Read( paramCount ) ) {
            return false;
        }

        for ( int i = 0; i < paramCount.param1; i++ ) {
            Param1<T> paramItem;

            if ( !ctx.Read( paramItem ) ) {
                return false;
            }

            list.Insert( paramItem.param1 );
        }

        return true;
    }
}

class NewStoryGroupArraySerializer_R<Class T>
{
    static void Pack(array<ref T> list, array<ref Param> params)
    {
        params.Insert( new Param1<int>( list.Count() ) );

        for ( int i = 0; i < list.Count(); i++ ) {
            params.Insert( new Param1<ref T>( list[i] ) );
        }
    }

    static bool Unpack(array<ref T> list, ParamsReadContext ctx)
    {
        Param1<int> paramCount;

        if ( !ctx.Read( paramCount ) ) {
            return false;
        }

        for ( int i = 0; i < paramCount.param1; i++ ) {
            Param1<ref T> paramItem;

            if ( !ctx.Read( paramItem ) ) {
                return false;
            }

            list.Insert( paramItem.param1 );
        }

        return true;
    }
}
