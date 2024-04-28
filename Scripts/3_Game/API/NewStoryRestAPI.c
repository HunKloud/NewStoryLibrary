class NewStoryGroupRestApi: INewStoryGroupRestApi
{
	private RestContext m_RestContext;
	private ref array<ref NewStoryGroupRestApiCaller> m_Callers;

	private string m_AuthenticationKey;
	private bool m_IsActive;
	private bool m_ReConnecting;

	private ref ScriptInvoker Event_OnUpdateIsActive;
	private ref ScriptInvoker Event_OnUpdateAuthKey;
	private ref ScriptInvoker Event_OnRequestReAuth;

	private ref ScriptCaller m_AuthCallback;

	override void Init(string url, ScriptCaller authCallback)
	{
		if ( !GetRestApi() ) {
			CreateRestApi();
		}

		m_ReConnecting	= false;
		m_IsActive			= false;

		m_Callers			= new array<ref NewStoryGroupRestApiCaller>();
		m_RestContext = GetRestApi().GetRestContext( url );

		m_AuthCallback = authCallback;
	}

	override void OnAuthentication(string key = "")
	{
		m_AuthenticationKey = key;
		
		m_ReConnecting = false;
		SetIsActive( key != "" );

		if ( Event_OnUpdateAuthKey ) {
			Event_OnUpdateAuthKey.Invoke( key != "");
		}

		if ( m_Callers.Count() != 0 ) {
			for ( int i = 0; i < m_Callers.Count(); i++ ) {
				NewStoryGroupRestApiCaller caller = m_Callers[i];

				if ( caller ) {
					caller.OnAuthentication( m_IsActive );
				}
			}
		}
	}

	override void RequestReAuthentication()
	{
		if ( !m_ReConnecting ) {
			m_ReConnecting = true;

			if ( Event_OnRequestReAuth ) {
				Event_OnRequestReAuth.Invoke();
			}

			m_AuthCallback.Invoke();
		}
	}

	override string GetAuthenticationKey()
	{
		return m_AuthenticationKey;
	}

	void AddCaller(NewStoryGroupRestApiCaller caller)
	{
		m_Callers.Insert( caller );
	}

	void RemoveCaller(NewStoryGroupRestApiCaller caller)
	{
		m_Callers.RemoveItemUnOrdered( caller );
	}

	override void SetIsActive(bool isActive)
	{
		if ( m_IsActive != isActive ) {
			m_IsActive = isActive;

			if ( Event_OnUpdateIsActive ) {
				Event_OnUpdateIsActive.Invoke( m_IsActive );
			}
		}
	}

	override bool IsActive()
	{
		return m_IsActive;
	}

	override bool ReConnecting()
	{
		return m_ReConnecting;
	}

	override RestContext GetRestContext()
	{
		return m_RestContext;
	}

	override ScriptInvoker OnUpdateIsActive()
	{
		if ( !Event_OnUpdateIsActive ) {
			Event_OnUpdateIsActive = new ScriptInvoker();
		}

		return Event_OnUpdateIsActive;
	}

	override ScriptInvoker OnRequestReAuth()
	{
		if ( !Event_OnRequestReAuth ) {
			Event_OnRequestReAuth = new ScriptInvoker();
		}

		return Event_OnRequestReAuth;
	}

	override ScriptInvoker OnUpdateAuthKey()
	{
		if ( !Event_OnUpdateAuthKey ) {
			Event_OnUpdateAuthKey = new ScriptInvoker();
		}

		return Event_OnUpdateAuthKey;
	}
}
