//!
//! @file 			Port.hpp
//! @author 		Geoffrey Hunter <gbmhunter@gmail.com> (www.cladlab.com)
//! @created		2013/03/19
//! @last-modified 	2014/01/14
//! @brief 			Contains port-specific skeleton functions for clide-cpp.
//! @details
//!				See README.rst in root dir for more info.

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

//===============================================================================================//
//======================================== HEADER GUARD =========================================//
//===============================================================================================//

#ifndef CLIDE_PORT_H
#define CLIDE_PORT_H

//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

namespace Clide
{

	//===============================================================================================//
	//==================================== PUBLIC DEFINES ===========================================//
	//===============================================================================================//

	#define STR_EXPAND(tok) #tok
	#define STR(tok) STR_EXPAND(tok)
	
	#if(defined __linux__)
		#define ClidePort_PF_UINT32_T		u
		#define ClidePort_PF_INT32_T		i
		#define ClidePort_PF_CHAR_T			c
	#elif(CY_PSOC5)
		#define ClidePort_PF_UINT32_T		lu
		#define ClidePort_PF_INT32_T		li
		#define ClidePort_PF_CHAR_T			li
	#else
		#warning No platform defined. Using defaults.
		#define ClidePort_PF_UINT32_T		u
		#define ClidePort_PF_INT32_T		i
		#define ClidePort_PF_CHAR_T			c
	#endif
		
	//===============================================================================================//
	//=================================== PUBLIC TYPEDEFS ===========================================//
	//===============================================================================================//

	// none

	//===============================================================================================//
	//================================== PUBLIC VARIABLES/STRUCTURES ================================//
	//===============================================================================================//

	// none

	//===============================================================================================//
	//=================================== PUBLIC FUNCTION PROTOTYPES ================================//
	//===============================================================================================//

	class Port
	{

		public:
		
			//===============================================================================================//
			//==================================== CONSTRUCTORS/DESTRUCTOR ==================================//
			//===============================================================================================//
			
			// none
		
			//! @brief		Prints debug messages. Port specific.
			//! @public
			static void DebugPrint(const char* msg);

			//! @brief		Prints messages to the command-line. Port specific.
			static void CmdLinePrint(const char* msg);
	};

} // namespace Clide

#endif	// #ifndef CLIDE_PORT_H

// EOF