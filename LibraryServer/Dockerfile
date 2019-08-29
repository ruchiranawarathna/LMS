FROM ubuntu:latest
FROM  gcc:7.4.0

RUN mkdir -p /home/LibrarySystem
COPY . /home/LibrarySystem

RUN apt update && apt -y install build-essential make && rm -rf /var/lib/apt/lists/*
#RUN apt-get install g++ git libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev libwebsocketpp-dev openssl libssl-dev ninja-build
RUN apt update && apt -y install libboost-dev
#RUN apt update && apt -y install libboost-all-dev
RUN apt update && apt -y install libcpprest-dev
WORKDIR /home/LibrarySystem
CMD [ "/usr/bin/make" ]
#CMD g++ -std=c++11 /home/hsenid/LibrarySystem/src/Main.cpp /home/hsenid/LibrarySystem/src/Server.cpp /home/hsenid/LibrarySystem/src/ConfigHandler.cpp  /home/hsenid/LibrarySystem/src/HttpRequestParser.cpp /home/hsenid/LibrarySystem/src/reporting/CSVHandler.cpp /home/hsenid/LibrarySystem/src/db/DBManager.cpp /home/hsenid/LibrarySystem/src/notify/SMSNotifier.cpp  /home/hsenid/LibrarySystem/src/utils/DateUtils.cpp -o server -lboost_system -lcrypto -lssl -lcpprest -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0 -L/usr/local/lib -lmongocxx -lbsoncxx  -lcurl
CMD /home/LibrarySystem/server
