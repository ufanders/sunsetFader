// These definitions are set in the MPLAB Project settings.  If you are starting
// a new project, you should start by modifying one of the pre-existing .mcp 
// files.  To modify the macro used, in MPLAB IDE, click on Project -> Build 
// Options... -> Project -> MPLAB XXX C Compiler -> Preprocessor Macros -> 
// Add.... Note that you may also have to add this macro to the assembler 
// (MPLAB XXX Assembler tab).
#if defined(YOUR_BOARD)
	#include "Configs/HWP YOUR_BOARD.h"
#elif defined(CFG_A1)
    #include "Configs/HWP_A1.h"
#else
	#error "No extended HWP .h included.  Add the appropriate compiler macro to the MPLAB project."
#endif

