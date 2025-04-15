#pragma once

#include <string>

typedef struct APPLICATION_INIT_DESC
{
	int Width;
	int Height;
	std::wstring WindowName;
}APPLICATION_INIT_DESC;

class IApplication
{
public:
	IApplication() = default;
	virtual ~IApplication() = default;

	IApplication(IApplication const&) = default;
	IApplication& operator=(IApplication const&) = default;

	IApplication(IApplication&&) = default;
	IApplication& operator=(IApplication&&) = default;

	virtual bool Init(const APPLICATION_INIT_DESC* desc) = 0;
	virtual int Execute() = 0;
};
