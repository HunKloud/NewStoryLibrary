modded class Weapon_Base: Weapon
{
    bool NewStoryGroupFillInnerMagazineWithCount(float count01, string ammoType = "", int flags = 0)
    {
        if ( !HasInternalMagazine(-1) ) {
			return false;
        }

        bool randomAmmo     = ammoType == "";
        bool fullRandomAmmo = randomAmmo && ( flags & WeaponWithAmmoFlags.AMMO_MAG_RNG );

        if ( !randomAmmo ) {
            if ( !AmmoTypesAPI.MagazineTypeToAmmoType( ammoType, ammoType ) ) {
				return false;
            }
        }
        else if ( !fullRandomAmmo ) {
            ammoType = GetRandomChamberableAmmoTypeName(0);
        }

        bool didSomething = false;

		int muzzCount = GetMuzzleCount();
        float ammoCountInMuzzle01 = count01 / muzzCount;

        int maxAmmoCount;
        int fillAmmoCount;
        
        for ( int i = 0; i < muzzCount; i++ ) {
            maxAmmoCount = GetInternalMagazineMaxCartridgeCount(i);

            fillAmmoCount = Math.Floor( ammoCountInMuzzle01 * maxAmmoCount );
            fillAmmoCount = Math.Min( fillAmmoCount, maxAmmoCount );

            for ( int j = 0; j < fillAmmoCount; j++ ) {
                if ( fullRandomAmmo ) {
                    ammoType = GetRandomChamberableAmmoTypeName(i);
                }

                didSomething = true;

                if ( !PushCartridgeToInternalMagazine( i, 0, ammoType ) ) {
                    break;
                }
            }
        }

        if ( ( flags & WeaponWithAmmoFlags.CHAMBER ) && FillChamber( ammoType, flags ) ) {
            didSomething = true;
        }

        return didSomething;
    }

    bool NewStoryGroupFillChamberWithCount(float count01, string ammoType = "", int flags = 0)
    {
		int muzzCount = GetMuzzleCount();
		bool anyEmpty = false;

        int i = 0;
		
		for ( ; i < muzzCount; i++ ) {
			if ( IsChamberEmpty(i) ) {
				anyEmpty = true;
				break;
			}
		}

		if ( !anyEmpty ) {
            return false;
        }

        bool randomAmmo     = ammoType == "";
        bool fullRandomAmmo = randomAmmo && ( flags & WeaponWithAmmoFlags.AMMO_CHAMBER_RNG );

        if ( !randomAmmo ) {
            if ( !AmmoTypesAPI.MagazineTypeToAmmoType( ammoType, ammoType ) ) {
				return false;
            }
        }
        else if ( !fullRandomAmmo ) {
            ammoType = GetRandomChamberableAmmoTypeName(0);
        }

        bool didSomething = false;

        int count = Math.Floor( count01 * muzzCount );

        if ( count > 0 ) {
            for ( i = 0; i < muzzCount; i++ ) {
                if ( !IsChamberEmpty(i) ) {
                    continue;
                }

                if ( fullRandomAmmo ) {
                    ammoType = GetRandomChamberableAmmoTypeName(i);
                }

                PushCartridgeToChamber( i, 0, ammoType );
                didSomething = true;

                count--;

                if ( count <= 0 ) {
                    break;
                }
            }
        }
		
		if ( !didSomething ) {
            return false;
        }
		
		RandomizeFSMState();		
		Synchronize();
		
		return true;
    }
}
