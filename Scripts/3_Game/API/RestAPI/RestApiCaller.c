class NewStoryGroupRestApiCallerCallback: RestCallback
{
	private NewStoryGroupRestApiCaller m_Caller;

	void SetCaller(NewStoryGroupRestApiCaller caller)
	{
		m_Caller = caller;
	}

	override void OnError(int errorCode)
	{
		if ( m_Caller ) {
			m_Caller.OnError( errorCode );
			m_Caller.Destroy();
		}
	}

	override void OnTimeout()
	{
		if ( m_Caller ) {
			m_Caller.OnTimeout();
			m_Caller.Destroy();
		}
	}

	override void OnSuccess(string data, int dataSize)
	{
		if ( m_Caller ) {
			m_Caller.OnSuccess( data, dataSize );
		}
	}

	override void OnFileCreated(string fileName, int dataSize)
	{
		if ( m_Caller ) {
			m_Caller.OnFileCreated( fileName, dataSize );
		}
	}
}

class NewStoryGroupRestApiCaller: INewStoryGroupRestApiCaller
{
	protected NewStoryGroupRestApi m_Api;

	// 0 - Get
	// 1 - Post
	// 2 - File
	protected int m_MethodType;

	protected string m_Request;
	protected string m_Data;
	protected string m_FileName;

	void NewStoryGroupRestApiCaller(NewStoryGroupRestApi api)
	{
		m_Api					= api;
		m_MethodType	= -1;

		m_Request		= "";
		m_Data			= "";
		m_FileName	= "";
	}

	void ~NewStoryGroupRestApiCaller()
	{
		Destroy();
	}

	void Destroy()
	{
		if ( m_Api && m_MethodType != -1 ) {
			m_MethodType = -1;
			m_Api.RemoveCaller( this );
		}
	}

	protected NewStoryGroupRestApiCallerCallback GetRestCallback()
	{
		auto callback = new NewStoryGroupRestApiCallerCallback();
		callback.SetCaller( this );

		return callback;
	}

	override void Get(string request)
	{
		if ( m_MethodType == -1 ) {
			m_Api.AddCaller( this );

			m_MethodType	= 0;
			m_Request			= request;
		}

		if ( !m_Api.ReConnecting() ) {
			CorrectedRequest( request );
			m_Api.GetRestContext().GET( GetRestCallback(), request );
		}
	}

	override string GetSync(string request)
	{
		CorrectedRequest( request );
		return m_Api.GetRestContext().GET_now( request );
	}

	override void Post(string request, string data = "")
	{
		if ( m_MethodType == -1 ) {
			m_Api.AddCaller( this );

			m_MethodType	= 1;
			m_Request			= request;
			m_Data				= data;
		}

		if ( !m_Api.ReConnecting() ) {
			CorrectedRequest( request );
			m_Api.GetRestContext().POST( GetRestCallback(), request, data );
		}
	}

	override string PostSync(string request, string data = "")
	{
		CorrectedRequest( request );
		return m_Api.GetRestContext().POST_now( request, data );
	}

	override void File(string request, string filename)
	{
		if ( m_MethodType == -1 ) {
			m_Api.AddCaller( this );

			m_MethodType	= 2;
			m_Request			= request;
			m_FileName		= filename;
		}

		if ( !m_Api.ReConnecting() ) {
			CorrectedRequest( request );
			m_Api.GetRestContext().FILE( GetRestCallback(), request, filename );
		}
	}

	override void FileSync(string request, string filename)
	{
		CorrectedRequest( request );
		m_Api.GetRestContext().FILE_now( request, filename );
	}

	override protected void RequestAuthentication()
	{
		if ( !m_Api.ReConnecting() ) {
			m_Api.RequestReAuthentication();
		}
	}

	void OnAuthentication(bool isActive)
	{
		if ( !isActive ) {
			OnAuthenticationFailed();
			Destroy();
			return;
		}

		switch ( m_MethodType ) {
			case 0: {
				Get( m_Request );
				break;
			}
			case 1: {
				Post( m_Request, m_Data );
				break;
			}
			case 2: {
				File( m_Request, m_FileName );
				break;
			}
		}
	}
}
