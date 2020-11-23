#include "pch.h"
#include "equation.h"
#include <fstream>
#include <iomanip>

equation::equation(int N, double L, std::map<std::string, double> _consts) : 
    Lx(L), NumPoint(N), h(L / N), consts(_consts), T(vec(0.0, N)), v(vec(0.0, N)), dT(vec(0.0, N)), dv(vec(0.0, N))
{
	dt = h * h / 4.0;
}

void equation::SetInitial(vec _T, vec _v)
{
	T = _T;
	v = _v;
}


void diff(vec& T, vec& dT)
{
	for (size_t i = 1; i < T.size() - 2; ++i)
		dT[i] = T[i + 1] + T[i - 1] - 2 * T[i];
}



void equation::step()
{
	double NuDivHH = consts["Nu"] /(h*h);
	double HiDivHH = consts["Hi"] /(h*h);
	double dth = dt * h;
	double param = consts["DeltaP"] / consts["Ro"];
		
	diff(v, dv);
	v = v + dt * ( dv * NuDivHH - param);

	diff(T, dT);
	T = T + dt*(HiDivHH * dT + v*(T.shift(-1) - T.shift(1))/(2*h));
	 
}

void equation::write(std::string adr)
{
	std::ofstream out;
	out.open(adr);
	
	out << std::setw(5) << "x";
	out << std::setw(15) << "v";
	out << std::setw(15) << "T";
	out << std::endl;
	for (int i = 0; i < NumPoint; i++)
	{
		out << std::setw(5) << i * h;
		out << std::setw(15) << v[i];
		out << std::setw(15) << T[i];
		out << std::endl;
	}
	out.close();
}

equation::~equation()
{

}
