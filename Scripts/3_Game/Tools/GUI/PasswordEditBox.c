class NewStoryGroupPasswordWidget: ScriptedWidgetEventHandler
{
	reference string PasswordEditBox;
	reference string HideButton;

	reference bool ShowPassword;

	reference int PasswordLength;

	reference bool UseNumber;
	reference bool UseEngSymbol;
	reference bool UseSpecialCharacter;

	private Widget m_Root;
	private EditBoxWidget m_PasswordEditBox;

	private Widget m_HideButtonWidget;

	private bool m_IsHide;

	private string m_Password;

	private int m_MaxPasswordLength;

	private bool m_CanUseNumber;
	private bool m_CanUseEngSymbol;
	private bool m_CanUseSpecialCharacter;

	// ()
	private ref ScriptInvoker m_OnFinished;
	// ()
	private ref ScriptInvoker m_OnChange;
	// (Widget w)
	private ref ScriptInvoker m_OnClickHideButton;
	// (Widget w)
	private ref ScriptInvoker m_OnMouseEnterHideButton;
	// (Widget w)
	private ref ScriptInvoker m_OnMouseLeaveHideButton;

	void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;

		m_PasswordEditBox = EditBoxWidget.Cast( w.FindAnyWidget( PasswordEditBox ) );
		m_HideButtonWidget = w.FindAnyWidget( HideButton );

		m_IsHide = !ShowPassword;
		m_MaxPasswordLength = PasswordLength;
		
		m_CanUseNumber = UseNumber;
		m_CanUseEngSymbol = UseEngSymbol;
		m_CanUseSpecialCharacter = UseSpecialCharacter;

		m_Root.SetHandler( this );
	}

	private void UpdatePasswordHideText()
	{
		if ( m_HideButtonWidget ) {
			Widget showImg = m_HideButtonWidget.FindAnyWidget( String( HideButton + "_true" ) );
			Widget hideImg = m_HideButtonWidget.FindAnyWidget( String( HideButton + "_false" ) );

			showImg.Show( !m_IsHide );
			hideImg.Show( m_IsHide );
		}

		if ( m_IsHide ) {
			m_PasswordEditBox.SetText( NewStoryGroupStringFunction.CreateSymbolLength( "*", m_Password.Length() ) );
		}
		else {
			m_PasswordEditBox.SetText( m_Password );
		}
	}

	private bool AllowedCharacter(string character)
	{
		int ascii = character.ToAscii();

		if ( ascii.InRange( 48, 57 ) ) {
			return m_CanUseNumber;
		}

		if ( ascii.InRange( 65, 90 ) || ascii.InRange( 97, 122 ) ) {
			return m_CanUseEngSymbol;
		}

		// ! " # $ % & ' ( ) + , - . / : ; < = > ? @ [ \ ] ^ _` { | } ~ 
		if ( NewStoryGroupMiscFunction.IsSpecialCharacter( character ) ) {
			return m_CanUseSpecialCharacter;
		}

		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( w == m_HideButtonWidget && m_OnMouseEnterHideButton ) {
			m_OnMouseEnterHideButton.Invoke( w );
			return true;
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( w == m_HideButtonWidget && m_OnMouseLeaveHideButton ) {
			m_OnMouseLeaveHideButton.Invoke( w );
			return true;
		}

		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( w == m_HideButtonWidget ) {
			m_IsHide = !m_IsHide;
			UpdatePasswordHideText();

			if ( m_OnClickHideButton ) {
				m_OnClickHideButton.Invoke( w );
			}

			return true;
		}

		return false;
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if ( w != m_PasswordEditBox ) {
			return false;
		}

		string currentPassword = m_Password;

		string text = m_PasswordEditBox.GetText();
		string character = "";

		if ( m_IsHide ) {
			int length = text.LengthUtf8();

			if ( length < m_Password.Length() ) {
				m_Password = m_Password.Substring( 0, length );
			}
			else if ( length <= m_MaxPasswordLength ) {
				character = text.SubstringUtf8( text.LengthUtf8() - 1, 1 );

				if ( AllowedCharacter( character ) ) {
					m_Password += character;
				}
			}

			m_PasswordEditBox.SetText( NewStoryGroupStringFunction.CreateSymbolLength( "*", m_Password.Length() ) );
		}
		else {
			m_Password = "";

			int i = 0;

			while ( i < text.LengthUtf8() && i != m_MaxPasswordLength ) {
				character = text.SubstringUtf8( i, 1 );

				if ( AllowedCharacter( character ) ) {
					m_Password += character;
				}

				i++;
			}

			m_PasswordEditBox.SetText( m_Password );
		}

		if ( m_OnChange && currentPassword != m_Password ) {
			m_OnChange.Invoke();
		}

		if ( m_OnFinished ) {
			if ( finished && m_Password.Length() == m_MaxPasswordLength ) {
				m_OnFinished.Invoke();
			}
		}
		
		return true;
	}

	string GetPassword()
	{
		return m_Password;
	}

	// Использовать с осторожностью
	//  !!! нет проверок на корректность !!!
	void SetPassword(string password)
	{
		m_Password = password;
		UpdatePasswordHideText();
	}

	void SetHideText(bool value)
	{
		if ( m_IsHide != value ) {
			m_IsHide = value;
			UpdatePasswordHideText();
		}
	}

	int GetMaxPasswordLength()
	{
		return m_MaxPasswordLength;
	}

	bool CanUseNumber()
	{
		return m_CanUseNumber;
	}

	bool CanUseEngSymbol()
	{
		return m_CanUseEngSymbol;
	}

	bool CanUseSpecialCharacter()
	{
		return m_CanUseSpecialCharacter;
	}

	// !!! Если надо, то вызывать только после инициализации !!!
	void SetMaxPasswordLength(int value)
	{
		m_MaxPasswordLength = value;
	}

	void SetUseEngSymbol(bool value)
	{
		m_CanUseEngSymbol = value;
	}

	void SetUseNumber(bool value)
	{
		m_CanUseNumber = value;
	}

	void SetSpecialCharacter(bool value)
	{
		m_CanUseSpecialCharacter = value;
	}

	ScriptInvoker Event_OnFinished()
	{
		if ( !m_OnFinished ) {
			m_OnFinished = new ScriptInvoker();
		}

		return m_OnFinished;
	}

	ScriptInvoker Event_OnChange()
	{
		if ( !m_OnChange ) {
			m_OnChange = new ScriptInvoker();
		}

		return m_OnChange;
	}

	ScriptInvoker Event_OnClickHideButton()
	{
		if ( !m_OnClickHideButton ) {
			m_OnClickHideButton = new ScriptInvoker();
		}

		return m_OnClickHideButton;
	}

	ScriptInvoker Event_OnMouseEnterHideButton()
	{
		if ( !m_OnMouseEnterHideButton ) {
			m_OnMouseEnterHideButton = new ScriptInvoker();
		}

		return m_OnMouseEnterHideButton;
	}

	ScriptInvoker Event_OnMouseLeaveHideButton()
	{
		if ( !m_OnMouseLeaveHideButton ) {
			m_OnMouseLeaveHideButton = new ScriptInvoker();
		}

		return m_OnMouseLeaveHideButton;
	}
}
