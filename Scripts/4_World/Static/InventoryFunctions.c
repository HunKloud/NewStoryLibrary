class NewStoryGroupInventoryFunctions
{
    static array<EntityAI> GetItems(EntityAI root_item)
    {
        array<EntityAI> items = new array<EntityAI>();

        if ( !root_item ) return items;

        root_item.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

        for (int i = items.Count() - 1; i >= 0; i--)
            if ( items[i] == root_item || InventoryItemExclusion( items[i] ) )
                items.Remove(i);

        return items;
    }

    static bool InventoryItemExclusion(EntityAI item)
    {
        if ( item.IsInherited(SurvivorBase) )
            return true;

        InventoryLocation il = new InventoryLocation();
        item.GetInventory().GetCurrentInventoryLocation(il);

        if ( il.GetType() == InventoryLocationType.HANDS )
            return true;

        if ( item.IsWeapon() || ( item.IsMagazine() && !il.GetType() == InventoryLocationType.PROXYCARGO ) )
            return false;
        
        if ( item.IsInherited(ToolBase) )
            return false;
        
        if ( !item.IsEmpty() )
            return true;

        if ( il.GetParent() && il.GetParent().IsContainer() )
            return false;
        
        return il.GetType() == InventoryLocationType.ATTACHMENT;
    }

    static array<EntityAI> GetItemsWithQuantityMax(EntityAI root_item, string item_type, int quantity_max01, int health_level = 3, int quantity_min01 = -1)
    {
        array<EntityAI> items = GetItems(root_item);

        for ( int i = items.Count() - 1; i >= 0; i-- )
        {
            ItemBase item = ItemBase.Cast( items[i] );

            if ( !item || item.GetType() != item_type || item.GetHealthLevel() <= health_level || !item.HasQuantity() )
            {
                items.Remove(i);
                continue;
            }

            float quantity01 = item.GetQuantity() / item.GetQuantityMax();

            if ( ( quantity_min01 != -1 && quantity01 < quantity_min01 ) || quantity01 > quantity_max01 )
            {
                items.Remove(i);
                continue;
            }
        }

        return items;
    }

    static bool HasItemsWithQuantityMax(EntityAI root_item, string item_type, int quantity_max01, int health_level = 3, int quantity_min01 = -1)
    {
        return GetItemsWithQuantityMax( root_item, item_type, quantity_max01, health_level, quantity_min01 ).Count() != 0;
    }

    static array<EntityAI> GetItemsWithType(EntityAI root_item, string item_type, int health_level = 3)
    {
        array<EntityAI> items = new array<EntityAI>();

        if ( !root_item ) return items;

        item_type.ToLower();
        root_item.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

        for (int i = items.Count() - 1; i >= 0; i--)
        {
            EntityAI item = items[i];
            string type = item.GetType();
            type.ToLower();

            if ( type != item_type || item.GetHealthLevel() > health_level || InventoryItemExclusion( item ) )
                items.Remove(i);
        }

        return items;
    }

    static bool HasItemsWithTypeCount(EntityAI root_item, string item_type, int count, int health_level = 3)
    {
        return GetItemsWithType( root_item, item_type, health_level ).Count() >= count;
    }

    static EntityAI GetItem(EntityAI root_item, string item_type, int health_level = 3, int quantity_min01 = -1)
    {
        array<EntityAI> items = GetItems(root_item);

        ItemBase item_ib;

        foreach(EntityAI item: items)
        {
            if ( item.GetType() != item_type || item.GetHealthLevel() <= health_level )
                continue;

            if ( quantity_min01 == -1 || ( Class.CastTo( item_ib, item ) && quantity_min01 >= item_ib.GetQuantity() / item_ib.GetQuantityMax() ) )
                return item;
        }

        return null;
    }

    static bool FindItem(EntityAI root_item, string item_type, int health_level = 3, int quantity_min01 = -1)
    {        
        return GetItem( root_item, item_type, health_level, quantity_min01 ) != null;
    }

    static bool DeleteItem(EntityAI root_item, string item_type, int health_level = 3, int quantity_min01 = -1)
    {
        #ifdef SERVER
        EntityAI item = GetItem( root_item, item_type, health_level, quantity_min01 );

        if ( item )
        {
            item.Delete();
            return true;
        }
        #endif
        return false;
    }
}
