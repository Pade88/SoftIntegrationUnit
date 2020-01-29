#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <algorithm>


class parts
{
private:
	float internalClock;
public:
	static bool OR(bool _first, bool _second)
	{
		return _first | _second;
	};

	static bool OR(bool _first, bool _second, bool _third)
	{
		return _first | _second | _third;
	};

	static bool OR(bool _first, bool _second, bool _third, bool _fourth)
	{
		return _first | _second | _third | _fourth;
	};

	static bool AND(bool _first, bool _second)
	{
		return _first & _second;
	};

	static bool AND(bool _first, bool _second, bool _third)
	{
		return _first & _second & _third;
	};

	static bool AND(bool _first, bool _second, bool _third, bool _fourth)
	{
		return _first & _second & _third & _fourth;
	};

	static bool SRLatch(bool _SETVAL, static bool _RSTVAL)
	{
		if (_RSTVAL)
			return false;
		else
			return _SETVAL;
	}

	static bool RLATCH(float _DT, float _MAXTIME, float _RECOVERYTIME, bool _RST, bool _INPUT)
	{
		int _iterationCounter = 0;
		float _elapsedT = 0.0f;
		bool timeReached = false;
		bool _internINPUT = _INPUT;

		if (_RST)
			return false;
		
		while (_RECOVERYTIME > 0)
		{
			if (timeReached)
			{
				_INPUT = !_internINPUT;
			}

			Sleep(_DT);
			std::cout << "Time: " << _elapsedT << "\tIteration: " << _iterationCounter << "\tInput: " << !_INPUT << std::endl;
			_elapsedT += _DT;

			_iterationCounter++;
			_RECOVERYTIME -= _DT;

			if (_elapsedT < _MAXTIME && !timeReached)
				continue;
			else
				timeReached = true;
		}
	}

	static void Filter_MZ(float _init, float _target, float _dt, float _time)
	{
		float iterations = _time / _dt;
		float incrementVal = (_target - _init) / iterations;
		float _elapsedT = 0.0f;
		float value = _init;

		while (iterations > 0)
		{
			std::cout << "Time: " << _elapsedT << "\titeration: " << (_time/_dt) - iterations << "\tvalue: " << value << std::endl;
			value += incrementVal;
			_elapsedT += _dt;
			iterations--;
			Sleep(_dt);
		}
	}

	void serialPrint(std::string _bar)
	{
		std::cout << _bar << std::endl;
	}

	float getInternalClock()
	{
		return this->internalClock;
	}

	void setInternalClock(float _reference)
	{
		this->internalClock = _reference;
	}

	static void ThreadID()
	{
		std::cout << "A fost apelat threadul cu ID-ul " << std::this_thread::get_id() << std::endl;
	}

