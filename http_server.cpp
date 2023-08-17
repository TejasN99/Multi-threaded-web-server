#include "http_server.hh"
#include <vector>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>

int counterr=0;

vector<string> split(const string &s, char delim) {
  vector<string> elems;

  stringstream ss(s);
  string item;

  while (getline(ss, item, delim)) {
    if (!item.empty())
      elems.push_back(item);
  }

  return elems;
}

HTTP_Request::HTTP_Request(string request) {
  vector<string> lines = split(request, '\n');
  vector<string> first_line = split(lines[0], ' ');

  this->HTTP_version = "1.0"; // We'll be using 1.0 irrespective of the request

  /*
   TODO : extract the request method and URL from first_line here
  */
  this->method=first_line[0];
  this->url= first_line[1];

  if (this->method != "GET") {
    cerr << "Method '" << this->method << "' not supported" << endl;
    // exit(1);
  }
}

HTTP_Response *handle_request(string req) {
  HTTP_Request *request = new HTTP_Request(req);

  HTTP_Response *response = new HTTP_Response();

  string url = string("./html_files/") + request->url;

  response->HTTP_version = "1.0";

  struct stat sb;

  if (stat(url.c_str(), &sb) == 0) // requested path exists
  {
    response->status_code = "200";
    response->status_text = "OK";
    response->content_type = "text/html";

    string body="";

    if (S_ISDIR(sb.st_mode)) {
      /*
      In this case, requested path is a directory.
      TODO : find the index.html file in that directory (modify the url
      accordingly)
      */
      url=url+"/index.html";
    }
    //cout<<url<<endl;

    /*
    TODO : open the file and read its contents
    */
    char buffer[1024];
    
    FILE *file_open = fopen(url.c_str(),"r");
    while (fgets(buffer,1023,file_open)){
      /* code */
      string temp(buffer); 
      body+=temp;
    }
    fclose(file_open);
    
    /*
    TODO : set the remaining fields of response appropriately
    */

    
    int content_int = body.size();
    response->content_length= to_string(content_int);
    response->body=body;

  }

  else {

    string error_body="";

    response->status_code = "404";
    /*
    TODO : set the remaining fields of response appropriately
    */
    response->status_text = "Page not found";
    response->content_type = "text/html";

    string url_error = string("html_files/error.html");

    FILE *file_open;
    char buffer[1024];
    file_open = fopen(url_error.c_str(),"r");
    

    while (fgets(buffer,1023,file_open)){
      /* code */
      string temp(buffer); 
      error_body+=temp;
    }
    fclose(file_open);
    
    /*
    TODO : set the remaining fields of response appropriately
    */
    int content_int = error_body.size();
    response->content_length= to_string(content_int);
    response->body=error_body;
  }



  delete request;

  return response;
}


string HTTP_Response::get_string() {
  /*
  TODO : implement this function
  */
  string res= "HTTP/1.0 " + this->status_code + " " + this->status_text + "\n";
  res += "Content-Length: " + this->content_length + "\n";
  res += "Content-Type: " + this->content_type + "\n";
  res += "Connection: Closed\n\n";
  res += this->body;

 return res;
}