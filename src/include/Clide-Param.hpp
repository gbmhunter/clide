//!
//! @file 		Clide-Param.h
//! @author 	Geoffrey Hunter <gbmhunter@gmail.com> (www.cladlab.com)
//! @date 		2013/04/02
//! @brief 		 
//! @details
//!				See README.rst in root dir for more info.

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

//===============================================================================================//
//======================================== HEADER GUARD =========================================//
//===============================================================================================//

#ifndef CLIDE_PARAM_H
#define CLIDE_PARAM_H

//===============================================================================================//
//========================================== INCLUDES ===========================================//
//===============================================================================================//

#include "Clide-Config.hpp"

//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

namespace Clide
{

	//! @brief		Holds all parameter variables
	//! @details	Object of this type is passed into Rx.RegisterParam() or Tx.Register.Param().
	class Param
	{
		public:
		
			//===============================================================================================//
			//==================================== CONSTRUCTORS/DESTRUCTOR ==================================//
			//===============================================================================================//
			
			//! @brief		Generic constructor.
			Param(bool (*callBackFunc)(char *paramVal), const char* description);
			
			//! @brief		Simplified constructor.
			//! @details	Automatically sets the function pointer to NULL.
			Param(const char* description);
			
			//! @brief		Destructor.
			//! @details	Deallocates memory.
			~Param();
			
			//===============================================================================================//
			//========================================= PUBLIC METHODS ======================================//
			//===============================================================================================//
			
			// none
			
			//===============================================================================================//
			//======================================= PUBLIC VARIABLES ======================================//
			//===============================================================================================//
			
			//! @brief		Description of a parameter. Used with the "-h", "--help" flags.
			char* description;
			
			//! @brief		String value of parameter. 
			//! @note		Parameters have no names
			char value[clideMAX_STRING_LENGTH];
			
			//! @brief		Optional callback function for when parameter is discovered.
			//! @details	Function is called before the callback function for the command that it 
			//! 			was sent in is called.
			bool (*callBackFunc)(char* paramVal);
			
		private:
		
			//! @brief		Common code for constructors
			void Init(bool (*callBackFunc)(char *paramVal), const char* description);
	};

	//===============================================================================================//
	//==================================== PUBLIC DEFINES ===========================================//
	//===============================================================================================//

	// none			

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


} // namespace Clide

#endif	// #ifndef CLIDE_PARAM_H

// EOF
