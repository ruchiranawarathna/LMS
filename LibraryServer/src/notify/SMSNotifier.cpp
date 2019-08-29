#include "SMSNotifier.h"
#include <iostream>

using namespace std;

void SMSNotifier::notify(string contact, string code, int delay)
{
	cout << "SMSNotifier::notify:" << contact + " book code:" + code  + " delay:" << delay << endl;
	string message = "Your book " + code + " is delayed by " + to_string(delay) + " days. Please return the book at your earliest possible.";
	cout << message << endl;
	string internationalContact = contact.replace(0, 1, "94");
	string url = "curl -X POST http://localhost:7000/sms/send -H 'Content-Type: application/json' -H 'Postman-Token: 93926e1d-9b8a-43df-ae9c-5c70c7002468' -H 'cache-control: no-cache' -d '{\"message\": \"" + message + "\",\"destinationAddresses\": [\"tel:"+ contact + "\"],\"password\": \"password\",\"applicationId\": \"APP_000001\"}'";
	system(url.c_str());
}




