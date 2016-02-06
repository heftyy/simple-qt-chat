#pragma once

#include <memory>

namespace SimpleChat {

class User;

class Chatee
{
public:
	explicit Chatee(std::unique_ptr<User> user);
	~Chatee();

	int getUserId() const;

private:
	std::unique_ptr<User> user_;
};

}