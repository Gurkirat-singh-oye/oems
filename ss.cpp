#include <iostream>
// #include <thread>
#include <future>
#include <curl/curl.h>
#include <string>
#include <iterator>
#include <regex>

#include "classDefs.hpp"


// static int recv_any(CURL *curl) {

//     size_t rlen;
//     const struct curl_ws_frame *meta;
//     char buffer[256];

//     CURLcode res = curl_ws_recv(curl, buffer, sizeof(buffer), &rlen, &meta);

//     if (res)
//         return res;
    
//     return 0;
// }

// static int ping(CURL *curl, const std::string *payload) {

//     size_t send;
//     CURLcode res = curl_ws_send(curl, payload, sizeof(payload), &send, 0, CURLWS_PING);

//     return (int)res;
// }

// static void websocket(CURL *curl) {
//     int o = 500;
//     std::string payload = "{ \"jsonrpc\" : \"2.0\",\"id\" : 7538,\"method\" : \"public/get_currencies\",\"params\" : { }}";
//     while (o){
//         recv_any(curl);
//         int tmp = ping(curl, &payload);
//         o--;
//     }
// }

static CURLcode httpApiCall(CURL *curl_http, curl_slist *headers, CURLcode *result_http) {
    curl_easy_setopt(curl_http, CURLOPT_URL, "https://test.deribit.com/api/v2/public/get_currencies?");
    curl_easy_setopt(curl_http, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl_http);
    return res;
}

void wssApiCall(CURL *curl, curl_slist *headers) {

}

int main()
{

    CURL *curl;
    CURLcode result;

    std::string oems_cmd;
    // std::future<CURLcode> resultFuture;

    // apiClass api;

    // std::string payload = "{ \"jsonrpc\" : \"2.0\",\"id\" : 7538,\"method\" : \"public/get_currencies\",\"params\" : { }};";

    // struct curl_slist *headers = NULL;

    // curl = curl_easy_init();

    // if (curl == NULL)
    // {
    //     fprintf(stderr, "curl request failed\n");
    //     return -1;
    // }

    // headers = curl_slist_append(headers, "Content-Type: application/json");

    // std::future<CURLcode> resultFuture = std::async();
    // std::thread httpCall(httpApiCall, &curl, &headers, std::move(result));

    // curl_easy_setopt(curl, CURLOPT_URL, "https://test.deribit.com/api/v2/public/get_currencies?");
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    // curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    // result = curl_easy_perform(curl);
    // result = curl_ws_recv(curl, buffer, sizeof(buffer), &rlen, &meta);

    // result = httpApiCall(curl, headers, &result);

    // result = api.httpCall();

    // if (result != CURLE_OK)
    // {
    //     fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
    //     return -1;
    // } 
    // else {
    //     websocket(curl);
    // }
    
    net::io_context ioc;
    ssl::context ctx(ssl::context::tls_client);

    ctx.set_default_verify_paths();

    apiClass api(ioc, ctx);

    wssLaunch wl;
    std::future<void> idkwtni = std::async(std::launch::async, &wssLaunch::wss_connect, &wl, &api);
    // wl.wss_connect(&api);

    idkwtni.wait();
    // establish a connection and then you authenticate that connection that is you dont have to send access token with every request you make
    // api.makeReq(api.auth);
    // auto jsonObj = api.makeReq(api.buy);

    // std::cout << std::endl << jsonObj << std::endl;


    // this gives me symbols and full names
    // auto jsonObjArr = jsonObj.as_object();
    // auto tmpArr = jsonObjArr["result"];

    utils ut(&api);
    while (true)
    {
        system("clear");
        std::cout << "\n'.help' if you feel stuck.\n";
        std::cout << "\noems: ";

        getline(std::cin, oems_cmd);
        std::regex re("\\s+");  // split on one or more whitespace chars
    
        std::sregex_token_iterator it(oems_cmd.begin(), oems_cmd.end(), re, -1);
        std::sregex_token_iterator end;
    
        std::vector<std::string> cmdNargs(it, end);
        ut.handle_oems_cmd(cmdNargs);
        std::cout << "press ENTER to refresh console.";
        std::getchar();
    }

    // std::cout << json::serialize((tmpArr.as_object())["access_token"]) << std::endl;

    // for ( auto a : tmpArr.as_array()) {
    //     std::cout << a.as_object()["currency"] << "\t" << a.as_object()["currency_long"] << std::endl;
    // }

    curl_easy_cleanup(curl);

    return 0;
}