#pragma once

namespace m1
{
class machine
{
public:
	enum states { A, B};

	void process_a()
	{
		switch (state_)
		{
		case A:
			++as_;
			break;
		case B:
			state_ = A;
			++bs_;
		};
	}

	void process_b()
	{
		switch (state_)
		{
		case A:
			state_ = B;
			++as_;
		case B:
			++bs_;
			break;
		};
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

private:
	int state_ = A;
	volatile int as_ = 0;
	volatile int bs_ = 0;

};

} // namespace m1
