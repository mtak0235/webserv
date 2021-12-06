#ifndef SERVER_HPP
# define SERVER_HPP

#include "ngxKqueue.hpp"

#define BUFF_SIZE 2048

class Server : public ngxKqueue
{
  public :
    /* constructor & destructor */
    Server();
    virtual ~Server();

    /* public function */
    virtual void acceptNewClient(int servSock);
    virtual int recvDataFromClient(int k);
    virtual void setStatus(void);

  private :
    /* private variable */
    Log _log;
    Cgi _cgi;
    Request _request;
    Response _response;
    LocationConfig _nowLocation;

    std::string _clientReq;
    std::string _body;
    std::string _lastResponse;
    std::string _requestMethod;
    std::string _requestPath;
    std::string _isFile;
    std::vector<std::string> _allowMethods;
    std::vector<std::string> _indexList;
    std::ifstream _ifs;

    char _c;
    char _buf[BUFF_SIZE];
    bool _isAllow;
    size_t _found;
    int _readDataSize;
    int _statusCode;
    int _n;

    /* private funcionts */
    void _setRequestInfo(int k);
    void _setResponse(int k);
    std::string _setBody(std::string file);
    std::string _getBody(std::string file, int k);
    int _responseDatatoServer(int k);
    int _fileJudge(int k);
    void _isDirectory(int k);
    bool _isRequestRemained(const std::string& cliReq) const;
		void clear();
};

#endif
