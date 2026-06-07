#pragma once
#ifndef _scimitar
#define _scimitar
//
#include "../utils/spoofcall/spoofcall.h"
#include "../scimitar/engine/havok_math.hpp"
#include "../scimitar/engine/ida_defs.hpp"
#include "../External/imgui/imgui.h"


#include <iostream>
#include <thread>


namespace scimitar 
{

	class Entity;

	enum class entity_status
	{
		INVALID,
		DEAD_1,
		DEAD_2,
		TEAM,
		LOCAL,
		VALID
	};

	class view_translation;

	class view_translation {

	private:
		
		static inline uint64_t temp = 0; //<-- Store View Translation Address

	public:

		static auto init( ) -> bool { //<-- View Translation Init (Auto Updating)

			utils::memory::search_pattern(
				"A4 70 7D BF 00 00 00 00 00 00 00 00 00 00 A0 40 00 00 A0 C0 00 00 00 00 00 00 00 00 CD CC 4C 3F 00 00 00 3F 00 00 80 3E",
				temp,
				-0x2A4,
				false
			);

			return true;
		}


		static view_translation* get_instance( ) { // <-- View Translition Instance

			return reinterpret_cast<view_translation*>( temp );


		}

	
		auto m_ViewMatrix( ) -> const ubiViewMatrix& // <-- View Matrix
		{
			return *reinterpret_cast<ubiViewMatrix*>( this + 0x250 );
		}

		auto m_ViewOffset( ) -> ubiVector4 { // <-- Camera Position

			if (!utils::memory::valid_pointer(this)) {

				return ubiVector4();
			}

			return *reinterpret_cast<ubiVector4*>(this + 0x190);
		}

		auto world_to_screen( const ubiVector4& world , ubiVector2& screen ) -> bool // <-- World To Screen Function
		{
			
			const ubiViewMatrix& vm = this->m_ViewMatrix();

			float w = vm[0][3] * world.x + vm[1][3] * world.y + vm[2][3] * world.z + vm[3][3];

			if (w < 0.001f)
				return false;

			float x = world.x * vm[0][0] + world.y * vm[1][0] + world.z * vm[2][0] + vm[3][0];
			float y = world.x * vm[0][1] + world.y * vm[1][1] + world.z * vm[2][1] + vm[3][1];


			float nx = x / w;
			float ny = y / w;


			const ImVec2 size = ImGui::GetIO().DisplaySize;


			screen.x = (size.x * 0.5f * nx) + (size.x * 0.5f);
			screen.y = -(size.y * 0.5f * ny) + (size.y * 0.5f);

			if (screen.x < 0 || screen.y < 0 || screen.x > size.x || screen.y > size.y)
			{
				return false;
			}

			return true;
		}

	};


	class Entity
	{


	public:

		auto m_Origin( ) -> ubiVector4 { // <-- Entity Position

			if (!utils::memory::valid_pointer( this ))
				return ubiVector4( );

			return *reinterpret_cast<ubiVector4*>( this + 0x50 );

		}
		

		auto m_Status() -> entity_status // <-- Entity Status (Used For Team, isAlive, local_entity)
		{

			if (!utils::memory::valid_pointer(this))
				return entity_status::INVALID;


			unsigned long long bitfield = *(unsigned long long*)(this + 0xB0);
			unsigned char byte3 = (bitfield >> 24) & 0xFF;
			unsigned char byte4 = (bitfield >> 32) & 0xFF;


			if (byte3 == 0xFA)
				return entity_status::DEAD_1;


			if (byte4 == 0x80)
				return entity_status::DEAD_2;

			if (byte4 == 0x02)
				return entity_status::TEAM;

			if (byte4 == 0x00) 
				return entity_status::LOCAL;


			return entity_status::VALID;
		}

		auto isA() -> bool // <-- Entity Filter (Filters Out Random Objects on the Map)
		{
			if (!utils::memory::valid_pointer( this )) 
				return false;


			int id = *(int*)( this + 0x1C );
			return id > 0 && id < 1000;
		}

	};

  

    inline auto Init( ) -> void
    {

		scimitar::view_translation::init( ); // <-- Inits View Translation

    }

}


#endif