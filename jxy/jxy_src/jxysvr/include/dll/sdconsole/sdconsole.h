#ifndef SGDP_SDCONSOLE_H_20080125
#define SGDP_SDCONSOLE_H_20080125

#include "sdbase.h"
#include "sdlogger.h"
#include "config.h"

namespace SGDP
{
	
    /**
     * @brief Default fixed area lines of the console screen
     */
	const INT32 DEFAULT_MAX_FIXED_AREA_LINES = 8;
	
    /**
     * @brief Console Module Name
     */
	const char SDCONSOLE_MODULENAME[]        = "SDCONSOLE";

    /**
     * @brief Version of this Module
     */
	const SSGDPVersion	SDCONSOLE_VERSION    = { 3, 2, 0, 0 };

    /**
     * @brief Ctrl+C or Close event hook handler function
     */
	struct  ISDConsoleEventHandler
	{
		virtual void OnCloseEvent() = 0;
		virtual void OnUserCmdEvent(const TCHAR* szCmd) = 0;
	};

    /**
     * @brief Display debug message on the system's console screen
     *        The inner implementation of the ISDConsole in the singleton mode,\n
     *        so the user should use it as a singleton
     *
     * The idea of the SDConsole is that we divided the screen into two areas, one is the
	 * Fixed Area which is used to print the fixed debug message such as on-line number, 
	 * connection number etc. which you'd like to look at every time.The other is the Scroll
	 * Area which is used to print the normal debug message and scrolled automatically.
	 * You can press 'p' to PAUSE and the four direction keys to scroll up or down the screen.
	 * |******************************************************************************|
	 * |																			  |
	 * |					The Fixed Area											  |
	 * |   In line control mode, you should set the line index when writing string.   |
	 * |																			  |
	 * |   In block control mode, you should set the entry index when writing string. |
	 * |   and you can press 'w' and 's' key to scroll up or down the fixed screen.   |
	 * |																			  |
	 * *******************************************************************************|
	 * |																			  |
	 * |					The Scrolling Area										  |
	 * |												  ^					          |
	 * |			This area will auto scroll message	  |							  |
	 * |			You can press 'p' key to pause and <--|-->                        |
	 * |			to scroll up or down. 			      |							  |
	 * |												  v							  |
	 * |																			  |
	 * --------------------------------------------------------------------------------
	 * [view/input] You can input commands defined yourself here. 
	 * --------------------------------------------------------------------------------
     */
	class ISDConsole 
	{
	public:
        /**
         * @brief Initialize console and start it's two inner threads: one is 
		 *        used to popping debug message from the MsgQueue, the other
		 *		  is used to listening the keyboard
         * @param nMaxFixedAreaLines : Fixed Area lines number, which ranged from 0 to 25, 8 for default
         * @param bLineControFlag : true means you can only print a single line in each calling\n
		 *				          WriteStringFixed, the SDConsole will truncate the string extended\n
		 *				          the border, false means you have no need to take care the length limit. The \n
		 *				          SDConsole will print the whole block you passed. In this mode the\n
		 *				          content of fixed area maybe dynamic changed according to your passed\n 
		 *				          string contents
         * @return true means success, false means failure
         */
		virtual bool SDAPI Init(INT32 nMaxFixedAreaLines = DEFAULT_MAX_FIXED_AREA_LINES,
								bool  bLineCtrlFlag      = true) = 0;

        /**
         * @brief Finalize console and stop it's two inner threads
         * @return true means success, false means failure
         */
		virtual bool SDAPI Release(void) = 0;

        /**
         * @brief Close it's internal msg queue
         */
		virtual	void SDAPI CloseMsgQueue(void) = 0;		 

        /**
         * @brief Write string on the Scroll Area of the console screen
         *
         * he following function can be called in multi-threading context\n
         * and it's thread-safe and supported uncertain parameters
         * @param pszFormat : uncertain parameters
         * @return writing number for success, -1 for failed
         */
		virtual INT32 SDAPI WriteString(const TCHAR* pszFormat, ...) = 0;

        /**
         * @brief Write string on the Fixed Area of the console screen
         *
         * Write string on the Fixed Area. You should set which line you want\n
         * to display it. it's also a thread-safe function and	supported\n
         * uncertain parameters
         * @param nIndex : line or block index
         * @param pszFormat : uncertain parameters
         * @return writing : number for success, -1 for failed
         */
		virtual INT32 SDAPI WriteStringFixed(INT32 nIndex, const TCHAR* pszFormat, ...) = 0;

        /**
         * @brief Install a Ctrl+C event hook handler when the SDConsole received Ctrl+C event
         *
         * you can do exit operation such as saving data, release resource\n
         * and etc if any needed. If you didn't set this hook, when the user pressed\n
         * Ctrl+C the SDConsole won't call poHandler->OnCloseEvent()
         * @param poHandler : Ctrl+C or CLOSE event hook handler
         */
		virtual	void SDAPI SetCtrlCHookHandler(ISDConsoleEventHandler* poHandler) = 0;

        /**
         * @brief External thread can drive this method to handle their specific commands
         *
         * In this method, the sdconsole will dequeue a user input commonds each time,\n
         * to let the external thread parsing and do their logic operation.\n
         * You should implement ISDConsoleEventHandler before calling the Run method
         */
		virtual void SDAPI Run(void) = 0;

	};

    /**
     * @brief Get the pointer of ISDConsole instance
     * @param pstVersion : SGDP version string
     * @return ISDConsole Pointer or NULL
     */
	ISDConsole* SDAPI SDConsoleGetModule(const SSGDPVersion* pstVersion);

	typedef ISDConsole* (SDAPI *PFN_SDConsoleGetModule)(const SSGDPVersion* pstVersion);

    /**
     * @brief Set Logger for SDConsole
     * @param poLogger : SDLogger instance pointer
     * @param dwLevel : SDLogger level
     * @return true means success, false means failed
     */
	bool SDAPI SDConsoleSetLogger(ISDLogger* poLogger, UINT32 dwLevel);

	typedef bool (SDAPI *PFN_SDConsoleSetLogger)(ISDLogger*  poLogger, UINT32 dwLevel);
};

#endif //SGDP_SDCONSOLE_H_20080125

