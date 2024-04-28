class NewStoryGroupRadialProgress: ScriptedWidgetEventHandler
{
	reference string RadialWidget;
	reference string ProgressInitWidget;
	reference string ProgressTextWidget;
	reference string ButtonBreakWidget;

	reference bool		ProgressTextInPercentages;
	reference float		InitRotationSpeed;
	reference float		TimeToStartAwait;
	reference string	AwaitHexColor;

	private Widget		  m_Root;

	private Widget		  m_RadialWidget;

	private Widget		  m_RadialBarWidget_1;
	private Widget		  m_RadialBarWidget_2;

	private Widget		  m_ProgressInitWidget;
	private TextWidget	  m_ProgressTextWidget;
	private Widget		  m_ButtonBreakWidget;

	private ScriptCaller	m_BreakCallback;

	private int			 m_InitColor;
	private int			 m_AwaitColor;

	private float		   m_ProgressTime;
	private float		   m_TimeBuffer;

	private int			 m_State;
	private float		   m_Rotation;

	// m_State = 0 - неактивно
	// m_State = 1 - инициализация
	// m_State = 2 - 1я стадия прогресса
	// m_State = 3 - 2я стадия прогресса
	// m_State = 4 - отсчет до начала ожидания
	// m_State = 5 - ожидание завершения прогресса

	void NewStoryGroupRadialProgress()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert( this.Update );
	}

	void ~NewStoryGroupRadialProgress()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove( this.Update );
	}

	void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;

		Class.CastTo( m_RadialWidget,	   m_Root.FindAnyWidget( RadialWidget ) );
		Class.CastTo( m_ProgressInitWidget, m_Root.FindAnyWidget( ProgressInitWidget ) );
		Class.CastTo( m_ProgressTextWidget, m_Root.FindAnyWidget( ProgressTextWidget ) );
		Class.CastTo( m_ButtonBreakWidget,  m_Root.FindAnyWidget( ButtonBreakWidget ) );

		Class.CastTo( m_RadialBarWidget_1,  m_RadialWidget.FindAnyWidget( RadialWidget + "_Bar1" ) );
		Class.CastTo( m_RadialBarWidget_2,  m_RadialWidget.FindAnyWidget( RadialWidget + "_Bar2" ) );

		m_Root.SetHandler( this );
		m_State = 0;

		m_InitColor = m_ProgressInitWidget.GetColor();

		string hexColor = AwaitHexColor;

		if ( hexColor.Length() == 7 && hexColor[0] == "#" ) {
			hexColor = "0x" + hexColor.Substring( 1, hexColor.Length() - 1 );
			m_AwaitColor = hexColor.HexToInt();

			int r = (m_AwaitColor >> 16) & 0xFF;
			int g = (m_AwaitColor >> 8) & 0xFF;
			int b = m_AwaitColor & 0xFF;

			m_AwaitColor = 0xFF << 24 | r << 16 | g << 8 | b;
		}
		else {
			m_AwaitColor = ARGB( 255, 176, 176, 176 );
		}
	}

	void SetBreakCallback(ScriptCaller callback)
	{
		m_BreakCallback = callback;
	}

	bool IsActive()
	{
		return m_State != 0;
	}

	bool IsInitialized()
	{
		return m_State == 1;
	}

	bool IsRunning()
	{
		return m_State >= 2 && m_State <= 5;
	}

	void PlayInitRotation()
	{
		m_Rotation  = 0;
		m_State	 = 1;
		m_ProgressInitWidget.Show( true );
		m_ProgressInitWidget.SetColor( m_InitColor );

		m_RadialWidget.Show( false );

		if ( m_ProgressTextWidget ) {
			m_ProgressTextWidget.Show( false );
		}
	}

	void PlayProgress(float time)
	{
		if ( m_ProgressTextWidget ) {
			m_ProgressTextWidget.Show( true );
		}

		m_ProgressInitWidget.Show( false );
		m_RadialWidget.Show( true );

		m_RadialBarWidget_1.SetRotation( 0, 0, 180 );
		m_RadialBarWidget_2.SetRotation( 0, 0, 0 );
		m_RadialBarWidget_2.Show( false );

		m_ProgressTime  = time;
		m_TimeBuffer	= 0;
		m_State		 = 2;
		m_Rotation	  = 0;
	}

	void StopProgress()
	{
		m_State = 0;
	}

	private void ShowButtonBreakImg(bool value)
	{
		Widget breakImgWidget = m_ButtonBreakWidget.GetChildren();

		if ( breakImgWidget ) {
			breakImgWidget.Show( value );
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( w == m_ButtonBreakWidget ) {
			ShowButtonBreakImg( true );
			return true;
		}
		
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( w == m_ButtonBreakWidget ) {
			ShowButtonBreakImg( false );
			return true;
		}
		
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( w == m_ButtonBreakWidget ) {
			if ( m_BreakCallback ) {
				m_BreakCallback.Invoke();
				m_State = 0;
			}
			return true;
		}

		return false;
	}

	private void Update(float dTime)
	{
		switch ( m_State )
		{
			case 1: UpdateStateInit( dTime );		   break;
			case 2: UpdateStateProgressLeft( dTime );   break;
			case 3: UpdateStateProgressRight( dTime );  break;
			case 4: UpdateStateStartAwait( dTime );	 break;
			case 5: UpdateStateAwait( dTime );		  break;
		}
	}

	private void SetProgressText()
	{
		if ( !m_ProgressTextWidget ) {
			return;
		}

		string text;

		if ( ProgressTextInPercentages ) {
			int progress = Math.Floor( ( m_TimeBuffer / m_ProgressTime ) * 100 );
			text = progress.ToString() + "%";
		}
		else {
			float progressTime = m_ProgressTime - m_TimeBuffer;

			if ( progressTime > 0 ) {
				text = NewStoryGroupFloatFunction.ConvectToString( m_ProgressTime - m_TimeBuffer, 1 );
			}
			else {
				text = "0";
			}
			
			text = text + " #STR_time_unit_abbrev_second_0";
		}

		m_ProgressTextWidget.SetText( text );
	}

	private void UpdateStateInit(float dTime)
	{
		m_Rotation += InitRotationSpeed * dTime;
		m_ProgressInitWidget.SetRotation( 0, 0, m_Rotation * Math.RAD2DEG );
	}

	private void UpdateStateProgressLeft(float dTime)
	{
		m_TimeBuffer += dTime;

		float progress = m_TimeBuffer / m_ProgressTime;

		if ( progress >= 0.5 ) {
			m_State = 3;
			m_RadialBarWidget_1.SetRotation( 0, 0, 0 );
			m_RadialBarWidget_2.Show( true );
			UpdateStateProgressRight(0);
			return;
		}

		m_RadialBarWidget_1.SetRotation( 0, 0, 180 + 360 * progress );
		SetProgressText();
	}

	private void UpdateStateProgressRight(float dTime)
	{
		m_TimeBuffer += dTime;

		float progress = m_TimeBuffer / m_ProgressTime;
		SetProgressText();

		if ( progress >= 1 ) {
			m_RadialBarWidget_2.SetRotation( 0, 0, 180 );

			m_State		 = 4 * ( TimeToStartAwait >= 0 );
			m_TimeBuffer	= 0;
			return;
		}

		m_RadialBarWidget_2.SetRotation( 0, 0, 180 + 360 * progress );
	}

	private void UpdateStateStartAwait(float dTime)
	{
		m_TimeBuffer += dTime;

		if ( m_TimeBuffer >= TimeToStartAwait ) {
			m_TimeBuffer	= 0;
			m_State		 = 5;
			m_ProgressInitWidget.Show( true );
			m_ProgressInitWidget.SetColor( m_AwaitColor );
		}
	}

	private void UpdateStateAwait(float dTime)
	{
		m_Rotation += InitRotationSpeed * dTime;
		m_ProgressInitWidget.SetRotation( 0, 0, m_Rotation * Math.RAD2DEG );
	}
}
