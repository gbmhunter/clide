//!
//! @file 			Cmd.cpp
//! @author 		Geoffrey Hunter <gbmhunter@gmail.com> (www.mbedded.ninja)
//! @created		2013-04-02
//! @last-modified 	2014-10-09
//! @brief 			Command-line style communications protocol
//! @details
//!				See README.rst in repo root dir for more info.

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

//===============================================================================================//
//========================================= INCLUDES ============================================//
//===============================================================================================//

//===== SYSTEM LIBRARIES =====//
#include <stdint.h>		// int8_t, int32_t e.t.c
#include <stdio.h>		// snprintf()
#include <stdlib.h>		// realloc(), malloc(), free()
#include <cctype>		// isalnum() 
#include <string.h>		// strlen()
#include <cinttypes>	// PRIu32

//===== USER LIBRARIES =====//
#include "MAssert/api/MAssertApi.hpp"

//===== USER SOURCE =====//
#include "../include/Config.hpp"
#include "../include/Global.hpp"
#include "../include/Print.hpp"
#include "../include/Option.hpp"
#include "../include/Param.hpp"
#include "../include/Cmd.hpp"


namespace MbeddedNinja
{
	namespace MClideNs
	{

		//===============================================================================================//
		//======================================= PUBLIC METHODS ========================================//
		//===============================================================================================//

		// Constructor
		Cmd::Cmd(MString name, bool (*callBackFunc)(Cmd* foundCmd), MString description)
		{
			#if(clide_ENABLE_DEBUG_CODE == 1)
				// Description too long, do not save it
				Print::PrintDebugInfo(
					"CLIDE: Cmd constructor called.\r\n",
					Print::DebugPrintingLevel::VERBOSE);
			#endif

			// CALLBACK
			this->functionCallback = callBackFunc;

			// Call init function
			this->Init(name, description);

		}

		// Constructor
		Cmd::Cmd(MString name, MCallbacks::Callback<void, Cmd*> methodCallback, MString description)
		{
			#if(clide_ENABLE_DEBUG_CODE == 1)
				// Description too long, do not save it
				Print::PrintDebugInfo("CLIDE: Cmd constructor called.\r\n",
					Print::DebugPrintingLevel::VERBOSE);
			#endif

			this->methodCallback = methodCallback;

			// Must set the function callback to NULL if not given, so that
			// it is not called (would cause segmentation fault)
			this->functionCallback = NULL;

			// Call init function
			this->Init(name, description);
		}
	
		void Cmd::Init(MString name, MString description)
		{
			#if(clide_ENABLE_DEBUG_CODE == 1)
				Print::PrintDebugInfo("CLIDE: Cmd::Init() called.\r\n",
					Print::DebugPrintingLevel::VERBOSE);
			#endif

			// NAME

			this->name = name;

			// DECRIPTION

			this->description = description;

			// HELP

			#if(clide_ENABLE_AUTO_HELP == 1)
				#if(clide_ENABLE_DEBUG_CODE == 1)
					Print::PrintDebugInfo("CLIDE: Registering help option.\r\n", Print::DebugPrintingLevel::GENERAL);
				#endif
				// HELP OPTION
				this->help = new Option('h', "help", NULL, "Prints help for the command.", false);

				M_ASSERT(this->help);

				this->RegisterOption(this->help);
			#endif

			// DETECTED FLAG
			this->isDetected = false;
			
			// PARENT COMM OBJECT

			// Set to null, this gets assigned when the command is registered.
			// Will be either set to a Clide::Tx or Clide::Rx object
			this->parentComm = NULL;

			#if(clide_ENABLE_DEBUG_CODE == 1)
				// Description too long, do not save it
				Print::PrintDebugInfo("CLIDE: Cmd constructor finished.\r\n",
					Print::DebugPrintingLevel::VERBOSE);
			#endif

		}

		Cmd::~Cmd()
		{
			// Destructor
			#if(clide_ENABLE_DEBUG_CODE == 1)	
				Print::PrintDebugInfo("CLIDE: Cmd destructor called.\r\n",
						Print::DebugPrintingLevel::VERBOSE);
			#endif

			#if(clide_ENABLE_AUTO_HELP == 1)
				delete this->help;
			#endif

		}
		
		void Cmd::RegisterParam(Param* param)
		{
			#if(clide_ENABLE_DEBUG_CODE == 1)
				// Description too long, do not save it
				Print::PrintDebugInfo("CLIDE: Registering parameter...\r\n",
						Print::DebugPrintingLevel::VERBOSE);
			#endif

			//this->numParams = 0;

			// MALLOC
			//this->paramA = (Param**)MemMang::AppendNewArrayElement(this->paramA, this->numParams, sizeof(Param*));
			this->paramA.Append(param);
			/*
			if(this->paramA == NULL)
			{
				#if(clide_ENABLE_DEBUG_CODE == 1)
					// Description too long, do not save it
					Print::PrintError("CLIDE: ERROR - Malloc failed while registering parameter.\r\n");
				#endif
				return;
			}
			*/
			// Increase parameter count
			//this->numParams++;

			// Remember the pointer
			//paramA[numParams - 1] = param;
		}

