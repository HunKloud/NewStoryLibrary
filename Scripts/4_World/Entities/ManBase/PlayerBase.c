modded class PlayerBase: ManBase
{
	void NewStoryGroupOtherSendSoundEvent(EPlayerSoundEventID id)
	{
		SendSoundEvent(id);
	}
}
