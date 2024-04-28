class NewStoryGroupSetSerializer<Class T>
{
    static void Pack(set<T> list, array<ref Param> params)
    {
        params.Insert( new Param1<int>( list.Count() ) );

        for ( int i = 0; i < list.Count(); i++ ) {
            params.Insert( new Param1<T>( list[i] ) );
        }
    }

    static bool Unpack(set<T> list, ParamsReadContext ctx)
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

class NewStoryGroupSetSerializer_R<Class T>
{
    static void Pack(set<ref T> list, array<ref Param> params)
    {
        params.Insert( new Param1<int>( list.Count() ) );

        for ( int i = 0; i < list.Count(); i++ ) {
            params.Insert( new Param1<ref T>( list[i] ) );
        }
    }

    static bool Unpack(set<ref T> list, ParamsReadContext ctx)
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
