// http://howardhinnant.github.io/date_algorithms.html
class NewStoryGroupTimeData: Managed
{
	int Year		= 1970;
	int Month	   = 1;
	int Day		 = 1;
	int Hour		= 0;
	int Minute	  = 0;
	int Second	  = 0;

	private static const int LAST_DAY_IN_MONTH[12]  = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	void SetFirstTime()
	{
		Year	= 1970;
		Month   = 1;
		Day	 = 1;
		Hour	= 0;
		Minute  = 0;
		Second  = 0;
	}

	void SetWorldTime()
	{
		GetYearMonthDay( Year, Month, Day );
		GetHourMinuteSecond( Hour, Minute, Second );
	}

	void SetUTCWorldTime()
	{
		GetYearMonthDayUTC( Year, Month, Day );
		GetHourMinuteSecondUTC( Hour, Minute, Second );
	}

	void SetGameTime()
	{
		GetGame().GetWorld().GetDate( Year, Month, Day, Hour, Minute );
	}

	string ConvertToDMY()
	{
		return string.Format( "%1:%2:%3", Day.ToStringLen(2), Month.ToStringLen(2), Year.ToStringLen(4) );
	}

	void ConvertFromDMY(string data)
	{
		array<string> values = new array<string>;

		data.Split(":", values);

		if ( values.Count() != 3 ) {
			return;
		}

		string s_day = values[0], s_month = values[1], s_year = values[2];

		if ( s_day.Length() != 2 || s_month.Length() != 2 || s_year.Length() != 4 ) {
			return;
		}

		Day = s_day.ToInt();
		Month = s_month.ToInt();
		Year = s_year.ToInt();
	}

	string ConvertToHMS()
	{
		return string.Format( "%1:%2:%3", Hour.ToStringLen(2), Minute.ToStringLen(2), Second.ToStringLen(2) );
	}

	void ConvertFromHMS(string data)
	{
		array<string> values = new array<string>;

		data.Split(":", values);

		if ( values.Count() != 3 ) {
			return;
		}

		string s_hour = values[0], s_minute = values[1], s_second = values[2];

		if ( s_hour.Length() != 2 || s_minute.Length() != 2 || s_second.Length() != 2 ) {
			return;
		}

		Hour = s_hour.ToInt();
		Minute = s_minute.ToInt();
		Second = s_second.ToInt();
	}

	string ConvertToHM()
	{
		return string.Format( "%1:%2", Hour.ToStringLen(2), Minute.ToStringLen(2) );
	}

	string ConvertToDMY_HMS()
	{
		return ConvertToDMY() + "_" + ConvertToHMS();
	}

	void ConvertFromDMY_HMS(string data)
	{
		array<string> values = new array<string>;

		data.Split("_", values);

		if ( values.Count() != 2 ) {
			return;
		}

		ConvertFromDMY( values[0] );
		ConvertFromHMS( values[1] );
	}

	string ConvertToDMY_HM()
	{
		return ConvertToDMY() + "_" + ConvertToHM();
	}

	static bool IsLeapYear(int y)
	{
		return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
	}

	static int GetLastDayOfMonth(int year, int month)
	{
		int lastDay = LAST_DAY_IN_MONTH[month - 1];

		if ( month == 2 && IsLeapYear( year ) ) {
			lastDay++;
		}

		return lastDay;
	}

	int GetUnixSeconds()
	{
		return GetUnixSeconds( this );
	}

	static int GetUnixSeconds(NewStoryGroupTimeData data)
	{
		return GetUnixDay(data) * 86400 + data.Hour * 3600 + data.Minute * 60 + data.Second;
	}

	int GetUnixDay()
	{
		return GetUnixDay( this );
	}

	static int GetUnixDay(NewStoryGroupTimeData data)
	{
		int month   = data.Month;
		int year	= data.Year;
		
		if ( month <= 2 ) {
			year--;
		}

		int era = year;

		if ( year < 0 ) {
			era = year - 399;
		}

		era /= 400;

		int yoe = year - era * 400;
		
		if ( month > 2 ) {
			month -= 3;
		}
		else {
			month += 9;
		}

		int doy = (153 * month + 2) / 5 + data.Day - 1;
		int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;

		return era * 146097 + doe - 719468;
	}

	static int GetNumberOfDaysBetweenDates(NewStoryGroupTimeData data1, NewStoryGroupTimeData data2)
	{
		int unixDay1 = GetUnixDay( data1 );
		int unixDay2 = GetUnixDay( data2 );

		return unixDay1 - unixDay2;
	}

	static int GetNumberOfSecondsBetweenDates(NewStoryGroupTimeData data1, NewStoryGroupTimeData data2)
	{
		int unixDay1 = GetUnixDay( data1 );
		int unixDay2 = GetUnixDay( data2 );

		int day = unixDay1 - unixDay2;
		int hour = data1.Hour - data2.Hour;
		int minute = data1.Minute - data2.Minute;
		int second = data1.Second - data2.Second;

		return day * 86400 + hour * 3600 + minute * 60 + second;
	}

	static string GetCurrentTime()
	{
		int hour, minute, second;
		GetHourMinuteSecond(hour, minute, second);
		return hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);
	}

	static string GetCurrentTimeUTC()
	{
		int hour, minute, second;
		GetHourMinuteSecondUTC(hour, minute, second);
		return hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);
	}

	static string ConvertSecondsToTimeAbbrev(int seconds)
	{
		int d = seconds / 86400;
		seconds -= d * 86400;
		
		int h = seconds / 3600;
		seconds -= h * 3600;
		
		int m = seconds / 60;
		seconds -= m * 60;

		string text = seconds.ToString() + " " + Widget.TranslateString( "#STR_time_unit_abbrev_second_0" );

		if ( m > 0 ) {
			text = m.ToString() + " " + Widget.TranslateString( "#STR_time_unit_abbrev_minute_0" ) + text;
		}

		if ( h > 0 ) {
			text = h.ToString() + " " + Widget.TranslateString( "#STR_time_unit_abbrev_hour_0" ) + text;
		}

		if ( d > 0 ) {
			text = d.ToString() + " " + Widget.TranslateString( "#STR_time_unit_abbrev_day_0" ) + text;
		}

		return text;
	}
}
