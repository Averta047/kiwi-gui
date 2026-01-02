//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KObject.h"

DWORD KObject::m_IdPool = ID_POOL_START;

KObject::KObject()
{
    this->m_ID = m_IdPool;

    if (m_IdPool >= (WORD)0xFFFFU)
    {
        // out of ID pool
        // todo: throw error
    }
    else
    {
        m_IdPool++;
    }
}

KObject::~KObject()
{
}