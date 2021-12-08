#include "Request.hpp"

const std::string Request::_availMethods[AVAIL_M]
    = {"GET", "POST", "DELETE"};
const std::string Request::_availHeaderInfos[AVAIL_H]
    = {"Host:", "User-Agent:", "Accept:", "Accept-Language:",
       "Accept-Encoding:", "Accept-Charset:", "Keep-Alive:",
       "Connection:", "Content-Type:", "Content-Length:",
       "Content-Range", "Content-Disposition", "Content-Encoding",
       "Transfer-Encoding", "Last-Modified", "Content-Language"};

Request::Request() { }

Request::Request(const std::string& r)
{
  _init(r);
}

Request::~Request() { }

void Request::clear(void)
{
  _method = "";
  _path = "";
  _httpVersion = "";
  for (int i = 0; i < AVAIL_H; i++)
    _headerInfo[i] = "";
  _rawBody = "";
  _fileInfo.clear();
}

void Request::setRequest(const std::string& r)
{
  _init(r);
}


std::string Request::getMethod(void) const
{
  return _method;
}

std::string Request::getPath(void) const
{
  return _path;
}

void Request::setPath(const std::string& path)
{
  _path = path;
}

std::string Request::getHttpVersion(void) const
{
  return _httpVersion;
}

std::string Request::getHeaderInfo(const HEADER_INFO& i) const
{
    return _headerInfo[i];
}

std::string Request::getBody() const
{
  return _rawBody;
}

std::vector<FileInfo> Request::getFileInfo(void) const
{
  return _fileInfo;
}

void Request::_init(const std::string& r)
{
  std::stringstream ss;
  ss << r;
  std::vector<std::string> v;
  while (!ss.eof())
  {
    std::string l;
    std::getline(ss, l);
    l += '\n';
    v.push_back(l);
  }
  _parseRequestLine(v[0]);
  for (size_t l = 1; l < v.size(); l++)
  {
    _parseRequestHeader(v[l]);
    if (v[l][0] == '\r')
    {
      l++;
      while (l < v.size())
      {
        _rawBody += v[l];
        l++;
      }
      break ;
    }
  }
  _parseFileInfo();
}

void Request::_parseRequestLine(const std::string& rl)
{
  size_t idx = 0;
  for (int m = 0; m < AVAIL_M; m++) {
    if (rl.find(_availMethods[m], idx) == 0)
    {
      _method = _availMethods[m];
      idx += _availMethods[m].length() + 1;
      break ;
    }
  }
  _path = rl.substr(idx, rl.find(' ', idx + 1) - idx);
  idx += _path.length() + 1;
  _httpVersion = rl.substr(idx, rl.find('\n', idx + 1) - idx);
}

void Request::_parseRequestHeader(const std::string& rh)
{
  size_t idx = 0;
  for (int h = 0; h < AVAIL_H; h++)
  {
    if (rh.find(_availHeaderInfos[h], idx) == 0)
    {
      idx += _availHeaderInfos[h].length() + 1;
      _headerInfo[h] = rh.substr(idx);
    }
  }
}

void Request::_parseFileInfo(void)
{
  FileInfo fi;
  if (!_headerInfo[CONTENT_TYPE].find("multipart/form-data"))
  {
    const std::string keyBoundary = "boundary=";
    size_t idx = _headerInfo[CONTENT_TYPE].find(keyBoundary);
    fi.boundaryCode = _headerInfo[CONTENT_TYPE].substr(idx + keyBoundary.length());
  }
  std::vector<std::string> info;
  std::vector<size_t> checkIdx;
  std::string tmp = "";
  std::string endCode = fi.boundaryCode;
  endCode.pop_back();
  endCode.pop_back();
  endCode += "--\r\n";
  for (size_t i = 0; i < _rawBody.size(); i++)
  {
    tmp += _rawBody[i];
    if (i > 0 && _rawBody[i] == '\n' && _rawBody[i - 1] == '\r')
    {
      info.push_back(tmp);
      if (tmp == "--" + fi.boundaryCode)
        checkIdx.push_back(info.size() - 1);
      if (tmp == "--" + endCode)
        checkIdx.push_back(info.size() - 1);
      tmp = "";
    }
  }
  if (checkIdx.empty())
    return;
  for (size_t i = 0; i < checkIdx.size() - 2; i++)
  {
    FileInfo f;
    f.boundaryCode = fi.boundaryCode;
    int j = checkIdx[i];
    std::stringstream ss(info[j + 1]);
    std::string contentDisposition, formData, name, fileName;
    ss >> contentDisposition >> formData >> name >> fileName;
    f.fileName = fileName.substr(10);
    f.fileName.pop_back();
    ss.clear();
    ss.str(info[j + 2]);
    std::string tmp;
    ss >> tmp >> f.type;
    f.data = "";
    for (size_t kk = j + 4; kk < checkIdx[i + 1]; kk++)
      f.data += info[kk];
    f.data.pop_back();
    f.data.pop_back();
    _fileInfo.push_back(f);
  }
}
