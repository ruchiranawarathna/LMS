#ifndef LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_NOTIFY_SMSNOTIFIER_H_
#define LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_NOTIFY_SMSNOTIFIER_H_

#include <iostream>
using namespace std;

class SMSNotifier
{
public:
	SMSNotifier(){}
	~SMSNotifier(){}
	void notify(string contact, string code, int delay);

private:
};



#endif /* LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_NOTIFY_SMSNOTIFIER_H_ */
