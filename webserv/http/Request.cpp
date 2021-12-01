#include "Request.hpp"

const std::string Request::_availMethods[AVAIL_M]
    = {"GET", "POST", "DELETE"};
const std::string Request::_availHeaderInfos[AVAIL_H]
    = {"Host:", "User-Agent:", "Accept:", "Accept-Language:",
       "Accept-Encoding:", "Accept-Charset:", "Keep-Alive:",
       "Connection:", "Content-Type:", "Content-Length:"};

Request::Request() : _rawBody("")
{

}

Request::Request(const std::string& r) 
{
  _init(r);
}

Request::~Request()
{
}

void Request::setRequest(const std::string& r)
{
	_init(r);
}

void Request::clear()
{
	/* request line */
	_method = "";
	_path = "";
	_httpVersion = "";
	/* request header */
	for (int i = 0; i < AVAIL_H; i++)
		_headerInfo[i] = "";
    _fileInfo.clear();
}

std::string Request::getMethod(void) const
{
	return _method;
}

std::string Request::getPath(void) const
{
	return _path;
}

std::string Request::getHttpVersion(void) const
{
	return _httpVersion;
}

std::string Request::getHeaderInfo(const HEADER_INFO& i) const {
    return _headerInfo[i];
}

void Request::_init(const std::string& r) {
    printf("%s\n", r.c_str());
    std::stringstream ss;
    ss << r;
    std::vector<std::string> v;
    while (!ss.eof()) {
        std::string l;
        std::getline(ss, l);
        v.push_back(l);
    }
    _parseRequestLine(v[0]);
    for (size_t l = 1; l < v.size(); l++) {
        _parseRequestHeader(v[l]);
        if (v[l][0] == '\r') {
					l++;
					while (l < v.size()) {
						_rawBody += v[l];
						l++;
					}
					break ;
        }
    }
    _parseFileInfo();
}

void Request::_parseRequestLine(const std::string& rl) {
    size_t idx = 0;
    for (int m = 0; m < AVAIL_M; m++) {
        if (rl.find(_availMethods[m], idx) == 0) {
            _method = _availMethods[m];
            idx += _availMethods[m].length() + 1;
            break ;
        }
    }
    _path = rl.substr(idx, rl.find(' ', idx + 1) - idx);
    idx += _path.length() + 1;
    _httpVersion = rl.substr(idx, rl.find('\n', idx + 1) - idx);
}

void Request::_parseRequestHeader(const std::string& rh) {
    size_t idx = 0;
    for (int h = 0; h < AVAIL_H; h++) {
        if (rh.find(_availHeaderInfos[h], idx) == 0) {
            idx += _availHeaderInfos[h].length() + 1;
            _headerInfo[h] = rh.substr(idx);
        }
    }
}

std::string Request::getBody() const {
    return _rawBody;
}

void Request::_parseFileInfo(void) {
    FileInfo fi;
    if (!_headerInfo[CONTENT_TYPE].find("multipart/form-data")) {
        const std::string keyBoundary = "boundary=";
        size_t idx = _headerInfo[CONTENT_TYPE].find(keyBoundary);
        fi.boundaryCode = _headerInfo[CONTENT_TYPE].substr(idx + keyBoundary.length());
        fi.boundaryCode.pop_back();
    }
    std::vector<std::string> info;
    std::string tmp = "";
    for (size_t i = 0; i < _rawBody.size(); i++)
    {
        tmp += _rawBody[i];
        if (_rawBody[i] == '\r')
        {   
            tmp.pop_back();
            info.push_back(tmp);
            tmp = "";
        }
    }

    for (size_t i = 0; i < info.size(); i++)
    {
        if (info[i] ==  "--" + fi.boundaryCode)
        {
            if (info[i + 4] == "--" + fi.boundaryCode + "--")
                break;
            //i + 1 cotent diposiion;
            std::stringstream ss(info[i + 1]);
            std::string tmp1, tmp2, tmp3, fName;
            ss >> tmp1 >>tmp2 >> tmp3 >> fName;
            fi.fileName = fName.substr(9);
            ss.clear();
            ss.str(info[i + 2]);
            ss >> tmp1 >> fi.type;
            fi.data = info[i + 4];
        }
    }
    if (fi.boundaryCode.length())
      _fileInfo.push_back(fi);
}

std::vector<FileInfo> Request::getFileInfo(void) const {
    return _fileInfo;
}