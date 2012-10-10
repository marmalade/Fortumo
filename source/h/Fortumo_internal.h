/*
 * Internal header for the Fortumo extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef FORTUMO_INTERNAL_H
#define FORTUMO_INTERNAL_H

#include "s3eTypes.h"
#include "Fortumo.h"
#include "Fortumo_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult FortumoInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult FortumoInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void FortumoTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void FortumoTerminate_platform();
s3eResult FortumoRequestPayment_platform(bool isConsumable, int creditsMultiplier, const char* displayString, const char* productName, const char* serviceId, const char* serviceSecret, const char* SKU);

void FortumoTerminate_platform();

s3eResult FortumoInit_platform();

void FortumoGetNonConsumablePaymentStatus_platform(const char* productName);

FortumoMessageStatus FortumoGetLocalNonConsumablePaymentStatus_platform(const char* productName);

void FortumoIsSupportedOperator_platform();
#endif /* !FORTUMO_INTERNAL_H */