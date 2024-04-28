class NewStoryGroupWidgetFunction
{
	static void ScrollToDown(ScrollWidget w)
	{
		float width, height;
		w.GetScreenSize(width, height);

		if ( w.GetContentHeight() > height ) {
			int cycle_limit = 500;
			while ( w.GetVScrollPos01() != 1 ) {
				w.VScrollToPos01(1);
				w.Update();

				if ( cycle_limit > 0 ) {
					cycle_limit--;
				}
				else {
					break;
				}
			}
		}
	}

	static void SetTextToHTMLWidget(HtmlWidget w, string text)
	{
		int length = text.LengthUtf8();

		if ( length > 13 && text.Substring( 0, 7 ) == "file://" && text.Substring( length - 5, 5 ) == ".html" ) {
			string path = text.Substring( 7, length - 7 );
			
			if ( FileExist( path ) ) {
				w.LoadFile( path );
			}
		}
		else {
			w.SetText( Widget.TranslateString( text ) );
		}
	}

	static void ClearChildren(Widget r_w)
	{
		Widget w = r_w.GetChildren();

		while(w) {
			Widget d_w = w;
			w = w.GetSibling();
			d_w.Unlink();
		}
	}

	static bool ResizeHeightWidgetToChild( Widget parent, Widget children, float limit )
	{
		children.Update();
		parent.Update();

		int x, y;
		float p_width, p_height, c_width, c_height;

		GetScreenSize(x, y);

		parent.GetScreenSize(p_width, p_height);
		children.GetScreenSize(c_width, c_height);

		float new_p_height = y * limit;

		if ( c_height < new_p_height )
		{
			parent.SetSize( p_width, c_height );
			return true;
		}

		parent.SetSize( p_width, new_p_height );
		return false;
	}

	static void SetWidgetPositionFromMouse(Widget root, Widget main, int indent_x, int indent_y)
	{
		int x, y;
		CreateWidgetPositionFromMouse( root, main, indent_x, indent_y, x, y );
		main.SetPos( x, y );
	}

	static void CreateWidgetPositionFromMouse(Widget root, Widget main, int indent_x, int indent_y, out int x, out int y)
	{
		int screen_w, screen_h;
		float w, h;
		int m_x, m_y;

		GetMousePos( m_x, m_y );

		if ( root ) {
			float r_s_w, r_s_h;
			root.GetSize( r_s_w, r_s_h );

			screen_w = r_s_w;
			screen_h = r_s_h;
		}
		else {
			GetScreenSize( screen_w, screen_h );
		}

		main.GetSize( w, h );
		
		screen_w -= 10;
		screen_h -= 10;
		m_x += indent_x;
		m_y += indent_y;

		m_x = Math.Max( 10, m_x );
		m_y = Math.Max( 10, m_y );

		int right_edge = m_x + w;

		if ( right_edge > screen_w ) {
			m_x = screen_w - w;
		}

		int bottom_edge = m_y + h;

		if ( bottom_edge > screen_h ) {
			m_y = m_y - h - ( 2 * indent_y );
		}

		x = m_x;
		y = m_y;
	}

	static vector TransformToScreenPos(vector worldPos, Widget parent)
	{
		vector screen_pos = GetGame().GetScreenPosRelative( worldPos );

		float wdg_w, wdg_h;
		parent.GetScreenSize( wdg_w, wdg_h );

		screen_pos[0] = Math.Ceil( screen_pos[0] * wdg_w );
		screen_pos[1] = Math.Ceil( screen_pos[1] * wdg_h );
		
		return screen_pos;
	}

	static TextWidget CreateTextWidget(string text, Widget parent)
	{
		TextWidget w = TextWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "NewStoryLibrary/layouts/Helpers/TextLine.layout", parent ) );
		w.SetText(text);
		return w;
	}

	static HtmlWidget CreateHtmlWidget(Widget parent, string text)
	{
		HtmlWidget w = HtmlWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "NewStoryLibrary/layouts/Helpers/Html.layout", parent ) );

		w.SetText( text );
		w.Update();

		return w;
	}

	static Widget CreateSeparator(Widget parent)
	{
		return GetGame().GetWorkspace().CreateWidgets( "NewStoryLibrary/layouts/Helpers/separator.layout", parent );
	}

	static WrapSpacerWidget CreateWrapSpacerWidget(Widget parent)
	{
		return WrapSpacerWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "NewStoryLibrary/layouts/Helpers/Wrap.layout", parent ) );
	}
}
