#pragma once

namespace m1
{
class machine
{
public:
	enum states { A, B};

	void react(char ch)
	{
		switch (state_)
		{
			case A: 
				if (ch == 'a')
				{
					++as_;
				}
				else 
				{
					state_ = B;
					react(ch);
				}
				break;
			case B:
				if (ch == 'b')
				{
					++bs_;
				}
				else
				{
					state_ = A;
					react(ch);
				}
		};
	}

private:
	int state_ = A;
	volatile int as_ = 0;
	volatile int bs_ = 0;

};

} // namespace m1
