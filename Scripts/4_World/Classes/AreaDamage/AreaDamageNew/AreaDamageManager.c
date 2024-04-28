modded class AreaDamageManager: AreaDamageEvents
{
	void SetNewStoryGroupCollisionCylinder(float radius, float height)
	{
		if ( m_AreaDamageTrigger ) {
			m_AreaDamageTrigger.SetCollisionCylinder(radius, height);
		}
	}

	void SetNewStoryGroupCollisionSphere(float radius)
	{
		if ( m_AreaDamageTrigger ) {
			m_AreaDamageTrigger.SetCollisionSphere(radius);
		}
	}
}
