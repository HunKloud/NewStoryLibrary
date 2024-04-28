class INewStoryGroupRestApiCaller
{
	// Constructor
	// void INewStoryGroupRestApiCaller(NewStoryGroupRestApi api);
	
	void Get(string request);
	// Not recommended
	string GetSync(string request);

	void Post(string request, string data = "");
	// Not recommended
	string PostSync(string request, string data = "");

	void File(string request, string filename);
	// Not recommended
	void FileSync(string request, string filename);

	// Look 'ERestResultState'
	// Called in case request failed
	void OnError(int errorCode);
	// Called in case request timed out or handled improperly
	void OnTimeout();
	// Called when data arrived and/ or response processed successfully
	void OnSuccess(string data, int dataSize);
	// Called when data arrived and/ or file created successfully
	void OnFileCreated(string fileName, int dataSize);

	// If required modify request, use it
	// Called every time the methods are called: Get, Post, File
	protected void CorrectedRequest(inout string request);
	
	// Requires re-authentication
	protected void RequestAuthentication();
	// Called when authentication failed
	protected void OnAuthenticationFailed();
}
