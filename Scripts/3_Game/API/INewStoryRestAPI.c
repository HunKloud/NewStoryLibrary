class INewStoryGroupRestApi
{
	void Init(string url, ScriptCaller authCallback);
	
	void SetIsActive(bool isActive);

	// If is active return 'true'
	bool IsActive();
	// If in state 're-authentication' return 'true'
	bool ReConnecting();

	// Required call when authentication
	// if 'key == ""', then api is not active
	// Requires used super.OnAuthentication( key );
	void OnAuthentication(string key = "");

	// Call when request re-authentication
	void RequestReAuthentication();

	// Return your session token
	string GetAuthenticationKey();

	RestContext GetRestContext();

	// Called when updated 'active' (bool isActive)
	ScriptInvoker OnUpdateIsActive();

	ScriptInvoker OnRequestReAuth();

	// (bool success)
	ScriptInvoker OnUpdateAuthKey();
}
