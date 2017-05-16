#pragma once

namespace m0
{
class machine
{
public:
	void process_a()
	{
		++as_;
	}

	void process_b()
	{
		++bs_;
	}

	void react(char ch)
	{
		if (ch == 'a')
		{
			process_a();
		}
		else
		{
			process_b();
		}
	}

	int get_a() { return as_; }
	int get_b() { return bs_; }
private:
	volatile int as_ = 0;
	volatile int bs_ = 0;
};

} // namespace m0
