class NewStoryGroupItemWithAttachmentMethods
{
	static EntityAI SpawnDemo(NewStoryGroupItemWithAttachment data, bool setSettings = false)
	{
		#ifndef SERVER
		EntityAI entity = EntityAI.Cast( GetGame().CreateObjectEx( data.Name, "0 100 0", ECE_LOCAL ) );

		if ( !entity ) {
			return null;
		}

		dBodyDestroy(entity);

		if ( setSettings ) {
			SetSettings( entity, data );
		}

		for ( int i = 0; i < data.Inventory.Count(); i++ ) {
			SpawnInInvetoryDemo( data.Inventory[i], entity, setSettings );
		}

		if ( entity.IsWeapon() ) {
			Weapon_Base weapon = Weapon_Base.Cast( entity );
			weapon.RandomizeFSMState();
		}

		return entity;
		#else
		return null;
		#endif
	}

	static void SpawnInInvetoryDemo(NewStoryGroupItemWithAttachment data, EntityAI entity, bool setSettings = false)
	{
		#ifndef SERVER
		EntityAI newEntity;

		if ( entity.IsWeapon() ) {
			newEntity = entity.GetInventory().CreateAttachment( data.Name );
		}
		else {
			newEntity = entity.GetInventory().CreateInInventory( data.Name );
		}

		if ( !newEntity ) {
			return;
		}

		if ( setSettings ) {
			SetSettings( newEntity, data );
		}

		for ( int i = 0; i < data.Inventory.Count(); i++ ) {
			SpawnInInvetoryDemo( data.Inventory[i], newEntity );
		}

		if ( newEntity.IsWeapon() ) {
			Weapon_Base weapon = Weapon_Base.Cast( newEntity );
			weapon.RandomizeFSMState();
		}
		#endif
	}

	static EntityAI Spawn(NewStoryGroupItemWithAttachment data, vector position)
	{
		#ifdef SERVER
		int flags = ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH;
		
		if ( position[1] == 0 ) {
			flags |= ECE_TRACE;
		}
		#else
		int flags = ECE_LOCAL;
		#endif

		EntityAI entity = EntityAI.Cast( GetGame().CreateObjectEx( data.Name, position, flags ) );

		if ( entity ) {
			#ifndef SERVER
			dBodyDestroy(entity);
			#endif
			SetSettings( entity, data );

			for ( int i = 0; i < data.Inventory.Count(); i++ ) {
				SpawnInInvetory( data.Inventory[i], entity );
			}

			if ( entity.IsWeapon() ) {
				Weapon_Base weapon = Weapon_Base.Cast( entity );

				weapon.RandomizeFSMState();
				#ifdef SERVER
				weapon.Synchronize();
				#endif
			}
		}

		return entity;
	}

	static EntityAI SpawnInInvetory(NewStoryGroupItemWithAttachment data, EntityAI entity, bool inHands = false)
	{
		EntityAI newEntity;

		if ( entity.IsWeapon() ) {
			newEntity = entity.GetInventory().CreateAttachment( data.Name );
		}
		else {
			if ( inHands && entity.IsInherited( Man ) ) {
				Man man = Man.Cast(entity);
				newEntity = man.GetHumanInventory().CreateInHands( data.Name );
			}
			else {
				newEntity = entity.GetInventory().CreateInInventory( data.Name );
			}
		}

		if ( newEntity ) {
			SetSettings( newEntity, data );

			for ( int i = 0; i < data.Inventory.Count(); i++ ) {
				SpawnInInvetory( data.Inventory[i], newEntity );
			}

			if ( newEntity.IsWeapon() ) {
				Weapon_Base weapon = Weapon_Base.Cast( newEntity );

				weapon.RandomizeFSMState();
				#ifdef SERVER
				weapon.Synchronize();
				#endif
			}

			#ifdef SERVER
			string quickBarIndexBuffer = "";
			if ( data.GetArgumentValue( "setinquickbar", quickBarIndexBuffer ) ) {
				PlayerBase player = PlayerBase.Cast( newEntity.GetHierarchyRootPlayer() );
			
				if ( player && player.GetIdentity() != null ) {
					int quickBarIndex = quickBarIndexBuffer.ToInt() - 1;
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call( player.SetQuickBarEntityShortcut, newEntity, quickBarIndex, false );
				}
			}
			#endif
		}

		return newEntity;
	}

