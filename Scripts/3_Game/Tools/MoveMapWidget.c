class NewStoryGroupMoveMapWidget
{
    private MapWidget   _Map;

    private vector      _StartPosition;
    private vector      _EndPosition;
    private vector      _Direction;
    private float       _Distance;

    private float       _StartScale;
    private float       _EndScale;

    private float       _Time;

    private float   _TimeBuffer;

    void SetData(MapWidget w, vector pos, float scale = 0.33, float time = 1)
    {
        _Map = w;

        _StartPosition  = _Map.GetMapPos();
        _EndPosition    = pos;

        _Direction = vector.Direction( _StartPosition, _EndPosition );
        _Direction.Normalize();

        _Distance = vector.Distance( _StartPosition, _EndPosition );

        _StartScale = _Map.GetScale();
        _EndScale   = scale;
        
        _Time       = Math.Max( time, 0.001 );

        _TimeBuffer = 0;

        _Map.SetFlags( WidgetFlags.IGNOREPOINTER );
    }

    void Update(float dTime)
    {
        if ( !_Map ) {
            delete this;
            return;
        }

        _TimeBuffer += dTime;

        if ( _TimeBuffer > _Time ) {
            _Map.SetMapPos( _EndPosition );
            _Map.SetScale( _EndScale );

            _Map.ClearFlags( WidgetFlags.IGNOREPOINTER );

            delete this;
            return;
        }

        float time01 = Math.InverseLerp( 0, _Time, _TimeBuffer );

        vector position = _StartPosition + ( _Direction * _Distance * time01 );
        _Map.SetMapPos( position );

        float scale = _StartScale + ( _EndScale - _StartScale ) * time01;
        _Map.SetScale( scale );
    }
}
