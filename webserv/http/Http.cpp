#include "Request.hpp"
#include "Response.hpp"

class Http
{
private:
    Request _request;
    Response _response;

public:
    void 
    ㄱ겨경ㄹ로를_받아서_경_에 맞게 실행(const std::string&str)경로를_받아서_경로_ 맞게 실행    {
    Http::경로를_받아서_경로_ 맞게 실행())std::striing path
    (  
        fstream fs;
        int statusCode;
        std::string msg;
        if (!fs.oepn(path)) {
            statusCode = 404;
            msg = "Not Found";
        }
        _response.setStatusCode(200);
        _response.setMsg("OK");
        fs >> msg;
        _response.setBody(msg);
        return 0;
    }
};

