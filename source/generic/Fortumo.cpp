/*
Generic implementation of the Fortumo extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "Fortumo_internal.h"
s3eResult FortumoInit()
{
    //Add any generic initialisation code here
    return FortumoInit_platform();
}

void FortumoTerminate()
{
    //Add any generic termination code here
    FortumoTerminate_platform();
}

s3eResult FortumoRequestPayment(bool isConsumable, int creditsMultiplier, const char* displayString, const char* productName, const char* serviceId, const char* serviceSecret, const char* SKU)
{
	return FortumoRequestPayment_platform(isConsumable, creditsMultiplier, displayString, productName, serviceId, serviceSecret, SKU);
}
