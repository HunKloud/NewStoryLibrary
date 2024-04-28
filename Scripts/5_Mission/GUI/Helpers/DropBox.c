class NewStoryGroupDropBox: ScriptedWidgetEventHandler
{
    protected Widget            _RootWidget;
    protected TextWidget        _TitleWidget;
    protected Widget            _ButtonWidget;
    protected Widget            _OnCloseImageWidget;
    protected Widget            _OnOpenImageWidget;
    protected GridSpacerWidget  _ContentWidget;

    void NewStoryGroupDropBox(Widget parent, bool opened = false)
    {
        _RootWidget         = GetGame().GetWorkspace().CreateWidgets( GetLayoutName(), parent );

        _TitleWidget        = TextWidget.Cast( _RootWidget.FindAnyWidget( "Text" ) );
        _ButtonWidget       = _RootWidget.FindAnyWidget( "Button" );
        _OnCloseImageWidget = _RootWidget.FindAnyWidget( "OnClose" );
        _OnOpenImageWidget  = _RootWidget.FindAnyWidget( "OnOpen" );
        _ContentWidget      = GridSpacerWidget.Cast( _RootWidget.FindAnyWidget( "Content" ) );
    
        _RootWidget.SetHandler( this );

        _ContentWidget.Show( opened );
        _OnOpenImageWidget.Show( opened );
        _OnCloseImageWidget.Show( !opened );
    }

    void ~NewStoryGroupDropBox()
    {
        if ( _RootWidget ) {
            _RootWidget.Unlink();
        }
    }

    protected string GetLayoutName()
    {
        return "NewStoryLibrary/layouts/Helpers/DropBox1.layout";
    }

    Widget GetRootWidget()
    {
        return _RootWidget;
    }

    GridSpacerWidget GetContentWidget()
    {
        return _ContentWidget;
    }

    void SetTitleText(string text)
    {
        _TitleWidget.SetText( text );
    }

    void Show(bool value)
    {
        _RootWidget.Show( value );
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if ( w == _ButtonWidget ) {
            Open( !_ContentWidget.IsVisible() );
            return true;
        }

        return false;
    }

    override bool OnMouseEnter(Widget w, int x, int y)
    {
        int color;

        if ( w == _ButtonWidget && GetButtonColorEnter( color ) ) {
            _OnCloseImageWidget.SetColor( color );
            _OnOpenImageWidget.SetColor( color );

            return true;
        }

        return false;
    }

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
    {
        int color;

        if ( w == _ButtonWidget && GetButtonColorLeave( color ) ) {
            _OnCloseImageWidget.SetColor( color );
            _OnOpenImageWidget.SetColor( color );

            return true;
        }
        
        return false;
    }

    protected void OnUpdateContentVisibility(bool value);

    void Open(bool value)
    {
        _ContentWidget.Show( value );
        _OnOpenImageWidget.Show( value );
        _OnCloseImageWidget.Show( !value );
        OnUpdateContentVisibility( value );
    }

    protected bool GetButtonColorEnter(out int color)
    {
        color = 0xFFFF0000;
        return true;
    }

    protected bool GetButtonColorLeave(out int color)
    {
        color = 0xFFFFFFFF;
        return true;
    }
}
