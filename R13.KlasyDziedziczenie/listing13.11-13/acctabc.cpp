#include <iostream>
#include "acctabc.h"

using std::cout;
using std::endl;
using std::string;
using std::ios_base;

AcctABC::AcctABC(const std::string &s, long an, double bal) {
	fullName = s;
	acctNum = an;
	balance = bal;
}

void AcctABC::Deposit(double amt) {
	if (amt < 0)
		cout << "Nie mozna wplacic ujemnej kwoty; Wplata anulowana\n";
	else
		balance += amt;
}

void AcctABC::Withdraw(double amt) {
	balance -= amt;
}

AcctABC::Formatting AcctABC::SetFormat() const {
	Formatting f;
	f.flag = cout.setf(ios_base::fixed, ios_base::floatfield);
	f.pr = cout.precision(2);
	return f;
}

void AcctABC::Restore(Formatting &f) const
{
	cout.setf(f.flag, ios_base::floatfield);
	cout.precision(f.pr);
}

void Brass::Withdraw(double amt) {
	if (amt < 0)
		cout << "Nie mozna wyplacic ujemnej kwoty; Wyplata anulowana\n";
	else if (amt <= Balance())
		AcctABC::Withdraw(amt);
	else
		cout << "Zadana suma " << amt
		<< " zl przekracza dostepne srodki.\n"
		<< "Wyplata anulowana.\n";
}

void Brass::ViewAcct() const {
	Formatting f = SetFormat();
	cout << "Klient Brass: " << FullName() << endl;
	cout << "Numer rachunku: " << AcctNum() << endl;
	cout << "Stan konta: " << Balance() << " zl" << endl;
	Restore(f);
}

BrassPlus::BrassPlus(const std::string &s, long an, double bal, double ml, double r) : AcctABC(s, an, bal)
{
	maxLoan = ml;
	owesBank = 0.0;
	rate = r;
}

BrassPlus::BrassPlus(const Brass &ba, double ml, double r) : AcctABC(ba) {
	maxLoan = ml;
	owesBank = 0.0;
	rate = r;
}

void BrassPlus::ViewAcct() const {
	Formatting f = SetFormat();
	cout << "Klient BrassPlus: " << FullName() << endl;
	cout << "Numer rachunku: " << AcctNum() << endl;
	cout << "Stan konta: " << Balance() << " zl" << endl;
	cout << "Limit debetu: " << maxLoan << "zl" << endl;
	cout << "Kwota zadluzenia: " << owesBank << "zl" << endl;
	cout.precision(3);
	cout << "Stopa oprocentowania: " << 100 * rate << "%\n";
	Restore(f);
}

void BrassPlus::Withdraw(double amt) {
	Formatting f = SetFormat();

	double bal = Balance();
	if (amt <= bal)
		AcctABC::Withdraw(amt);
	else if (amt <= bal + maxLoan - owesBank)
	{
		double advance = amt - bal;
		owesBank += advance * (1.0 + rate);
		cout << "Zadluzenie faktyczne: " << advance << "zl" << endl;
		cout << "Odsetki: " << advance * rate << "zl" << endl;
		Deposit(advance);
		AcctABC::Withdraw(amt);
	}
	else
		cout << "Przekroczony limit debetu. Operacja anulowana.\n";
	Restore(f);
}