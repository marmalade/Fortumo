package com.fernsblossom;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.os.Bundle;

import com.fortumo.android.Fortumo;

public class PaymentStatusReceiver extends BroadcastReceiver {

    public static native void native_onReceiveCallback(
	int billingStatus,
	String creditAmount,
	String creditName,
	long messageId,
	String paymentCode,
	String priceAmount,
	String priceCurrency,
	String productName,
	String serviceId,
	String SKU,
	String userId);

	@Override
	public void onReceive(Context context, Intent intent) {
	}
}