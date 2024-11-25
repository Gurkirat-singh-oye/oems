#include <curl/curl.h>
#include <string>
#include <iostream>

#include "classDefs.hpp"

CURL *apiClass::curl = curl_easy_init();

CURLcode apiClass::httpCall (std::string url)
{

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(this->curl, CURLOPT_URL, url);
    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);

    return res;
}

json::value apiClass::wssRead ()
{

    ws_.read(buffer);
    std::cout << "\nReceived message: " << beast::make_printable(buffer.data()) << std::endl;

    std::string data = beast::buffers_to_string(buffer.data());
    json::value resJson = json::parse(data);
    // std::cout << std::endl << resJson.as_object()["result"] << std::endl;

    return resJson;
}

json::value apiClass::wssWrite (apiCallKeys ack, std::string method, std::string params)
{

    // auto method = apiList.at(ack);
    std::string reqMsg = "{ \"jsonrpc\" : \"2.0\", \"id\" : 9929,\"method\" : " + method + ",\"params\" : " + params + " }";
    ws_.write(net::buffer(reqMsg));
    std::cout << "Sent message: " << reqMsg << std::endl;

    json::value resJson = wssRead();

    return resJson;
}

json::value apiClass::makeReq (apiCallKeys ack) {

    // TODO: if its a private method, add access_token

    json::value reqObj = apiList.at(ack);
    std::string method = json::serialize(reqObj.as_object()["method"]);
    std::string params = json::serialize(reqObj.as_object()["params"]);

    return wssWrite(ack, method, params);

}

// Forms a Websocket connection (last mile for initializing Websocket connection)
void wssLaunch::wss_connect (apiClass *wso)
{

    // These will hold the resolved IP and port
    std::string host = "test.deribit.com"; // WebSocket testing server
    std::string port = "443";

    // Resolver for converting hostname to IP address
    tcp::resolver resolver((*wso).ws_.get_executor());
    auto const results = resolver.resolve(host, port);

    // Create a WebSocket stream connected to the given endpoint
    net::connect((*wso).ws_.next_layer().next_layer(), results.begin(), results.end());

    // Perform SSL handshake
    (*wso).ws_.next_layer().handshake(ssl::stream_base::client);
    (*wso).ws_.handshake(host, "/ws/api/v2");

    std::cout << "socket connected." << std::endl;

}

void orderClass::getSupportedIndexNames ()
{
}

void orderClass::placeOrder ()
{
    // apiClass api;
    // api.httpCall("https://test.deribit.com/api/v2/public/status?");
}