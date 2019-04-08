#include "CReceiver.hpp"


CReceiver::CReceiver () {
  const uint16_t port = get_available_port();
  sockfd_ = get_ready_sockrfd(port);
}

CReceiver::~CReceiver () {
  close_sockrfd(sockfd_);
}

void CReceiver::Receive () const {

}


void CReceiver::makeHandshake () {

}

