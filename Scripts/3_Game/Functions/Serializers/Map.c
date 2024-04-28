class NewStoryGroupMapSerializer<Class TKey,Class TValue>
{
    static void Pack(map<TKey, TValue> list, array<ref Param> params)
    {
        params.Insert( new Param1<int>( list.Count() ) );

        for ( int i = 0; i < list.Count(); i++ ) {
            params.Insert( new Param1<TKey>( list.GetKey(i) ) );
            params.Insert( new Param1<TValue>( list.GetElement(i) ) );
        }
    }

    static bool Unpack(map<TKey, TValue> list, ParamsReadContext ctx)
    {
        Param1<int> paramCount;

        if ( !ctx.Read( paramCount ) ) {
            return false;
        }

        for ( int i = 0; i < paramCount.param1; i++ ) {
            Param1<TKey> paramKey;
            Param1<TValue> paramValue;

            if ( !ctx.Read( paramKey ) ) {
                return false;
            }

            if ( !ctx.Read( paramValue ) ) {
                return false;
            }

            list.Insert( paramKey.param1, paramValue.param1 );
        }

        return true;
    }
}

class NewStoryGroupMapSerializer_RK<Class TKey,Class TValue>
{
    static void Pack(map<ref TKey, TValue> list, array<ref Param> params)
    {
        params.Insert( new Param1<int>( list.Count() ) );

        for ( int i = 0; i < list.Count(); i++ ) {
            params.Insert( new Param1<ref TKey>( list.GetKey(i) ) );
            params.Insert( new Param1<TValue>( list.GetElement(i) ) );
        }
    }

    static bool Unpack(map<ref TKey, TValue> list, ParamsReadContext ctx)
    {
        Param1<int> paramCount;

        if ( !ctx.Read( paramCount ) ) {
            return false;
        }

        for ( int i = 0; i < paramCount.param1; i++ ) {
            Param1<ref TKey> paramKey;
            Param1<TValue> paramValue;

            if ( !ctx.Read( paramKey ) ) {
                return false;
            }

            if ( !ctx.Read( paramValue ) ) {
                return false;
            }

            list.Insert( paramKey.param1, paramValue.param1 );
        }

        return true;
    }
}

class NewStoryGroupMapSerializer_RV<Class TKey,Class TValue>
{
    static void Pack(map<TKey, ref TValue> list, array<ref Param> params)
    {
        params.Insert( new Param1<int>( list.Count() ) );

        for ( int i = 0; i < list.Count(); i++ ) {
            params.Insert( new Param1<TKey>( list.GetKey(i) ) );
            params.Insert( new Param1<ref TValue>( list.GetElement(i) ) );
        }
    }

    static bool Unpack(map<TKey, ref TValue> list, ParamsReadContext ctx)
    {
        Param1<int> paramCount;

        if ( !ctx.Read( paramCount ) ) {
            return false;
        }

        for ( int i = 0; i < paramCount.param1; i++ ) {
            Param1<TKey> paramKey;
            Param1<ref TValue> paramValue;

            if ( !ctx.Read( paramKey ) ) {
                return false;
            }

            if ( !ctx.Read( paramValue ) ) {
                return false;
            }

            list.Insert( paramKey.param1, paramValue.param1 );
        }

        return true;
    }
}

class NewStoryGroupMapSerializer_RK_RV<Class TKey,Class TValue>
{
    static void Pack(map<ref TKey, ref TValue> list, array<ref Param> params)
    {
        params.Insert( new Param1<int>( list.Count() ) );

        for ( int i = 0; i < list.Count(); i++ ) {
            params.Insert( new Param1<ref TKey>( list.GetKey(i) ) );
            params.Insert( new Param1<ref TValue>( list.GetElement(i) ) );
        }
    }

    static bool Unpack(map<ref TKey, ref TValue> list, ParamsReadContext ctx)
    {
        Param1<int> paramCount;

        if ( !ctx.Read( paramCount ) ) {
            return false;
        }

        for ( int i = 0; i < paramCount.param1; i++ ) {
            Param1<ref TKey> paramKey;
            Param1<ref TValue> paramValue;

            if ( !ctx.Read( paramKey ) ) {
                return false;
            }

            if ( !ctx.Read( paramValue ) ) {
                return false;
            }

            list.Insert( paramKey.param1, paramValue.param1 );
        }

        return true;
    }
}
