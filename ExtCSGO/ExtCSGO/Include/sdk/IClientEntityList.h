#pragma once
#include <vector>
#include "..\Maths\Vector.h"

namespace ExtCSGO
{
	class Engine;
	class vec3;
}

namespace ExtCSGO::sdk
{
	class Player;

	struct s_matrix3x4
	{
		matrix3x4_t Matrix;
	};

	class IClientEntityList
	{
		Player*        m_Entity;
		s_matrix3x4*   m_Matrix;
	public:
		IClientEntityList();
		~IClientEntityList();

		vec3 GetHeadBone(const int & Index) const;
		Player* GetClientEntity(const int & Index) const;

		void Update(const Engine *engine) const;
	};
}