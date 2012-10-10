/*
 * android-specific implementation of the Fortumo extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "Fortumo_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"
#include <string>
#include <cstdio>
#include "s3e.h"
using namespace std;

static jobject g_Obj;
static jmethodID g_FortumoRequestPayment;
static jmethodID g_FortumoTerminate;
static jmethodID g_FortumoInit;
static jmethodID g_FortumoGetNonConsumablePaymentStatus;
static jmethodID g_FortumoGetLocalNonConsumablePaymentStatus;
static jmethodID g_FortumoIsSupportedOperator;

void JNICALL Fortumo_onReceiveCallback(
	JNIEnv* env,
	jobject obj,
	jint billingStatus,
	jstring creditAmount,
	jstring creditName,
	jlong messageId,
	jstring paymentCode,
	jstring priceAmount,
	jstring priceCurrency,
	jstring productName,
	jstring serviceId,
	jstring SKU,
	jstring userId);
	
s3eResult FortumoInit_platform()
{
    //s3eDebugOutputString("FortumoInit_platform");
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;

    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("Fortumo");
    if (!cls)
        goto fail;

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_FortumoRequestPayment = env->GetMethodID(cls, "FortumoRequestPayment", "(ZILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    if (!g_FortumoRequestPayment)
        goto fail;

    g_FortumoTerminate = env->GetMethodID(cls, "FortumoTerminate", "()V");
    if (!g_FortumoTerminate)
        goto fail;

    g_FortumoInit = env->GetMethodID(cls, "FortumoInit", "()I");
    if (!g_FortumoInit)
        goto fail;

	{
        jclass receiver=s3eEdkAndroidFindClass("com/fernsblossom/PaymentStatusReceiver");
        static const JNINativeMethod methods[]=
        {
            {"native_onReceiveCallback","(ILjava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",(void*)&Fortumo_onReceiveCallback},
        };
        // Register the native hooks
        if(env->RegisterNatives(receiver,methods,sizeof(methods)/sizeof(methods[0])))
            goto fail;
    }


    //s3eDebugOutputString("FORTUMO init success");
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    // Add any platform-specific initialisation code here
    return (s3eResult)env->CallIntMethod(g_Obj, g_FortumoInit);

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        //s3eDebugOutputString("One or more java methods could not be found");
    }
    return S3E_RESULT_ERROR;

}

void FortumoTerminate_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FortumoTerminate);
}

s3eResult FortumoRequestPayment_platform(bool isConsumable, int creditsMultiplier, const char* displayString, const char* productName, const char* serviceId, const char* serviceSecret, const char* SKU)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring displayString_jstr = env->NewStringUTF(displayString);
    jstring productName_jstr = env->NewStringUTF(productName);
    jstring serviceId_jstr = env->NewStringUTF(serviceId);
    jstring serviceSecret_jstr = env->NewStringUTF(serviceSecret);
    jstring SKU_jstr = env->NewStringUTF(SKU);
    return (s3eResult)env->CallIntMethod(g_Obj, g_FortumoRequestPayment, isConsumable, creditsMultiplier, displayString_jstr, productName_jstr, serviceId_jstr, serviceSecret_jstr, SKU_jstr);
}
void jstringToNative(jobject obj,const char *c,const char *field,char *output) {
    JNIEnv* env = s3eEdkJNIGetEnv();
	jclass j_class=env->FindClass(c);
    jfieldID jf = env->GetFieldID(j_class,field,"Ljava/lang/String;");
    jstring str=(jstring)env->GetObjectField(obj,jf);
    if(!str)
    {
		output[0]=0;
        return;
    }
    jsize len=env->GetStringLength(str);
    const char* utf=env->GetStringUTFChars(str,NULL);
    //char* retval=new char[len+1];
    //memcpy(retval,utf,len+1); //faster strcpy
	if (len>255) {
		IwTrace(FORTUMO, ("jstringToNative: too long argument"));
		memcpy(output,utf,255); //faster strcpy
		output[255]=0;
	} else
		memcpy(output,utf,len+1); //faster strcpy
    env->ReleaseStringUTFChars(str,utf);
}
void jstringToNative(jstring str,char *output) {
    if(!str)
    {
		output[0]=0;
        return;
    }
    JNIEnv* env = s3eEdkJNIGetEnv();
    jsize len=env->GetStringLength(str);
    const char* utf=env->GetStringUTFChars(str,NULL);
    //char* retval=new char[len+1];
    //memcpy(retval,utf,len+1); //faster strcpy
	if (len>255) {
		IwTrace(FORTUMO, ("jstringToNative: too long argument"));
		memcpy(output,utf,255); //faster strcpy
		output[255]=0;
	} else
		memcpy(output,utf,len+1); //faster strcpy
    env->ReleaseStringUTFChars(str,utf);
}

void JNICALL Fortumo_onReceiveCallback(
	JNIEnv* env,
	jobject obj,
	jint billingStatus,
	jstring creditAmount,
	jstring creditName,
	jlong messageId,
	jstring paymentCode,
	jstring priceAmount,
	jstring priceCurrency,
	jstring productName,
	jstring serviceId,
	jstring SKU,
	jstring userId) {
    ReceiveInfo response;
	response.billingStatus=(FortumoMessageStatus)billingStatus;
	jstringToNative(creditAmount,response.creditAmount);
	jstringToNative(creditName,response.creditName);
	response.messageId=messageId;
	jstringToNative(paymentCode,response.paymentCode);
	jstringToNative(priceAmount,response.priceAmount);
	jstringToNative(priceCurrency,response.priceCurrency);
	jstringToNative(productName,response.productName);
	jstringToNative(serviceId,response.serviceId);
	jstringToNative(SKU,response.SKU);
	jstringToNative(userId,response.userId);
    s3eEdkCallbacksEnqueue(S3E_EXT_FORTUMO_HASH, FORTUMO_RECEIVE_CALLBACK, &response, sizeof(ReceiveInfo));
	char buffer[512];
	sprintf(buffer,"Fortumo_onReceiveCallback billingStatus=%d",response.billingStatus);
	//s3eDebugOutputString(buffer);
}
