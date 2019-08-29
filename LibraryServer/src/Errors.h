/*
 * Errors.h
 *
 *  Created on: Jul 15, 2019
 *      Author: hsenid
 */

#ifndef LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_ERRORS_H_
#define LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_ERRORS_H_

class Errors {
public:
	static const int DUPLICATE_USER_ID = 800;
	static const int DUPLICATE_BOOK_CODE = 801;
	static const int INVALID_USER_ID = 802;
	static const int INVALID_BOOK_CODE = 803;
	static const int BOOK_NOT_AVAILABLE = 804;
};



#endif /* LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_ERRORS_H_ */
