

#ifndef SDBASE_H
#define SDBASE_H


#include "sdtype.h"

namespace SGDP {

/**
 * @brief Version structure define of namespace SGDP
 *
 * Every module of SGDP has its own version information.Make sure to get the\n
 * right version DLL and Head file.Otherwise there may be some exceptions\n
 * while using the modules
 */
struct SSGDPVersion
{
	UINT16 wMajorVersion;       ///< This field will increase by 1 when the module was changed from beta version to final version
	UINT16 wMinorVersion;       ///< This field will increase by 1 every time the module's interface changed and conflicted with the last version
	UINT16 wCompatibleVersion;  ///< This field will increase by 1 every time the module's interface changed but compatible with the last version
	UINT16 wBuildNumber;        ///< This field will increase by 1 every official build and never back to zero
};

/**
 * @brief global SGDP version
 */
const SSGDPVersion SGDP_VERSION = { 3, 2, 0, 0 };

/**
 * @brief The base class of modules in namespace SGDP
 *
 * Every module that exports from DLL was inherited from this class
 */
class ISDBase
{
public:
    virtual SDAPI ~ISDBase(void){}

    /**
     * @brief Add the reference of the module instance
     *
     * This method should be called every time the CreateInstance \n
     * method of the module was called
     */
    virtual void SDAPI AddRef(void) = 0;

    /**
     * @brief Query the reference time of the module instance
     * @return the reference number of the instance
     */
    virtual UINT32  SDAPI QueryRef(void) = 0;

    /**
     * @brief Decrease the reference number of the module by 1
     *
     * When decrease to zero, the instance of the module will be destroyed
     */
    virtual void SDAPI Release(void) = 0;

    /**
     * @brief Get the version information of the module
     * @return the version structure of the module
     */
    virtual SSGDPVersion SDAPI GetVersion(void) = 0;

    /**
     * @brief Get the name of the module
     * @return the name string of the module
     */
    virtual const char * SDAPI GetModuleName(void) = 0;
};



}

#endif

