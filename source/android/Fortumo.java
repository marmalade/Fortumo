/*
java implementation of the Fortumo extension.

Add android-specific functionality here.

These functions are called via JNI from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
import android.content.Intent;
import com.ideaworks3d.marmalade.LoaderAPI;
import com.ideaworks3d.marmalade.LoaderActivity;
import android.content.Context;

import com.fortumo.android.*;
import com.fernsblossom.*;

class Fortumo
{
    public int FortumoRequestPayment(boolean isConsumable, int creditsMultiplier, String displayString, String productName, String serviceId, String serviceSecret, String SKU)
    {
		//System.out.println("FortumoRequestPayment:\nisConsumable="+isConsumable+"\ncreditsMultiplier="+creditsMultiplier+"\ndisplayString="+displayString+"\nproductName="+productName+"\nserviceId="+serviceId+"\nserviceSecret="+serviceSecret+"\nSKU="+SKU);
		PaymentRequestBuilder builder = new PaymentRequestBuilder();
		builder.setConsumable(isConsumable);
		builder.setCreditsMultiplier(creditsMultiplier/100.0f);
		if (displayString!=null)
			builder.setDisplayString(displayString);
		if (productName!=null)
			builder.setProductName(productName);
		if (serviceId!=null && serviceSecret!=null)
			builder.setService(serviceId,serviceSecret);
		if (SKU!=null)
			builder.setSku(SKU);
		com.fernsblossom.PayActivity.handle=null;
		Intent intent = new Intent(LoaderActivity.m_Activity, com.fernsblossom.PayActivity.class);
		LoaderActivity.m_Activity.startActivity(intent);
		com.fernsblossom.PayActivity.pay(builder);
        return 0;
    }
    public void FortumoTerminate()
    {
		//com.fernsblossom.PayActivity.handle.finish();
		//System.out.println("Fortumo terminated");
    }
    public int FortumoInit()
    {
		//if (com.fernsblossom.PayActivity.handle==null) {
		//}
		//System.out.println("Fortumo initiated");
        return 0;
    }
}
