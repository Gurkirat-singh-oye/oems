flags = -Wall --no-pie -fPIE
srcs = classDefs.cpp ss.cpp
linklibs = -lcurl -lboost_system -lboost_thread -lssl -lcrypto -lpthread -lboost_json

all:
	g++ -o deribit_oems $(flags) $(srcs) $(linklibs)

# classDefs.cpp: classDefs.cpp
# 	g++ -c classDefs.cpp

# ss.cpp: ss.cpp
# 	g++ -c ss.cpp