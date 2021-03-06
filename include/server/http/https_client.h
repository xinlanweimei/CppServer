/*!
    \file https_client.h
    \brief HTTPS client definition
    \author Ivan Shynkarenka
    \date 12.02.2019
    \copyright MIT License
*/

#ifndef CPPSERVER_HTTP_HTTPS_CLIENT_H
#define CPPSERVER_HTTP_HTTPS_CLIENT_H

#include "http_request.h"
#include "http_response.h"

#include "server/asio/ssl_client.h"

namespace CppServer {
namespace HTTP {

//! HTTPS client
/*!
    HTTPS client is used to communicate with HTTPS Web server.
    It allows to send GET, POST, PUT, DELETE requests and
    receive HTTP result using secure transport.

    Thread-safe.
*/
class HTTPSClient : public Asio::SSLClient
{
public:
    using SSLClient::SSLClient;

    HTTPSClient(const HTTPSClient&) = delete;
    HTTPSClient(HTTPSClient&&) = delete;
    virtual ~HTTPSClient() = default;

    HTTPSClient& operator=(const HTTPSClient&) = delete;
    HTTPSClient& operator=(HTTPSClient&&) = delete;

    //! Get the HTTP request
    HTTPRequest& request() noexcept { return _request; }
    const HTTPRequest& request() const noexcept { return _request; }

    //! Send the current HTTP request (synchronous)
    /*!
        \return Size of sent data
    */
    size_t SendRequest() { return SendRequest(_request); }
    //! Send the HTTP request (synchronous)
    /*!
        \param request - HTTP request
        \return Size of sent data
    */
    size_t SendRequest(const HTTPRequest& request) { return Send(request.cache()); }

    //! Send the HTTP request body (synchronous)
    /*!
        \param body - HTTP request body
        \return Size of sent data
    */
    size_t SendRequestBody(std::string_view body) { return Send(body); }
    //! Send the HTTP request body (synchronous)
    /*!
        \param buffer - HTTP request body buffer
        \param size - HTTP request body size
        \return Size of sent data
    */
    size_t SendRequestBody(const void* buffer, size_t size) { return Send(buffer, size); }

    //! Send the current HTTP request with timeout (synchronous)
    /*!
        \param timeout - Timeout
        \return Size of sent data
    */
    size_t SendRequest(const CppCommon::Timespan& timeout) { return SendRequest(_request, timeout); }
    //! Send the HTTP request with timeout (synchronous)
    /*!
        \param request - HTTP request
        \param timeout - Timeout
        \return Size of sent data
    */
    size_t SendRequest(const HTTPRequest& request, const CppCommon::Timespan& timeout) { return Send(request.cache(), timeout); }

    //! Send the HTTP request body with timeout (synchronous)
    /*!
        \param body - HTTP request body
        \param timeout - Timeout
        \return Size of sent data
    */
    size_t SendRequestBody(std::string_view body, const CppCommon::Timespan& timeout) { return Send(body, timeout); }
    //! Send the HTTP request body with timeout (synchronous)
    /*!
        \param buffer - HTTP request body buffer
        \param size - HTTP request body size
        \param timeout - Timeout
        \return Size of sent data
    */
    size_t SendRequestBody(const void* buffer, size_t size, const CppCommon::Timespan& timeout) { return Send(buffer, size, timeout); }

    //! Send the current HTTP request (asynchronous)
    /*!
        \return 'true' if the current HTTP request was successfully sent, 'false' if the client is not connected
    */
    bool SendRequestAsync() { return SendRequestAsync(_request); }
    //! Send the HTTP request (asynchronous)
    /*!
        \param request - HTTP request
        \return 'true' if the current HTTP request was successfully sent, 'false' if the client is not connected
    */
    bool SendRequestAsync(const HTTPRequest& request) { return SendAsync(request.cache()); }

    //! Send the HTTP request body (asynchronous)
    /*!
        \param body - HTTP request body
        \return 'true' if the current HTTP request was successfully sent, 'false' if the client is not connected
    */
    bool SendRequestBodyAsync(std::string_view body) { return SendAsync(body); }
    //! Send the HTTP request body (asynchronous)
    /*!
        \param buffer - HTTP request body buffer
        \param size - HTTP request body size
        \return 'true' if the current HTTP request was successfully sent, 'false' if the client is not connected
    */
    bool SendRequestBodyAsync(const void* buffer, size_t size) { return SendAsync(buffer, size); }

protected:
    void onReceived(const void* buffer, size_t size) override;
    void onDisconnected() override;

    //! Handle HTTP response header received notification
    /*!
        Notification is called when HTTP response header was received
        from the server.

        \param response - HTTP response
    */
    virtual void onReceivedResponseHeader(const HTTPResponse& response) {}

    //! Handle HTTP response error notification
    /*!
        Notification is called when HTTP response error was received
        from the server.

        \param response - HTTP response
        \param error - HTTP response error
    */
    virtual void onReceivedResponseError(const HTTPResponse& response, const std::string& error) {}

    //! Handle HTTP response received notification
    /*!
        Notification is called when HTTP response was received
        from the server.

        \param response - HTTP response
    */
    virtual void onReceivedResponse(const HTTPResponse& response) {}

private:
    // HTTP request
    HTTPRequest _request;
    // HTTP response
    HTTPResponse _response;
};

/*! \example https_client.cpp HTTPS client example */

} // namespace HTTP
} // namespace CppServer

#endif // CPPSERVER_HTTP_HTTPS_CLIENT_H
