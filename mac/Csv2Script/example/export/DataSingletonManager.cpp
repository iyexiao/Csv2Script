#include "GodProj.h"
#include "DataSingletonManager.h"

UDB_Role* UDataSingletonManager::m_DB_Role(NULL);
UDB_Role* UDataSingletonManager::GetDB_Role()
{    if (!m_DB_Role)
    {
        m_DB_Role = NewObject<UDB_Role>();
        m_DB_Role->loadData();
    }
    return m_DB_Role;
}

UDB_Skill* UDataSingletonManager::m_DB_Skill(NULL);
UDB_Skill* UDataSingletonManager::GetDB_Skill()
{    if (!m_DB_Skill)
    {
        m_DB_Skill = NewObject<UDB_Skill>();
        m_DB_Skill->loadData();
    }
    return m_DB_Skill;
}

