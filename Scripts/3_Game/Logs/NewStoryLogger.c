class NewStoryGroupLogger
{
	private FileHandle m_FileHandle;
	private bool m_IsDebug;

	void NewStoryGroupLogger(string path, bool isDebug = false)
	{
		bool hasFile = FileExist( path );
		m_FileHandle = OpenFile( path, FileMode.APPEND );

		if ( m_FileHandle == 0 ) {
			return;
		}

		string firstLog = "";

		if ( hasFile ) {
			firstLog += "\n";
		}

		firstLog += string.Format( "%1: Log started\n", GetTime() );
		FPrintln( m_FileHandle, firstLog );
		m_IsDebug = isDebug;
	}

	void ~NewStoryGroupLogger()
	{
		if ( m_FileHandle != 0 ) {
			CloseFile( m_FileHandle );
		}
	}

	private string GetTime()
	{
		int year, month, day;
		int hour, minute, second;

		GetYearMonthDay( year, month, day );
		GetHourMinuteSecond( hour, minute, second );

		string data  = year.ToString() + "-" + month.ToStringLen(2) + "-" + day.ToStringLen(2);
		string time  = hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);

		return data + " " + time;
	}

	void Info(string text)
	{
		if ( m_FileHandle != 0 ) {
			string log = GetTime() + ": " + text;
			FPrintln( m_FileHandle, log );
		}
	}

	void Warning(string text)
	{
		if ( m_FileHandle != 0 ) {
			string log = GetTime() + ": [WARNING] " + text;
			FPrintln( m_FileHandle, log );
		}
	}
	
	void Error(string text)
	{
		if ( m_FileHandle != 0 ) {
			string log = GetTime() + ": [ERROR]   " + text;
			FPrintln( m_FileHandle, log );
		}
	}
	
	void Debug(string text)
	{
		if ( m_FileHandle != 0 && m_IsDebug ) {
			string log = GetTime() + ": [DEBUG]   " + text;
			FPrintln( m_FileHandle, log );
		}
	}

	void Diag(string text)
	{
		if ( m_FileHandle != 0 && m_IsDebug ) {
			string log = GetTime() + ": [DIAG]    " + text;
			FPrintln( m_FileHandle, log );
		}
	}
}
