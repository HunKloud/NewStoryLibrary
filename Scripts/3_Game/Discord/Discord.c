class NewStoryGroupDiscordRestCallback: RestCallback
{
	override void OnError(int errorCode)
	{
		// Print("[DiscordMsgCallback] : OnError!!! errorCode = " + errorCode);
	}
}

class NewStoryGroupDiscordEmbedAuthor
{
	string name;
	string url;
}

// Картинка под текстов
class NewStoryGroupDiscordEmbedImage
{
	string url;
}

// Картинка справа от описания 
class NewStoryGroupDiscordEmbedThumbnail
{
	string url;
}

class NewStoryGroupDiscordEmbedField
{
	string name;
	string value;
	bool inline;
}

class NewStoryGroupDiscordEmbedFooter
{
	string text;
	string icon_url;
}

class NewStoryGroupDiscordEmbeds
{
	int color;
	string title;
	string description;
	ref NewStoryGroupDiscordEmbedThumbnail thumbnail;
	ref NewStoryGroupDiscordEmbedImage image;
	ref NewStoryGroupDiscordEmbedAuthor author;
	ref array<ref NewStoryGroupDiscordEmbedField> fields;
	ref NewStoryGroupDiscordEmbedFooter footer;
	string timestamp;

	void SetColor(int valueColor)
	{
		color = valueColor;
	}

	void SetTitle(string valueTitle)
	{
		title = valueTitle;
	}

	void SetDescription(string valueDescription)
	{
		description = valueDescription;
	}

	void SetEmbedThumbnail(string valueURL)
	{
		if ( valueURL == "" ) return;

		thumbnail = new NewStoryGroupDiscordEmbedThumbnail();
		thumbnail.url = valueURL;
	}

	void SetEmbedImage(string valueURL)
	{
		if ( valueURL == "" ) return;

		image = new NewStoryGroupDiscordEmbedImage();
		image.url = valueURL;
	}

	void SetEmbedAuthor(string valueName, string valueURL = "")
	{
		author = new NewStoryGroupDiscordEmbedAuthor();
		author.name = valueName;
		author.url = valueURL;
	}

	void AddEmbedField(string valueName, string value, bool valueInline = false)
	{
		if ( !fields ) fields = new array<ref NewStoryGroupDiscordEmbedField>();

		NewStoryGroupDiscordEmbedField field = new NewStoryGroupDiscordEmbedField();
		field.name = valueName;
		field.value = value;
		field.inline = valueInline;

		fields.Insert(field);
	}

	void SetEmbedFooter(string valueText, string valueURL = "")
	{
		footer = new NewStoryGroupDiscordEmbedFooter();
		footer.text = valueText;
		footer.icon_url = valueURL;
	}

	void SetNewStoryGroupGroup()
	{
		SetEmbedFooter( "NEW STORY GROUP" );
	}

	void SetTimeStamp()
	{
		int year, month, day, hour, minute, second;
		GetYearMonthDayUTC(year, month, day);
		GetHourMinuteSecondUTC(hour, minute, second);

		timestamp = string.Format( "%1-%2-%3T%4:%5:%6", year.ToStringLen(4), month.ToStringLen(2), day.ToStringLen(2), hour.ToStringLen(2), minute.ToStringLen(2), second.ToStringLen(2) );
	}
}

class NewStoryGroupDiscordBody
{
	string username;
	string avatar_url;
	ref array<ref NewStoryGroupDiscordEmbeds> embeds;

	void SetUserName(string valueUserName)
	{
		username = valueUserName;
	}

	void SetAvatarUrl(string valueAvatarUrl)
	{
		avatar_url = valueAvatarUrl;
	}

	void AddEmbed(NewStoryGroupDiscordEmbeds embed)
	{
		if ( !embeds ) embeds = new array<ref NewStoryGroupDiscordEmbeds>();
		embeds.Insert(embed);
	}

	void SetNewStoryGroupGroup()
	{
		username	= "NEW STORY GROUP";
		avatar_url  = NewStoryGroupAPI.URI_IMAGE_NEW_STORY_LOGO;
	}
}
