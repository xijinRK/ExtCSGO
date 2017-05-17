#include "sdk\IClientEntityList.h"
#include "Netvars.h"
#include "Engine.h"

namespace ExtCSGO::sdk
{

	IClientEntityList::IClientEntityList() : 
		m_Entity(new Player[32]),
		m_Matrix(new s_matrix3x4[32])
	{
	}

	IClientEntityList::~IClientEntityList()
	{
		delete m_Matrix;
		delete m_Entity;
	}

	vec3 IClientEntityList::GetHeadBone(const int & Index) const
	{
		return vec3(m_Matrix[Index].Matrix[0][3], m_Matrix[Index].Matrix[1][3], m_Matrix[Index].Matrix[2][3]);
	}

	Player* IClientEntityList::GetClientEntity(const int & Index) const
	{
		return &m_Entity[Index];
	}

	void IClientEntityList::Update(const Engine *engine) const
	{
		for (int i = 0; i < 32; i++)
		{
			static DWORD Ptr = 0;
			if (!engine->GetProcess()->ReadMemory
			(
				(PVOID)((DWORD64)engine->GetClientDLL()->GetdwBaseAddress() + m_EntityList + i * 0x10),
				&Ptr, sizeof(DWORD)
			))
			{
				continue;
			}

			if (!engine->GetProcess()->ReadMemory
			(
				(PVOID)((DWORD64)Ptr),
				&m_Entity[i], sizeof(Player)
			))
			{
				continue;
			}

			if (!engine->GetProcess()->ReadMemory
			(
				(PVOID)((DWORD64)m_Entity[i].GetdwBoneMatrix() + 8 * 0x30),
				&m_Matrix[i], sizeof(s_matrix3x4)
			))
			{
				continue;
			}
		}
	}	
}