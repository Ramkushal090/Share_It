#include "ShareItApp.h"
#include "classes.h"
#include <string>

Manager manager;

JNIEXPORT jint JNICALL Java_com_example_demo1_ShareItApp_getCoins(JNIEnv *env, jobject obj)
{
    return manager.getBalance();
}

JNIEXPORT jobjectArray JNICALL Java_com_example_demo1_ShareItApp_getMyListings(JNIEnv *env, jobject obj)
{
    vector<string> my_listings = manager.getMyListings();
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray javaListings = env->NewObjectArray(my_listings.size(), stringClass, nullptr);
    for (int i = 0; i < my_listings.size(); i++)
    {
        env->SetObjectArrayElement(javaListings, i, env->NewStringUTF(my_listings[i].c_str()));
    }
    return javaListings;
}
JNIEXPORT jobjectArray JNICALL Java_com_example_demo1_ShareItApp_getMyRequests(JNIEnv *env, jobject obj)
{
    vector<string> requests = manager.getRequests();
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray javaRequests = env->NewObjectArray(requests.size(), stringClass, nullptr);
    for (int i = 0; i < requests.size(); i++)
    {
        env->SetObjectArrayElement(javaRequests, i, env->NewStringUTF(requests[i].c_str()));
    }
    return javaRequests;
}

JNIEXPORT jboolean JNICALL Java_com_example_demo1_ShareItApp_saveListingChanges(
    JNIEnv * env, jobject obj,
    jstring listingId, jstring name, jstring condition,
    jstring price, jstring quantity, jstring owner,
    jstring fromDate, jstring toDate)
{

    // Convert Java Strings to C strings
    string cListingId = env->GetStringUTFChars(listingId, nullptr);
    int listing_number = atoi(cListingId.c_str()) + 1;
    string cName = env->GetStringUTFChars(name, nullptr);
    string cCondition = env->GetStringUTFChars(condition, nullptr);
    string cPrice = env->GetStringUTFChars(price, nullptr);
    int price_int = atoi(cPrice.c_str());
    string cQuantity = env->GetStringUTFChars(quantity, nullptr);
    int quantity_int = atoi(cQuantity.c_str());
    string cFromDate = env->GetStringUTFChars(fromDate, nullptr);
    string cToDate = env->GetStringUTFChars(toDate, nullptr);

    if (manager.editListing(listing_number, cName, cCondition, price_int, quantity_int, cFromDate, cToDate, "good"))
    {
        return JNI_TRUE;
    }
    else
    {
        return JNI_FALSE;
    }
}
JNIEXPORT jboolean JNICALL Java_com_example_demo1_ShareItApp_deleteListing(JNIEnv *env, jobject obj, jstring listingId)
{
    printf("Deleting listing...\n");
    // Convert Java String to C string
    const char *cListingId = env->GetStringUTFChars(listingId, nullptr);
    // convert to integar
    int index = atoi(cListingId) + 1;
    if (manager.removeListing(index))
    {
        return JNI_TRUE;
    }
    else
    {
        return JNI_FALSE;
    }
}

JNIEXPORT void JNICALL Java_com_example_demo1_ShareItApp_requestListing(JNIEnv *env, jobject obj, jboolean isListing, jint index)
{
    if (isListing == JNI_TRUE)
    {
        manager.requestOwnerToBorrow(index + 1);
    }
    else
    {
        // do nothing: placeholder
        return;
    }
}
JNIEXPORT jobjectArray JNICALL Java_com_example_demo1_ShareItApp_getNotifications(JNIEnv *env, jobject obj)
{
    vector<string> notifications = manager.getNotificationString();
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray javaNotifications = env->NewObjectArray(notifications.size(), stringClass, nullptr);
    for (int i = 0; i < notifications.size(); i++)
    {
        env->SetObjectArrayElement(javaNotifications, i, env->NewStringUTF(notifications[i].c_str()));
    }
    return javaNotifications;
}
JNIEXPORT jboolean JNICALL Java_com_example_demo1_ShareItApp_sendNotificationResponse(JNIEnv *env, jobject obj, jstring notificationId, jboolean isAccepted)
{
    int index = atoi(env->GetStringUTFChars(notificationId, NULL)) + 1;
    string action;
    if (isAccepted == JNI_TRUE)
    {
        action = "yes";
    }
    else
    {
        action = "no";
    }
return manager.replyToNotification(index, action);
    }

