#include <curl/curl.h>
#include <future>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/json.hpp>
#include <boost/unordered_map.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
namespace json = boost::json;
using tcp = net::ip::tcp;

class apiClass
{
public:
    static CURL *curl;
    // static std::string accessToken;

    enum apiCallKeys
    {
        get_currencies,
        auth,
        buy,
    };
    std::unordered_map<apiCallKeys, json::value> apiList = {
        {get_currencies, json::object{
                             {"method", "public/get_currencies"},
                             {"params", json::object{{}, {}}}}},
        {auth, json::object{{"method", "public/auth"}, {"params", json::object{{"grant_type", "client_credentials"}, {"client_id", "INR1NzDj"}, {"scope", "session:apiconsole-l4796p4dcjm expires:2592000"}, {"client_secret", "TxbYH8R_NYlPUfShfDKLjjCMVl1ZISBzsCu72EkG9fE"}}}}},
        {buy, json::object{{"method", "private/buy"}, {"params", json::object{
                                                                     {"instrument_name", "ETH-PERPETUAL"},
                                                                     {"amount", 4},
                                                                     {"price", 3356},
                                                                 }}}}};

    apiClass(net::io_context &ioc, ssl::context &ctx) : ws_(ioc, ctx), ctx_(ctx) {};

    CURLcode httpCall(std::string url);
    json::value wssWrite(apiCallKeys ack, std::string method, std::string params);
    json::value wssRead();

    json::value makeReq(apiCallKeys ack);
    void breakRes();

    websocket::stream<beast::ssl_stream<tcp::socket>> ws_;
    ssl::context &ctx_;
    beast::flat_buffer buffer;
};

class wssLaunch
{
public:
    void wss_connect(apiClass *ws_stream_obj);
};

class orderClass
{

public:
    orderClass();
    void placeOrder();
    void cancelOrder();
    void modifyOrder();
    void getBook();
    void getOpenPositions();
    void getSupportedIndexNames();
};

class utils
{
public:

    utils(apiClass *api) {api_ctx_ = api;};

    boost::unordered_map<std::string, std::function<void()>> cmd_map =
        {{
            ".q", [](){
            std::cout << "goodbye!\n";
            exit(0);
            }
          },
          {
            ".getcurs", [this](){
            auto jsonObj = (*api_ctx_).makeReq((*api_ctx_).get_currencies);
            std::cout << std::endl << jsonObj << std::endl;
            }
          },
          {
            ".help", [](){
            std::cout << ".q\t\t:to exit\n.getcurs\t:to get currencies\n";
            }
          }};

    void handle_oems_cmd(std::string cmd);
    apiClass *api_ctx_;
};

// class apiClass
// {
// public:
//     static CURL *curl;

//     apiClass()
//     {
//         curl = curl_easy_init();
//     }

//     CURLcode httpCall()
//     {

//         struct curl_slist *headers = NULL;
//         headers = curl_slist_append(headers, "Content-Type: application/json");

//         curl_easy_setopt(curl, CURLOPT_URL, "https://test.deribit.com/api/v2/public/get_currencies?");
//         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

//         CURLcode res = curl_easy_perform(curl);

//         return res;
//     }
// };