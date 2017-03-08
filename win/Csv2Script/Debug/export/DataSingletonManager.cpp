#include "GodProj.h"
#include "DataSingletonManager.h"

UDB_Skill* UDataSingletonManager::m_DB_Skill(NULL);
UDB_Skill* UDataSingletonManager::GetDB_Skill()
{    if (!m_DB_Skill)
    {
        m_DB_Skill = NewObject<UDB_Skill>();
        m_DB_Skill->loadData();
    }
    return m_DB_Skill;
}

