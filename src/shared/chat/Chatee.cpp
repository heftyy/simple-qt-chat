#include "Chatee.h"
#include "proto/User.pb.h"

namespace SimpleChat {

Chatee::Chatee(std::unique_ptr<User> user) :
	user_(std::move(user)) {

}


Chatee::~Chatee() {
}

int Chatee::getUserId() const {
	return user_->id();
}

}