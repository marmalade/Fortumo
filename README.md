Fortumo
===

Fortumo in-app purchasing on Android SDK allows easy to use payments inside mobile apps and games.

### Usage

To initialise a payment you need to build a Fortumo_PaymentRequest object and call the Fortumo_MakePayment method.
When a payment is completed, notification about that will be sent to the callback function.
If the billing has been succesful, the user should be given the virtual goods bought and the purchase should be stored
on the device or on server-side so that users will keep access to the goods:

	Fortumo_PaymentRequest *request = Fortumo_PaymentRequest_Create();
	Fortumo_PaymentRequest_SetDisplayString(request, "display_string_here");
    Fortumo_PaymentRequest_SetService(request, "service_id_here", "app_secret_here");
	Fortumo_PaymentRequest_SetProductName(request, "product_name_here");
    Fortumo_PaymentRequest_SetType(request, Fortumo_ServiceType_Consumable);
    Fortumo_MakePayment(request, &Fortumo_OnPaymentComplete, NULL);
    Fortumo_PaymentRequest_Delete(request);

Once the purchase has been processed, a notification will be sent to your callback function.
You can decide to give users access to premium features upon the notification on the device or
if you are keeping track of users purchases on your server you will update purchase data from there.

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

See samples/DemoPayment for a full example.

### Documentation

Read more about integration: http://developers.fortumo.com/in-app-purchasing-for-marmalade/