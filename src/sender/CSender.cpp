#include "CSender.hpp"


CSender::CSender (std::string ip)
    : ip_(std::move(ip)) {}

CSender::~CSender () {
  close_socksfd(sockfd_);
}

void CSender::Send (std::string file_path, size_t threads_amt) {
  file_path_ = std::move(file_path);
  threads_amt_ = threads_amt;

}

void CSender::makeHandshake () {

}