	static void SetSettings(EntityAI entity, NewStoryGroupItemWithAttachment data)
	{
		float health = data.GetRandomHealth01() * entity.GetMaxHealth();
		entity.SetHealth( health );

		float randomQuantity = data.GetRandomQuantity();

		if ( randomQuantity == -1 ) {
			return;
		}

		Weapon_Base weapon;
		Magazine magazine;

		string ammoTypeName = "";

		if ( entity.IsInherited( Weapon_Base ) ) {
			weapon = Weapon_Base.Cast(entity);

			if ( weapon.GetMagazineTypeCount(0) == 0 ) {
				int ammoFillFlags = 0;

				if ( data.GetArgumentValue( "ammo", ammoTypeName ) && ammoTypeName == "fullrandom" ) {
					ammoFillFlags |= WeaponWithAmmoFlags.AMMO_MAG_RNG;
					ammoTypeName = "";
				}

				if ( data.HasArgument( "fillchamber" ) ) {
					ammoFillFlags |= WeaponWithAmmoFlags.CHAMBER;
				}

				if ( weapon.HasInternalMagazine(-1) ) {
					weapon.NewStoryGroupFillInnerMagazineWithCount( randomQuantity, ammoTypeName, ammoFillFlags );
				}
				else {
					weapon.NewStoryGroupFillChamberWithCount( randomQuantity, ammoTypeName, ammoFillFlags );
				}
			}
		}
		else if ( entity.IsInherited( Magazine ) ) {
			bool isAmmo = entity.IsInherited( Ammunition_Base );

			magazine = Magazine.Cast(entity);

			int ammoMax = magazine.GetAmmoMax();

			randomQuantity = Math.Floor( randomQuantity * ammoMax );
			randomQuantity = Math.Clamp( randomQuantity, 0, ammoMax );

			bool loadDefaultAmmo = true;                

			if ( !isAmmo && data.GetArgumentValue( "ammo", ammoTypeName ) && ammoTypeName != "fullrandom" ) {
				Magazine ammo = Magazine.Cast( GetGame().CreateObjectEx( ammoTypeName, "0 0 0", ECE_LOCAL ) );

				if ( ammo ) {
					loadDefaultAmmo = false;

					float dmg;
					string ammoType;

					bool acquireCartridge = false;

					// Походу `SetAmmoCount` это такой же перебор только заполняет магазин базовым патроном -.-

					#ifdef SERVER
					if ( ammo.ServerAcquireCartridge( dmg, ammoType ) ) {
						magazine.ServerSetAmmoCount(0);
						acquireCartridge = true;
					}
					#else
					if ( ammo.LocalAcquireCartridge( dmg, ammoType ) ) {
						magazine.LocalSetAmmoCount(0);
						acquireCartridge = true;
					}
					#endif

					if ( acquireCartridge ) {
						for ( int i = 0; i < randomQuantity; i++ ) {
							#ifdef SERVER
							if ( !magazine.ServerStoreCartridge( dmg, ammoType ) ) {
								break;
							}
							#else
							if ( !magazine.LocalStoreCartridge( dmg, ammoType ) ) {
								break;
							}
							#endif
						}
					}

					ammo.Delete();
				}
			}

			if ( loadDefaultAmmo ) {
				#ifdef SERVER
				magazine.ServerSetAmmoCount( randomQuantity );
				#else
				magazine.LocalSetAmmoCount( randomQuantity );
				#endif
			}

			if ( !isAmmo && data.HasArgument( "fillchamber" ) ) {
				weapon = Weapon_Base.Cast( magazine.GetHierarchyParent() );

				if ( weapon ) {
					int muzzleIndex = weapon.GetCurrentMuzzle();
					pushToChamberFromAttachedMagazine( weapon, muzzleIndex );
				}
			}
		}
		else if ( entity.IsInherited(ItemBase) ) {
			ItemBase item = ItemBase.Cast(entity);

			if ( item.GetCompEM() ) {
				item.GetCompEM().SetEnergy0To1( randomQuantity );
				return;
			}

			if ( item.HasQuantity() )
			{
				randomQuantity = Math.Floor( randomQuantity * item.GetQuantityMax() );

				#ifdef SERVER
				item.SetQuantity( randomQuantity );
				#else
				item.SetQuantity( randomQuantity, true, false, true );
				#endif
			}
		}
	}
}