JNIEXPORT jboolean JNICALL Java_com_example_demo1_LoginPage_validateUser(JNIEnv *env, jobject obj, jstring username, jstring password)
{
    string username_cpp = env->GetStringUTFChars(username, NULL);
    string password_cpp = env->GetStringUTFChars(password, NULL);
    if (manager.login(username_cpp, password_cpp)) {
        return JNI_TRUE;
    }
    else {
        if (manager.userExists(username_cpp)) {
            return JNI_FALSE;
        }
        else {
            manager.registerUser(username_cpp, password_cpp);
            return manager.login(username_cpp, password_cpp);
        }
    }

}
JNIEXPORT void JNICALL Java_com_example_demo1_ShareItApp_initializeNative(JNIEnv *env, jobject obj)
{
    printf("Initializing backend resources...\n");
}
JNIEXPORT jobjectArray JNICALL Java_com_example_demo1_ShareItApp_getListings(JNIEnv *env, jobject obj)
{
    vector<string> listings = manager.getListings();
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray javaListings = env->NewObjectArray(listings.size(), stringClass, nullptr);
    for (int i = 0; i < listings.size(); i++)
    {
        string listing = listings[i];
        size_t delimiterPos = listing.find('|');
        string listing_name = listing.substr(0, delimiterPos);
        env->SetObjectArrayElement(javaListings, i, env->NewStringUTF(listing_name.c_str()));
    }
    return javaListings;
}
JNIEXPORT jobjectArray JNICALL Java_com_example_demo1_ShareItApp_getRequests(JNIEnv *env, jobject obj)
{
    vector<string> requests = manager.getRequests();
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray javaRequests = env->NewObjectArray(requests.size(), stringClass, nullptr);
    for (int i = 0; i < requests.size(); i++)
    {
        env->SetObjectArrayElement(javaRequests, i, env->NewStringUTF(requests[i].c_str()));
    }
    return javaRequests;
}
JNIEXPORT jstring JNICALL Java_com_example_demo1_ShareItApp_getItemDetails(JNIEnv *env, jobject obj, jboolean isListing, jint index)
{
    if (isListing == JNI_TRUE)
    {
        string toReturn = manager.getListingDetails(index + 1);
        return env->NewStringUTF(toReturn.c_str());
    }
    else
    {
        string toReturn = manager.getRequestDetails(index + 1);
        return env->NewStringUTF(toReturn.c_str());
    }
}
JNIEXPORT jboolean JNICALL Java_com_example_demo1_ShareItApp_saveNewListing(JNIEnv *env, jobject obj, jstring name, jstring condition, jstring price, jstring quantity, jstring owner, jstring fromDate, jstring toDate)
{
    // convert the java strings into cpp strings
    string name_cpp = env->GetStringUTFChars(name, NULL);
    string condition_cpp = env->GetStringUTFChars(condition, NULL);
    string price_cpp = env->GetStringUTFChars(price, NULL);
    int price_int = atoi(price_cpp.c_str());
    string quantity_cpp = env->GetStringUTFChars(quantity, NULL);
    int quantity_int = atoi(quantity_cpp.c_str());
    string fromDate_cpp = env->GetStringUTFChars(fromDate, NULL);
    string toDate_cpp = env->GetStringUTFChars(toDate, NULL);
    string category = "others";
    return manager.addListing(name_cpp, category, quantity_int, price_int, fromDate_cpp, toDate_cpp, condition_cpp);

}
JNIEXPORT jboolean JNICALL Java_com_example_demo1_ShareItApp_saveNewRequest(JNIEnv *env, jobject obj, jstring name, jstring condition, jstring price, jstring quantity, jstring owner, jstring fromDate, jstring toDate)
{
    // convert the java strings into cpp strings
    string name_cpp = env->GetStringUTFChars(name, NULL);
    string condition_cpp = env->GetStringUTFChars(condition, NULL);
    string price_cpp = env->GetStringUTFChars(price, NULL);
    string quantity_cpp = env->GetStringUTFChars(quantity, NULL);
    string fromDate_cpp = env->GetStringUTFChars(fromDate, NULL);
    string toDate_cpp = env->GetStringUTFChars(toDate, NULL);
    // convert quantity into int
    int quantity_int = atoi(quantity_cpp.c_str());
    return manager.addRequest(name_cpp, "other", quantity_int, fromDate_cpp, toDate_cpp);

}