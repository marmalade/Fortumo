package com.fernsblossom;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.content.Intent;

import com.fortumo.android.Fortumo;
import com.fortumo.android.PaymentActivity;
import com.fortumo.android.PaymentRequestBuilder;
import com.fortumo.android.PaymentResponse;


public class PayActivity extends PaymentActivity {
	static public PayActivity handle;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		//System.out.println("PayActivity::onCreate");
		// Enable the broadcast to receive status updates _Available from v7.1_
		//Fortumo.enablePaymentBroadcast(this, "com.fernsblossom.PAYMENT_BROADCAST_PERMISSION");
		super.onCreate(savedInstanceState);
		//setContentView(R.layout.main);
			//handle=this;
	}
	public void onStart() {
		//System.out.println("PayActivity::onStart");
		super.onStart();
		if (handle==null) {
			handle=this;
			//finish();
		}
	}
	private ReceiveInfo convertPaymentResponseToReceiveInfo(PaymentResponse response) {
		ReceiveInfo info=new ReceiveInfo();
		info.billingStatus = response.getBillingStatus();
		//System.out.println("convertPaymentResponseToReceiveInfo, pre info.billingStatus="+info.billingStatus);
		switch (info.billingStatus) {
			case Fortumo.MESSAGE_STATUS_BILLED :
			info.billingStatus=0;
			break;
			case Fortumo.MESSAGE_STATUS_FAILED  :
			info.billingStatus=1;
			break;
			case Fortumo.MESSAGE_STATUS_NOT_SENT   :
			info.billingStatus=2;
			break;
			case Fortumo.MESSAGE_STATUS_PENDING    :
			info.billingStatus=3;
			break;
		}
		//System.out.println("convertPaymentResponseToReceiveInfo, post info.billingStatus="+info.billingStatus);
		info.creditAmount = response.getCreditAmount();
		info.creditName = response.getCreditName();
		info.messageId = response.getMessageId();
		info.paymentCode = response.getPaymentCode();
		info.priceAmount = response.getPriceAmount();
		info.priceCurrency = response.getPriceCurrency();
		info.productName = response.getProductName();
		info.serviceId = response.getServiceId();
		info.SKU = response.getSku();
		info.userId = response.getUserId();
		PaymentStatusReceiver.native_onReceiveCallback(
			info.billingStatus,
			response.getCreditAmount(),
			response.getCreditName(),
			response.getMessageId(),
			response.getPaymentCode(),
			response.getPriceAmount(),
			response.getPriceCurrency(),
			response.getProductName(),
			response.getServiceId(),
			response.getSku(),
			response.getUserId());
		return info;
	}
	protected void onPaymentCanceled(PaymentResponse response) {
		//System.out.println("onPaymentCanceled");
		convertPaymentResponseToReceiveInfo(response);
		finish();
		//PaymentStatusReceiver.native_onReceiveCallback(convertPaymentResponseToReceiveInfo(response));
	}
	protected void onPaymentFailed(PaymentResponse response) {
		//System.out.println("onPaymentFailed");
		convertPaymentResponseToReceiveInfo(response);
		finish();
		//PaymentStatusReceiver.native_onReceiveCallback(convertPaymentResponseToReceiveInfo(response));
	}
	protected void onPaymentPending(PaymentResponse response) {
		//System.out.println("onPaymentPending");
		convertPaymentResponseToReceiveInfo(response);
		finish();
		//PaymentStatusReceiver.native_onReceiveCallback(convertPaymentResponseToReceiveInfo(response));
	}
	protected void onPaymentSuccess(PaymentResponse response) {
		//System.out.println("onPaymentSuccess");
		convertPaymentResponseToReceiveInfo(response);
		finish();
		//PaymentStatusReceiver.native_onReceiveCallback(convertPaymentResponseToReceiveInfo(response));
	}
	public static void pay(PaymentRequestBuilder req) {
        try {
			while (handle==null)
				Thread.sleep(100);
			handle.makePayment(req.build());
		} catch (Exception e) {
			System.out.println("exception during payment :\n"+e.getMessage()+"\n"+e.toString());
		}
	}
	
}