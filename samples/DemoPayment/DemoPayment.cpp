#include "Fortumo.h"
#include "s3eDevice.h"
#include "s3ePointer.h"
#include "s3eSurface.h"
#include "s3eTimer.h"
#include "IwDebug.h"

static void Fortumo_OnPaymentComplete(Fortumo_PaymentResponse *response, void *userData)
{
    int billingStatus = Fortumo_PaymentResponse_GetBillingStatus(response);
    
    switch(billingStatus) {
        case Fortumo_BillingStatus_NotSent:
            // The payment flow was cancelled by the user (eg. Back button)
            break;
        case Fortumo_BillingStatus_Pending:
            // The payment was probably successful, but isn't completely processed yet or the UI was closed too soon.
            // 1. Assume that the payment was successful without waiting.
            // 2. Use Fortumo_SetStateChangeListener() to get notified as soon as possible
            // 3. Use Fortumo_FindPayment() later to check if the status has changed.
            // 4. Fail. If the payment was successful, then Fortumo_MakePayment() should return the 'Billed' status next time.
            break;
        case Fortumo_BillingStatus_Billed:
            // The payment was successful.
            break;
        case Fortumo_BillingStatus_Failed:
            // The payment was not successful.
            break;
        default:
            // Try alternative payment methods if possible.
            break;
    }
    
    Fortumo_PaymentResponse_Delete(response);
}

S3E_MAIN_DECL int main()
{
    s3eBool available = FortumoAvailable();
    
	Fortumo_SetLoggingEnabled(true);
	
    while(!s3eDeviceCheckQuitRequest()) {
        s3eDeviceYield(0);
        s3ePointerUpdate();
        s3eDebugPrint(0, 30, (available) ? "Fortumo (OK)" : "Fortumo (ERR)", 0);
        s3eSurfaceShow();
        
        if(available && (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_PRESSED)) {
            Fortumo_PaymentRequest *request = Fortumo_PaymentRequest_Create();
            
            Fortumo_PaymentRequest_SetDisplayString(request, "display_string_here");
            Fortumo_PaymentRequest_SetService(request, "service_id_here", "app_secret_here");
            Fortumo_PaymentRequest_SetProductName(request, "product_name_here");
            Fortumo_PaymentRequest_SetConsumable(request, true);
            Fortumo_MakePayment(request, &Fortumo_OnPaymentComplete, NULL);
            Fortumo_PaymentRequest_Delete(request);
        }
    }
    
    return 0;
}