		// Register option
		void Cmd::RegisterOption(Option* option)
		{
			#if(clide_ENABLE_DEBUG_CODE == 1)
				// Description too long, do not save it
				Print::PrintDebugInfo("CLIDE: Registering option...\r\n",
						Print::DebugPrintingLevel::VERBOSE);
			#endif

			// Create option pointer at end of option pointer array.
			//this->optionA = (Option**)MemMang::AppendNewArrayElement(this->optionA, this->numOptions, sizeof(Option*));
			this->optionA.Append(option);

			/*
			if(this->optionA == NULL)
			{
				#if(clide_ENABLE_DEBUG_CODE == 1)
					// Description too long, do not save it
					Print::PrintError("CLIDE: ERROR - Malloc failed while registering option.\r\n");
				#endif
				return;
			}*/

			// Increase option count
			//this->numOptions++;
			
			// Remember option
			//optionA[this->numOptions - 1] = option;
			
			#if(clide_ENABLE_DEBUG_CODE == 1)
				if(optionA[this->optionA.Size() - 1]->shortName != '\0')
				{
					snprintf(
						Global::debugBuff,
						sizeof(Global::debugBuff),
						"CLIDE: Option short name = '%c'. Option long name = '%s'.\r\n",
						optionA[this->optionA.Size() - 1]->shortName,
						optionA[this->optionA.Size() - 1]->longName.cStr);
				}
				else
				{
					snprintf(
						Global::debugBuff,
						sizeof(Global::debugBuff),
						"CLIDE: Option short name = '%s'. Option long name = '%s'.\r\n",
						"none",
						optionA[this->optionA.Size() - 1]->longName.cStr);
				}

				Print::PrintDebugInfo(Global::debugBuff,
						Print::DebugPrintingLevel::VERBOSE);

			#endif
		}

		Option* Cmd::FindOptionByShortName(char shortOptionName)
		{
			for(uint32_t x = 0; x < this->optionA.Size(); x++)
			{
				if(this->optionA[x]->shortName == shortOptionName)
					return this->optionA[x];
			}

			// Option must of not been found, so return NULL
			return NULL;
		}

		Option* Cmd::FindOptionByLongName(MString longOptionName)
		{
			for(uint32_t x = 0; x < this->optionA.Size(); x++)
			{
				if(this->optionA[x]->longName == longOptionName)
					return this->optionA[x];
			}

			// Option must of not been found, so return NULL
			return NULL;
		}

		uint32_t Cmd::NumLongOptions()
		{
			#if(clide_ENABLE_DEBUG_CODE == 1)
				// Description too long, do not save it
				Print::PrintDebugInfo("CLIDE: Calculating num. of long options...\r\n",
						Print::DebugPrintingLevel::VERBOSE);
			#endif

			uint32_t numLongOptions = 0;

			uint32_t x;
			for(x = 0; x < this->optionA.Size(); x++)
			{
				if(this->optionA[x]->longName.GetLength() > 0)
					numLongOptions++;
			}

			#if(clide_ENABLE_DEBUG_CODE == 1)
				snprintf(
					Global::debugBuff,
					sizeof(Global::debugBuff),
					"CLIDE: Num. long options = '%" PRIu32 "'.\r\n",
					numLongOptions);
				Print::PrintDebugInfo(Global::debugBuff,
						Print::DebugPrintingLevel::VERBOSE);
			#endif

			return numLongOptions;
		}

		void Cmd::AddToGroup(CmdGroup *cmdGroup)
		{
			#if(clide_ENABLE_DEBUG_CODE == 1)
				Print::PrintDebugInfo("CLIDE: Adding command to a command group...\r\n",
						Print::DebugPrintingLevel::VERBOSE);
			#endif

			// Create option pointer at end of option pointer array.
			//this->cmdGroupA = (CmdGroup**)MemMang::AppendNewArrayElement(this->cmdGroupA, this->numCmdGroups, sizeof(CmdGroup*));
			this->cmdGroupA.Append(cmdGroup);

			/*
			if(this->cmdGroupA == NULL)
			{
				#if(clide_ENABLE_DEBUG_CODE == 1)
					// Description too long, do not save it
					Print::PrintError("CLIDE: ERROR - Malloc failed while adding command to command group.\r\n");
				#endif
				return;
			}*/

			// Increase command group count
			//this->numCmdGroups++;

			/*
			// Remember the given command group
			this->cmdGroupA[this->numCmdGroups - 1] = cmdGroup;
			*/

			#if(clide_ENABLE_DEBUG_CODE == 1)
				Print::PrintDebugInfo("CLIDE: Added command to a command group.\r\n",
						Print::DebugPrintingLevel::VERBOSE);
			#endif

		}

		uint32_t Cmd::GetNumCmdGroups()
		{
			return this->cmdGroupA.Size();
		}

		CmdGroup* Cmd::GetCmdGroup(uint32_t cmdGroupNum)
		{
			return this->cmdGroupA[cmdGroupNum];
		}

		//===============================================================================================//
		//==================================== PRIVATE FUNCTIONS ========================================//
		//===============================================================================================//

		// none

	} // namespace MClide
} // namespace MbeddedNinja

// EOF