	static void counter()
	{
		int internIndex = rand();

		std::cout << "ID-ul firului de executie este: " << std::this_thread::get_id() << std::endl;
		while (internIndex --> 0)
		{
			std::cout << "Astept o iteratie, au mai ramas " << internIndex << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	/* 
	@Author: OPA 30.12.2019
	Argument: a vector of integers
	loop through all vector and transoforms every <<1>> value to <<TRUE>> and every
	<<0>> value to <<FALSE>>
	*/
	static std::vector<bool>IntegerToBooleanVecConvertor(std::vector<int> X1)
	{
		std::vector<bool>returnedBool;
		for (auto iterator : X1)
		{
			if (iterator)
			{
				returnedBool.push_back(true);
			}
			else
			{
				returnedBool.push_back(false);
			}
		}
		return returnedBool;
	}
};


class ALU
{
public:

	/*
	@Author: OPA 31.12.2019
	@Arguments: 2 vectors of integers containing bits of numbers to be added
	@ToBeImplemented: NegativeFlag, OverflowFlag.
	@Internal variables:
		carryFlag --- Carry flag is active if at the end of parsing, there is still one carry left, wich means that
						addition goes over boundaries. Another bit with value <<1>> is added in the most significant position
						to maintain consistency of adding process.
		NegativeFlag --- Is active if comparison 2 vectors are negative.
		OverflowFlag --- Is active if result is over 8 bit(256) boundaries.
		inputVectorLength --- Is the actual lenght of input vector.
		BadVectorSizeFlag --- Is active if input vectors don't have the same size. (Warning, not error)
		combinedVector -- Contains pairs with bits placed on same position in the numbers
		carry --- Carry flag is active when both bits that follows to be added are equal to 1.
		LPVcarry --- Last Pass Value of carry represent value from previous adding process.
		bitAdded --- Result of adding bits from same position in numbers using XOR.
	*/
	static std::vector<int> full8bitAdder(std::vector<int>first, std::vector<int> second)
	{
		bool carryFlag = false;
		bool NegativeFlag = false;
		bool OverflowFlag = false;
		int inputVectorLength;
		std::vector<int>sum;
		bool BadVectorSizeFlag = !((first.size() - second.size()) == 0) ? true : false;

		if (BadVectorSizeFlag)
		{
			while (BadVectorSizeFlag)
			{
				if (first.size() > second.size())
					second.insert(second.begin(), 0);
				else
					first.insert(first.begin(), 0);

				BadVectorSizeFlag = !((first.size() - second.size()) == 0) ? true : false;
			}
			
		}

		inputVectorLength = first.size(); // Default size **Sice <<first>> and <<second>> are equals does not influence.
		std::vector<std::pair<int, int>>combinedVector;

		for (int element = 0; element < inputVectorLength; element++)
		{
			combinedVector.push_back(std::pair<int, int>(first.at(element), second.at(element))); 
		}
	
		/*
		for (auto i : combinedVector)
		{
			std::cout << i.first << "\t" << i.second << std::endl;
		}
		*/

		/* Since addition is done from right to left, vector of pairs must be reversed*/
		std::reverse(combinedVector.begin(), combinedVector.end());
		int carry = 0;
		int LPVcarry = 0;
		int bitAdded = 0;
		int SupplementCarry = 0;
		int parser = 0;
		for (auto bit : combinedVector)
		{
			carry = bit.first & bit.second | 0 ;
			bitAdded = bit.first ^ bit.second; 

			if (!carry && !LPVcarry)        
				sum.push_back(bitAdded);
			else
			{
				if (bitAdded && LPVcarry)
					carry = 1;
				bitAdded = bitAdded ^ LPVcarry;
				sum.push_back(bitAdded);
			}
			LPVcarry = carry;
			parser++;

			if (parser == inputVectorLength && carry || LPVcarry)
				carryFlag = true;
		}
		if (carryFlag)
			sum.push_back(1);
		return sum;
	}

	/* 
	@Author: OPA 30.12.2019
	@Argument: a vector with ints representing every bit of number.
	Looping through vector, indexing every loop and where there is a bit
	with value <<1>> returned number add 2 at power index.
	*/
	static int bitsToDec(std::vector<int> bits)
	{
		int composedNumber = 0;
		int index = 0;
		for (auto parser : bits)
		{
			if (parser)
			{
				composedNumber += pow(2, index);
				index++;
			}
			else
			{
				index++;
			}
		}
		return composedNumber;
	}

	static std::vector<int> completeVector(std::vector<int> tbC, int size)
	{
		std::vector<int>_internalCopy = tbC;
		while (tbC.size() < size)
		{
			_internalCopy.push_back(0);
		}
		return _internalCopy;
	}
};

auto compare = [](auto a, auto b) {return a < b; };

bool check(int x)
{
	bool intern = x != 1 && x != 2 ? true : false;
	return intern;
}

void main()
{
	/*
	std::thread executionThread(parts::Filter_MZ, 15, 18, 0.192, 5);
	std::thread counterThread(parts::Filter_MZ, 8, 15, 0.192, 5);
	
	counterThread.join();	
	std::this_thread::sleep_for(std::chrono::seconds(10));
	executionThread.join();
	*/

	/*
	std::thread T1(parts::ThreadID);
	std::thread T2(parts::ThreadID);

	T1.join(); T2.join();

	std::thread T3(parts::counter);
	T3.join();
	*/

	/*
	std::vector<int>primulNumar = {1, 1, 1, 0, 0, 1, 1, 1}; //231
	//std::vector<int>aldoileaNumar = {1, 1, 0, 0, 1, 0, 1, 1}; //203
	std::vector<int>aldoileaNumar = { 1 }; //203
	std::cout<<ALU::bitsToDec(ALU::full8bitAdder(primulNumar, aldoileaNumar));
	*/
	std::cout << check(5);

}